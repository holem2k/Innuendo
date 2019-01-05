#include "stdafx.h"
#include "Win.h"
#include "assert!.h"
#include "resource.h"


LRESULT CALLBACK CWin::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	return DefWindowProc(hWnd, message, wParam, lParam);
}


BOOL CWin::Create(DWORD dwWidth, DWORD dwHeight, WNDPROC OutWndProc)
{

    wsprintf(lpszWndClassName, "_In%u", rand());
    lstrcpy(lpszWndClassName, "MainWClass");


    WNDCLASS WndClass;
    WndClass.lpszClassName = lpszWndClassName;
    WndClass.hIcon = 0;
    WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
    WndClass.lpszMenuName = NULL;
    if (OutWndProc != NULL) WndClass.lpfnWndProc = OutWndProc;
    else WndClass.lpfnWndProc = WndProc;
    WndClass.hbrBackground = NULL;
    WndClass.cbWndExtra = 0;
    WndClass.cbClsExtra = 0;
    WndClass.hInstance =  GetModuleHandle(NULL);
    WndClass.style = 0;
    int result = RegisterClass(&WndClass);
    assert2(result); //>>>
    if (!result) return FALSE;

	
	m_hWnd = CreateWindow(lpszWndClassName, "Курсовая работа", 
        (WS_OVERLAPPEDWINDOW | WS_VISIBLE) & (~ (WS_THICKFRAME | WS_MAXIMIZEBOX)),
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		dwWidth,
		dwHeight + GetSystemMetrics(SM_CYCAPTION),
		NULL,
		hMenu = LoadMenu(GetModuleHandle(NULL), MAKEINTRESOURCE(IDR_MENU1)),
		GetModuleHandle(NULL),
		NULL);
	assert2(m_hWnd); //>>>
    if (m_hWnd == NULL) return  FALSE;

    ShowWindow(m_hWnd, TRUE);
    UpdateWindow(m_hWnd);
	return TRUE;
}

////////////////////////////////////////////////////////////////////////

CWin::~CWin()
{
    UnregisterClass(lpszWndClassName, GetModuleHandle(NULL));
}

////////////////////////////////////////////////////////////////////////

BOOL CWin::BitBlt(int nXDest,int nYDest, int nWidth, int nHeight, 
        HDC hdcSrc,  int nXSrc, int nYSrc)
{
    HDC dc = GetDC();
    return ::BitBlt(dc, nXDest, nYDest, nWidth, nHeight,hdcSrc, nXSrc, nYSrc, SRCCOPY);
}


