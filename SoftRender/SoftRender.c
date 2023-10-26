#include <Windows.h>
#include "f3d.h"

#define CLIENTWIDTH			1024
#define CLIENTHEIGHT		768
#define CLIENTTOP			0
#define CLIENTLEFT			0
#define COLORDEPTH			4
#define PI					3.14f

HBITMAP backHandle;
char* backBuffer;

BOOL done;


LRESULT CALLBACK winProc(HWND _hWnd, UINT _msg, WPARAM _wParam, LPARAM _lParam)
{
	switch (_msg)
	{
	case WM_QUIT:
	{
		done = TRUE;
		break;
	}

	case WM_CREATE:
	{
		HDC hdc = GetDC(_hWnd);
		BITMAPINFO bmi;
		RECT rect;
		GetClientRect(_hWnd, &rect);
		ZeroMemory(&bmi, sizeof(BITMAPINFO));
		bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
		bmi.bmiHeader.biWidth = rect.right;//ps.rcPaint.right;
		bmi.bmiHeader.biHeight = rect.bottom;//ps.rcPaint.bottom;
		bmi.bmiHeader.biPlanes = 1;
		bmi.bmiHeader.biBitCount = 32;
		bmi.bmiHeader.biCompression = BI_RGB;

		backHandle = CreateDIBSection(hdc, &bmi, DIB_RGB_COLORS, (void**)&backBuffer, NULL, 0);
		ReleaseDC(_hWnd, hdc);
		break;
	}

	case WM_CLOSE:
	{
		SendMessage(_hWnd, WM_QUIT, (WPARAM)EXIT_SUCCESS, (LPARAM)NULL);
		break;
	}

	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(_hWnd, &ps);
		HDC backDC = CreateCompatibleDC(hdc);
		HBITMAP oldBm = SelectObject(backDC, backHandle);

		//SetPixel(backDC, 100, 100, RGB(255,255,255));

		BitBlt(hdc, 0, 0, CLIENTWIDTH, CLIENTHEIGHT, backDC, 0, 0, SRCCOPY);

		SelectObject(backDC, oldBm);
		DeleteDC(backDC);
		EndPaint(_hWnd, &ps);
		break;
	}
	}
	return(DefWindowProc(_hWnd, _msg, _wParam, _lParam));
}

int WINAPI WinMain(HINSTANCE _hInstance, HINSTANCE _prevInstance, LPSTR lpCmdLine, int nCmdShow)
{

	done = FALSE;
	MSG msg;
	HWND hWnd = NULL;
	backBuffer = NULL;

	int time = GetTickCount();
	int delta = 0;
	int ftime = 0;

	WNDCLASSEX wc;
	ZeroMemory(&wc, sizeof(WNDCLASSEX));
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.hInstance = _hInstance;
	wc.lpszClassName = L"3dTestWnd";
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = (WNDPROC)winProc;

	RECT r;
	r.right = CLIENTWIDTH;
	r.bottom = CLIENTHEIGHT;
	r.left = CLIENTLEFT;
	r.top = CLIENTTOP;
	AdjustWindowRectEx(&r, WS_OVERLAPPEDWINDOW, FALSE, (DWORD)NULL);
	if (r.left < 0)
	{
		r.right -= r.left;
		r.left = 0;
	}
	if (r.top < 0)
	{
		r.bottom -= r.top;
		r.top = 0;
	}

	if (RegisterClassExW(&wc))
	{
		hWnd = CreateWindowEx((DWORD)NULL, L"3dTestWnd", L"Window", WS_OVERLAPPEDWINDOW, r.left, r.top, r.right, r.bottom, NULL, NULL, _hInstance, NULL);
	}
	if (!hWnd)
	{
		done = TRUE;
	}
	else
	{
		AnimateWindow(hWnd, 200, AW_BLEND | AW_SLIDE | AW_ACTIVATE);
	}

	//LPFTARGET back = initializeF3D(r.right, r.bottom, 4, backBuffer, 45);
	LPFTARGET back = initializeF3D(CLIENTWIDTH, CLIENTHEIGHT, 4, backBuffer, 45);

	while (!done)
	{
		if (PeekMessage(&msg, NULL, NULL, NULL, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		delta = GetTickCount() - time;
		time += delta;
		ftime += delta;
		draw(NULL, 10, back);
		if (ftime > 16)
		{
			InvalidateRect(hWnd, NULL, TRUE);
		}
	}
	destroyF3D();
	UnregisterClass(L"3dTestWnd", _hInstance);
	return(EXIT_SUCCESS);
}