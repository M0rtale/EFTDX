#include "OverlayWindow.h"
#include "Font.h"

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND _Hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK WndProc(HWND _Hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (ImGui_ImplWin32_WndProcHandler(_Hwnd, message, wParam, lParam))
		return true;


	switch (message)
	{
	case WM_SYSCOMMAND:
		if ((wParam & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
			return 0;
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(_Hwnd, message, wParam, lParam);
}

bool OverlayWindow::Init()
{
#ifdef VMP
	VMProtectBeginMutation("OverlayWindowInit");
#endif // NDEBUG

	WNDCLASSEXA wcex;
	ZeroMemory(&wcex, sizeof(WNDCLASSEX));
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = GetModuleHandle(NULL);
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)CreateSolidBrush(RGB(0, 0, 0));
	wcex.lpszClassName = "Rusty_Rust";
	if (!RegisterClassExA(&wcex))
	{
		return false;
	}

	_Hwnd = CreateWindowExA(WS_EX_TRANSPARENT | WS_EX_LAYERED, "Rusty_Rust", "Rusty_Rust", WS_CHILD | WS_POPUP,
		0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN),
		nullptr, nullptr, GetModuleHandle(NULL), nullptr);

	if (!_Hwnd)
	{
		int a = GetLastError(); 
		return false;
	}
	MARGINS temp_margins = { 0,0,GetSystemMetrics(SM_CXSCREEN),GetSystemMetrics(SM_CYSCREEN) };
	DwmExtendFrameIntoClientArea(_Hwnd, &temp_margins);
	SetLayeredWindowAttributes(_Hwnd, RGB(0, 0, 0), 0, LWA_COLORKEY);
	ShowWindow(_Hwnd, SW_SHOW);
	UpdateWindow(_Hwnd);
	_DX_Show->Init(_Hwnd);

	//这个g_fRubik我是从我H1内部挂抄来的，个人感觉还行
	//https://i.imgur.com/lGu3V1b.jpg
	//例子
	ImGuiIO& io = ImGui::GetIO();

	io.Fonts->AddFontFromMemoryTTF(g_fRubik, sizeof(g_fRubik), 16.0f, NULL, io.Fonts->GetGlyphRangesCyrillic());
	m_pDefault = io.Fonts->AddFontFromMemoryTTF(g_fRubik, sizeof(g_fRubik), 20.0f, NULL, io.Fonts->GetGlyphRangesCyrillic());
	// No chinese zone for now.
	//m_pChinese = io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\simhei.ttf", 20.f, NULL, io.Fonts->GetGlyphRangesChineseFull());

	m_pChinese = m_pDefault;

	//改字体

#ifdef VMP
	VMProtectEnd();
#endif // NDEBUG

	return true;
}