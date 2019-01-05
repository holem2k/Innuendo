#ifndef __CWIN_H__
#define __CWIN_H__


////////////////////////////////////////////////////////////////////////

class CWin;

////////////////////////////////////////////////////////////////////////
//         			class CWin
////////////////////////////////////////////////////////////////////////


class CWin
{
    HWND m_hWnd;
    char lpszWndClassName[10];
	HMENU hMenu;
public:
    static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
    CWin() { hMenu = NULL; };
    ~CWin();

    BOOL Create(DWORD dwWidth, DWORD dwHeight, WNDPROC OutWndProc);
    BOOL BitBlt(int nXDest,int nYDest, int nWidth, int nHeight, 
        HDC hdcSrc,  int nXSrc, int nYSrc);
    HDC GetDC() { return ::GetDC(m_hWnd); };
    void ReleaseDC(HDC dc) { ::ReleaseDC(m_hWnd, dc); };
	HMENU GetMenu() { assert(hMenu); return hMenu; };
	HWND GetHandle() { assert(m_hWnd); return m_hWnd; }
};



#endif //__CWIN_H__