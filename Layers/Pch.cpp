#include "Pch.h"

ComPtr<IWICFormatConverter> Helper::ConvertBitmapFormat(IWICImagingFactory* wicFactory, IWICBitmapDecoder* bitmapDecoder)
{
	ComPtr<IWICBitmapFrameDecode>	bitmapFrame;
	ComPtr<IWICFormatConverter>		formatConverter;
	bitmapDecoder->GetFrame(0, &bitmapFrame);

	wicFactory->CreateFormatConverter(&formatConverter);
	formatConverter->Initialize(
		bitmapFrame.Get(),
		GUID_WICPixelFormat32bppPRGBA,
		WICBitmapDitherTypeNone,
		nullptr,
		1.0,
		WICBitmapPaletteTypeCustom);
	return formatConverter;
}

ComPtr<ID2D1Bitmap> Helper::LoadBitmapSource(LPCTSTR filename, IWICImagingFactory* wicFactory, ID2D1RenderTarget* renderTarget)
{
	ComPtr<IWICBitmapDecoder>		bitmapDecoder;

	wicFactory->CreateDecoderFromFilename(
		filename,
		nullptr,
		GENERIC_READ,
		WICDecodeMetadataCacheOnDemand,
		&bitmapDecoder);

	ComPtr<ID2D1Bitmap> bitmap;
	renderTarget->CreateBitmapFromWicBitmap(
		ConvertBitmapFormat(wicFactory, bitmapDecoder.Get()).Get(),
		&bitmap);
	return bitmap;
}

ComPtr<ID2D1Bitmap> Helper::LoadBitmapSource(ULONG_PTR hFile, IWICImagingFactory* wicFactory, ID2D1RenderTarget* renderTarget)
{
	ComPtr<IWICBitmapDecoder>		bitmapDecoder;

	wicFactory->CreateDecoderFromFileHandle(
		hFile,
		nullptr,
		WICDecodeMetadataCacheOnDemand,
		&bitmapDecoder);

	ComPtr<ID2D1Bitmap> bitmap;
	renderTarget->CreateBitmapFromWicBitmap(
		ConvertBitmapFormat(wicFactory, bitmapDecoder.Get()).Get(),
		&bitmap);
	return bitmap;
}

ComPtr<ID2D1Bitmap> Helper::LoadBitmapSource(IStream* stream, IWICImagingFactory* wicFactory, ID2D1RenderTarget* renderTarget)
{
	ComPtr<IWICBitmapDecoder>		bitmapDecoder;

	wicFactory->CreateDecoderFromStream(
		stream,
		nullptr,
		WICDecodeMetadataCacheOnDemand,
		&bitmapDecoder);

	ComPtr<ID2D1Bitmap> bitmap;
	renderTarget->CreateBitmapFromWicBitmap(
		ConvertBitmapFormat(wicFactory, bitmapDecoder.Get()).Get(),
		&bitmap);
	return bitmap;
}

ComPtr<ID2D1Bitmap> Helper::LoadBitmapSource(HBITMAP hBitmap, IWICImagingFactory* wicFactory, ID2D1RenderTarget* renderTarget)
{
	ComPtr<IWICBitmap>		wicBitmap;
	wicFactory->CreateBitmapFromHBITMAP(
		hBitmap,
		nullptr, WICBitmapAlphaChannelOption::WICBitmapUsePremultipliedAlpha,
		&wicBitmap
	);

	ComPtr<ID2D1Bitmap> bitmap;
	renderTarget->CreateBitmapFromWicBitmap(
		wicBitmap.Get(),
		&bitmap);
	return bitmap;
}

ComPtr<ID2D1Bitmap> Helper::LoadBitmapSource(HICON hIcon, IWICImagingFactory* wicFactory, ID2D1RenderTarget* renderTarget)
{
	ComPtr<IWICBitmap>		wicBitmap;
	wicFactory->CreateBitmapFromHICON(
		hIcon,
		&wicBitmap
	);

	ComPtr<ID2D1Bitmap> bitmap;
	renderTarget->CreateBitmapFromWicBitmap(
		wicBitmap.Get(),
		&bitmap);
	return bitmap;
}