#pragma once
class DeviceResources
{
private:
	const unsigned int										BufferCount = 3u;

	D3D_FEATURE_LEVEL										m_d3dFeatureLevel;
	Microsoft::WRL::ComPtr<ID3D11Device5>					m_d3dDevice;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext4>			m_d3dContext;

	Microsoft::WRL::ComPtr<IDXGIFactory6>					m_dxgiFactory;
	Microsoft::WRL::ComPtr<IDXGIDevice4>					m_dxgiDevice;
	Microsoft::WRL::ComPtr<IDXGISwapChain4>					m_swapChain;

	Microsoft::WRL::ComPtr<ID2D1Factory7>					m_d2dFactory;
	Microsoft::WRL::ComPtr<ID2D1Device6>					m_d2dDevice;
	Microsoft::WRL::ComPtr<ID2D1DeviceContext6>				m_d2dContext;
	Microsoft::WRL::ComPtr<ID2D1Bitmap1>					m_d2dTargetBitmap;

	Microsoft::WRL::ComPtr<IDWriteFactory7>					m_dwriteFactory;

	Microsoft::WRL::ComPtr<IWICImagingFactory2>				m_wicFactory;

	//´°¿ÚÐÅÏ¢
	HWND								m_hWnd;
	D2D1_SIZE_U							m_clientSize;

public:
	DeviceResources();
	~DeviceResources();

private:
	void CreateWindowDependentResources();
	void CreateSizeDependentResources();
	void SetRenderTargetBitmap();

public:
	void SetTargetWindow(HWND hWnd);
	void Resize(D2D1_SIZE_U clientSize);

public:
	auto GetD3DFeatureLevel()	const { return m_d3dFeatureLevel; }
	auto GetD3DDevice()			const { return m_d3dDevice.Get(); }
	auto GetD3DContext()		const { return m_d3dContext.Get(); }
	auto GetDxgiDevice()		const { return m_dxgiDevice.Get(); }
	auto GetDxgiFactory()		const { return m_dxgiFactory.Get(); }
	auto GetSwapChain()			const { return m_swapChain.Get(); }
	auto GetD2DFactory()		const { return m_d2dFactory.Get(); }
	auto GetD2DDevice()			const { return m_d2dDevice.Get(); }
	auto GetD2DContext()		const { return m_d2dContext.Get(); }
	auto GetDwriteFactory()		const { return m_dwriteFactory.Get(); }
	auto GetWicFactory()		const { return m_wicFactory.Get(); }
};

