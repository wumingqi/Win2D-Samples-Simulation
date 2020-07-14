#include "Pch.h"
#include "Application.h"
#include <tchar.h>

using namespace D2D1;

void Application::InitializeResources()
{
	m_deviceResources.SetTargetWindow(m_hWnd);
	m_d2dContext = m_deviceResources.GetD2DContext();

	//创建资源
	m_d2dContext->CreateSolidColorBrush(ColorF::ColorF(0xFFAA22), &m_brush);

	m_lock = CreateMutex(nullptr, FALSE, nullptr);
	m_RenderFanished = CreateEvent(nullptr, TRUE, FALSE, nullptr);

	m_balls = {
		{L"One",	D2D1::ColorF::Red},
		{L"Two",	D2D1::ColorF::Lime},
		{L"Three",	D2D1::ColorF::Blue},
		{L"Four",	D2D1::ColorF::Yellow},
		{L"Five",	D2D1::ColorF::Cyan},
		{L"Six",	D2D1::ColorF::Magenta},
		{L"Seven",	D2D1::ColorF::LightGray},
		{L"Eight",	D2D1::ColorF::DarkGray},
		{L"Nine",	D2D1::ColorF::Brown},
	};

	D2D1_GRADIENT_STOP stops[] = {
		{0, D2D1::ColorF(D2D1::ColorF::White)},
		{0.25f, D2D1::ColorF(D2D1::ColorF::White)},
		{1.0f, D2D1::ColorF(0, 0)},
	};

	ComPtr<ID2D1GradientStopCollection> stopCollection;
	m_d2dContext->CreateGradientStopCollection(stops, _countof(stops), &stopCollection);

	m_d2dContext->CreateRadialGradientBrush(
		D2D1::RadialGradientBrushProperties({}, {}, 0,0), 
		stopCollection.Get(), 
		&m_gradientBrush);

	CreateThread(nullptr, 0, RenderThread, this, 0, 0);
}

void Application::Update()
{
	m_d2dContext->BeginDraw();
	m_d2dContext->Clear(ColorF(D2D1::ColorF::CornflowerBlue));
	//开始绘制

	switch (m_fadeMode)
	{
	case FADE_MODE::PER_PRIMITIVE:
		DrawPerPrimitive();
		break;
	case FADE_MODE::OPACITY_BRUSH:
		DrawOpacityBrush();
		break;
	}

	m_d2dContext->EndDraw();
	m_deviceResources.GetSwapChain()->Present(1, 0);
}

void Application::DrawPerPrimitive()
{
	for (auto& ball : m_balls)
	{
		ball.Update({ (float)m_width, (float)m_height });

		// 绘制黑色阴影
		m_brush->SetColor(D2D1::ColorF(0x000000, ball.Color.a));
		m_d2dContext->FillEllipse(D2D1::Ellipse({ ball.Position.x + 4,ball.Position.y + 4 }, ball.Radius, ball.Radius), m_brush.Get());

		// 绘制X标志
		D2D1_POINT_2F p0, p1;
		p0.x = ball.Position.x - ball.Radius * 0.8f;
		p0.y = ball.Position.y - ball.Radius * 0.8f;
		p1.x = ball.Position.x + ball.Radius * 0.8f;
		p1.y = ball.Position.y + ball.Radius * 0.8f;
		m_brush->SetColor(D2D1::ColorF(0xFFFFFF, ball.Color.a));
		m_d2dContext->DrawLine(p0, p1, m_brush.Get(), 4.0f);
		p0.x = ball.Position.x - ball.Radius * 0.8f;
		p0.y = ball.Position.y + ball.Radius * 0.8f;
		p1.x = ball.Position.x + ball.Radius * 0.8f;
		p1.y = ball.Position.y - ball.Radius * 0.8f;
		m_d2dContext->DrawLine(p0, p1, m_brush.Get(), 4.0f);

		m_brush->SetColor(ball.Color);
		m_d2dContext->FillEllipse(D2D1::Ellipse({ ball.Position.x,ball.Position.y }, ball.Radius, ball.Radius), m_brush.Get());

		m_brush->SetColor(D2D1::ColorF(0xFFFFFF, ball.Color.a));
		D2D1_POINT_2F origin = { ball.Position.x - ball.Radius, ball.Position.y - ball.Radius };
		m_d2dContext->DrawTextLayout(origin, CreateText(ball.Text, ball.Radius * 2, ball.Radius * 2).Get(), m_brush.Get());
	}
}

void Application::DrawOpacityBrush()
{
	for (auto& ball : m_balls)
	{
		ball.Update({ (float)m_width, (float)m_height });
		m_gradientBrush->SetCenter({ ball.Position.x, ball.Position.y });
		m_gradientBrush->SetRadiusX(ball.Radius);
		m_gradientBrush->SetRadiusY(ball.Radius);

		m_d2dContext->FillEllipse(D2D1::Ellipse({ ball.Position.x,ball.Position.y }, ball.Radius, ball.Radius), m_gradientBrush.Get());
	}
}

ComPtr<IDWriteTextLayout> Application::CreateText(const std::wstring text, float width, float height)
{
	ComPtr<IDWriteTextLayout> layout;
	ComPtr<IDWriteTextFormat3> format;

	m_deviceResources.GetDwriteFactory()->CreateTextFormat(
		L"",
		nullptr,
		nullptr,
		0,
		32.f,
		L"zh-CN",
		&format);

	m_deviceResources.GetDwriteFactory()->CreateTextLayout(text.c_str(), text.length(),
		format.Get(),
		width, height,
		&layout);
	layout->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
	layout->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
	return layout;
}

DWORD __stdcall Application::RenderThread(LPVOID param)
{
	Application& app = *(Application*)param;
	while (app.m_CanRender)
	{
		LARGE_INTEGER frequency, begin, end;
		QueryPerformanceFrequency(&frequency);
		QueryPerformanceCounter(&begin);

		WaitForSingleObject(app.m_lock, INFINITE);
		app.Update();
		ReleaseMutex(app.m_lock);

		QueryPerformanceCounter(&end);

		double t = (end.QuadPart - begin.QuadPart) / (double)frequency.QuadPart;

		TCHAR buffer[128];
		_stprintf_s(buffer, L"当前帧率：%0.0lf fps", 1 / t);

		// 会导致死锁
		SetWindowText(app.m_hWnd, buffer);
	}

	SetEvent(app.m_RenderFanished);

	return 0;
}
