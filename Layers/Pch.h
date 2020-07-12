#ifndef PCH_H
#define PCH_H

#include <d2d1_3.h>
#include <dxgi1_6.h>
#include <d3d11_4.h>
#include <dwrite_3.h>
#include <wincodec.h>
#include <wrl.h>

#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dwrite.lib")
#pragma comment(lib, "windowscodecs.lib")

#include <string>
#include <memory>

using namespace Microsoft::WRL;

struct Helper
{
private:
	static ComPtr<IWICFormatConverter> ConvertBitmapFormat(
		IWICImagingFactory* wicFactory,
		IWICBitmapDecoder* bitmapDecoder
	);
public:
	/********************************************************************************
	*					加载位图的函数。ID2D1Bitmap接口用来显示图片。					*
	********************************************************************************/
	static ComPtr<ID2D1Bitmap> LoadBitmapSource(
		LPCTSTR filename,
		IWICImagingFactory* wicFactory,
		ID2D1RenderTarget* renderTarget
	);
	static ComPtr<ID2D1Bitmap> LoadBitmapSource(
		ULONG_PTR hFile,
		IWICImagingFactory* wicFactory,
		ID2D1RenderTarget* renderTarget
	);
	static ComPtr<ID2D1Bitmap> LoadBitmapSource(
		IStream* stream,
		IWICImagingFactory* wicFactory,
		ID2D1RenderTarget* renderTarget
	);
	static ComPtr<ID2D1Bitmap> LoadBitmapSource(
		HBITMAP hBitmap,
		IWICImagingFactory* wicFactory,
		ID2D1RenderTarget* renderTarget
	);
	static ComPtr<ID2D1Bitmap> LoadBitmapSource(
		HICON hIcon,
		IWICImagingFactory* wicFactory,
		ID2D1RenderTarget* renderTarget
	);
};

#endif // !PCH_H
