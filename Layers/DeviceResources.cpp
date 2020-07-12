#include "Pch.h"
#include "DeviceResources.h"


DeviceResources::DeviceResources()
{
	//��ʼ��D3D11
	Microsoft::WRL::ComPtr<ID3D11Device> d3dDevice;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> d3dDeviceContext;
	
	UINT creationFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;
#ifdef _DEBUG
	creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif // _DEBUG

	D3D11CreateDevice(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		creationFlags,
		nullptr,
		0,
		D3D11_SDK_VERSION,
		&d3dDevice,
		&m_d3dFeatureLevel,
		&d3dDeviceContext);
	d3dDevice.As(&m_d3dDevice);
	d3dDevice.As(&m_dxgiDevice);
	d3dDeviceContext.As(&m_d3dContext);

#ifdef _DEBUG
	UINT dxgiFlags = DXGI_CREATE_FACTORY_DEBUG;
#else
	UINT dxgiFlags = 0;
#endif // _DEBUG
	CreateDXGIFactory2(dxgiFlags, IID_PPV_ARGS(&m_dxgiFactory));

	//����Direct2D����
	D2D1CreateFactory(
		D2D1_FACTORY_TYPE_SINGLE_THREADED,
		IID_PPV_ARGS(&m_d2dFactory));

	//����Direct2D�豸
	m_d2dFactory->CreateDevice(
		m_dxgiDevice.Get(), 
		&m_d2dDevice);

	m_d2dDevice->CreateDeviceContext(
		D2D1_DEVICE_CONTEXT_OPTIONS_NONE,
		&m_d2dContext);

	//��ʼ��DirectWrite
	DWriteCreateFactory(
		DWRITE_FACTORY_TYPE_SHARED,
		__uuidof(m_dwriteFactory),
		&m_dwriteFactory);

	//��ʼ��WIC������˴�û�е���CoInitialize���������������ú�����ִ��COM��ʼ����
	CoCreateInstance(
		CLSID_WICImagingFactory2,
		nullptr,
		CLSCTX_INPROC_SERVER,
		IID_PPV_ARGS(&m_wicFactory));
}


DeviceResources::~DeviceResources()
{
}


void DeviceResources::CreateWindowDependentResources()
{
	DXGI_SWAP_CHAIN_DESC1 swapChainDesc = { 0 };

	swapChainDesc.Width = m_clientSize.width;
	swapChainDesc.Height = m_clientSize.height;
	swapChainDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	swapChainDesc.Stereo = false;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.BufferCount = BufferCount;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL; 
	swapChainDesc.Flags = 0;
	swapChainDesc.AlphaMode = DXGI_ALPHA_MODE_IGNORE; 
	swapChainDesc.Scaling = DXGI_SCALING_NONE;

	Microsoft::WRL::ComPtr<IDXGISwapChain1> swapChain;
	m_dxgiFactory->CreateSwapChainForHwnd(
		m_d3dDevice.Get(),
		m_hWnd,
		&swapChainDesc,
		nullptr,
		nullptr,
		&swapChain);
	swapChain.As(&m_swapChain);

	SetRenderTargetBitmap();
}

void DeviceResources::CreateSizeDependentResources()
{
	if (m_swapChain == nullptr)
		return;

	ID3D11RenderTargetView* nullViews[] = { nullptr };
	m_d3dContext->OMSetRenderTargets(ARRAYSIZE(nullViews), nullViews, nullptr);
	m_d2dContext->SetTarget(nullptr);
	m_d2dTargetBitmap = nullptr;
	m_d3dContext->Flush();

	m_swapChain->ResizeBuffers(
		2,
		m_clientSize.width,
		m_clientSize.height,
		DXGI_FORMAT_B8G8R8A8_UNORM, 0);

	SetRenderTargetBitmap();
}

void DeviceResources::SetRenderTargetBitmap()
{
	Microsoft::WRL::ComPtr<IDXGISurface2> dxgiBackBuffer;
	m_swapChain->GetBuffer(0, IID_PPV_ARGS(&dxgiBackBuffer));
	D2D1_BITMAP_PROPERTIES1 bitmapProperties = D2D1::BitmapProperties1(
		D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW,
		D2D1::PixelFormat(
			DXGI_FORMAT_B8G8R8A8_UNORM,
			D2D1_ALPHA_MODE_PREMULTIPLIED));

	m_d2dContext->CreateBitmapFromDxgiSurface(
		dxgiBackBuffer.Get(),
		&bitmapProperties,
		&m_d2dTargetBitmap);

	//����D2D��ȾĿ��
	m_d2dContext->SetTarget(m_d2dTargetBitmap.Get());

	//���ý������ı�����ɫΪ͸������ֹ���ڳߴ�ı�ʱ������ɫ
	DXGI_RGBA swapChainBackColor = {};
	m_swapChain->SetBackgroundColor(&swapChainBackColor);
}

void DeviceResources::SetTargetWindow(HWND hWnd)
{
	m_hWnd = hWnd;
	RECT clientRect;
	GetClientRect(m_hWnd, &clientRect);
	m_clientSize.width = static_cast<UINT>(clientRect.right);
	m_clientSize.height = static_cast<UINT>(clientRect.bottom);

	//���ڸı䣬���´�����Դ
	CreateWindowDependentResources();
}

void DeviceResources::Resize(D2D1_SIZE_U clientSize)
{
	if (m_clientSize.width == clientSize.width&&m_clientSize.height == clientSize.height)
	{
		//�ߴ���ȣ������ı�
		return;
	}
	if (clientSize.width == 0 || clientSize.height == 0)
	{
		//���Ϸ��Ľ������ߴ磬�����ı�
		return;
	}

	m_clientSize = clientSize;

	//���ڳߴ�ı䣬������������С
	CreateSizeDependentResources();
}
