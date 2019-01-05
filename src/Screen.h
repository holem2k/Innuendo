#ifndef __SCREEN_H__
#define __SCREEN_H__
#include "stdafx.h"

typedef  WORD PIXEL;
////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////

class CScreen;
   class CDirectScreen; // "ddraw.h"
   class CDIBScreen;

////////////////////////////////////////////////////////////////////////
//                      class CScreen
////////////////////////////////////////////////////////////////////////

class CScreen
{
public:
    CScreen() {};
    virtual BOOL Create(DWORD dwWidth, DWORD dwHeight) = 0;
    virtual void *Lock() = 0;
    virtual void Unlock() = 0;
    virtual DWORD GetPitch()  = 0;
    virtual HDC GetDC() = 0;
    virtual void ReleaseDC(HDC dc) = 0;
    virtual void Clear() = 0;
protected:
    DWORD m_dwWidth;
    DWORD m_dwHeight;
};

////////////////////////////////////////////////////////////////////////
//                  class CDirectScreen 
////////////////////////////////////////////////////////////////////////

typedef struct _LockStruc
{
    RECT rect;
    BOOL bLocked;
} LockStruc;


class CDirectScreen : public CScreen
{
public:
    CDirectScreen();
    ~CDirectScreen();
    BOOL Create(DWORD dwWidth, DWORD dwHeight);

    void *Lock();
    void Unlock();

    DWORD GetPitch();

    HDC GetDC();
    void ReleaseDC(HDC dc);
    void Clear() {};
protected:
    LPDIRECTDRAW lpDD;
    LPDIRECTDRAWSURFACE lpDDS;
    DDSURFACEDESC locked_ddsd;
    void DestroyDDObjects();
    LockStruc lockstruc;
};


////////////////////////////////////////////////////////////////////////
//                       class CDIBScreen 
////////////////////////////////////////////////////////////////////////

class CDIBScreen : public CScreen
{
public:
    CDIBScreen();
    ~CDIBScreen();

    BOOL Create(DWORD dwWidth, DWORD dwHeight);
    void DestroyDIBObjects();

    void *Lock();
    void Unlock() {};

    DWORD GetPitch();

    HDC GetDC();
    void ReleaseDC(HDC dc);
    void Clear();
protected:
    HDC dc;
    HBITMAP bm;
    HBITMAP oldbm;
    BITMAPINFO bminfo;
    void *lpData;  // DIB section
    void *lpFirstLine;
    DWORD m_dwPitch;
};


////////////////////////////////////////////////////////////////////////

#endif //__SCREEN_H__