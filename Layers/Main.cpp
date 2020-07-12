#include "Pch.h"
#include "Application.h"

class Example
{
public:
	int normal_1;
	const int const_1;
	static int static_1;
	static const int static_const_1;
public:
	Example(int t):
		const_1(t)
	{}
};

int Example::static_1 = 127;
const int Example::static_const_1 = 255;

static auto str = L"This_is_test_string_for_wchar_t";

int __stdcall wWinMain(HINSTANCE hInstance, HINSTANCE, LPWSTR lpCmdLine, int nCmdShow)
{
	CoInitialize(nullptr);
	return Application(1280,720,hInstance).Run(nCmdShow);
}
