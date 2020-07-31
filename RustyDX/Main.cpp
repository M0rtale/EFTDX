// Entry Point

#include "OverlayWindow.h"

OverlayWindow *_Overlay = nullptr;

static DWORD GetPIDForProcess(const char *Str)
{
	BOOL            working = 0;
	PROCESSENTRY32 lppe = { 0 };
	DWORD            targetPid = 0;

	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hSnapshot)
	{
		lppe.dwSize = sizeof(lppe);
		working = Process32First(hSnapshot, &lppe);
		while (working)
		{
			if (std::string(lppe.szExeFile) == Str)
			{
				targetPid = lppe.th32ProcessID;
				break;
			}
			working = Process32Next(hSnapshot, &lppe);
		}
	}

	CloseHandle(hSnapshot);
	return targetPid;
}

int main()
{
	getchar();
	printf(XorStr("HONK HONK Deploying\n"));

	_Overlay = new OverlayWindow();
	_Overlay->Init();

	MSG msg;
	GetMessage(&msg, nullptr, 0, 0);
	printf(XorStr("HONK HONK Deployed\n"));
	printf(XorStr("Don't act like a fucking dumbass\n"));
	printf(XorStr("Enjoy your party\n"));
	printf(XorStr("\n-------------------------------"));

	while (msg.message != WM_QUIT)
	{

		SetWindowPos(_Overlay->_Hwnd, HWND_TOPMOST, 0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN), SWP_SHOWWINDOW);
		if (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
		{

			TranslateMessage(&msg);
			DispatchMessage(&msg);
			continue;
		}
		_Overlay->_DX_Show->Render();

		if (GetPIDForProcess(XorStr("EscapeFromTarkov.exe")) == 0)
			break;

		if (!FindWindowA("Rusty_Rust", NULL)) // 如果透明窗口被关闭了
			break;

		Sleep(10);
	}
}