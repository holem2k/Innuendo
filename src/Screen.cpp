#include <stdafx.h>
#include "Screen.h"
#include "assert!.h"



////////////////////////////////////////////////////////////////////////
//                  class CDirectScreen 
////////////////////////////////////////////////////////////////////////


CDirectScreen::CDirectScreen()
{
    lpDD = NULL;
    lpDDS = NULL;
    lockstruc.bLocked = FALSE;
}


////////////////////////////////////////////////////////////////////////


CDirectScreen::~CDirectScreen()
{
    DestroyDDObjects();
}

////////////////////////////////////////////////////////////////////////

void CDirectScreen::DestroyDDObjects()
{
    if (lpDD != NULL)
    {
        if (lpDDS != NULL)
        {
            lpDDS->Release();
            lpDDS = NULL;
        }
        lpDD->Release();
        lpDD = NULL;
    }
}

////////////////////////////////////////////////////////////////////////

BOOL CDirectScreen::Create(DWORD dwWidth, DWORD dwHeight)
{
    HRESULT ddrval;
    ddrval = DirectDrawCreate(NULL,  &lpDD, NULL);
    if (ddrval != DD_OK)  return FALSE;

    ddrval = lpDD->SetCooperativeLevel(NULL, DDSCL_NORMAL);
    if (ddrval != DD_OK)
    {
        DestroyDDObjects();
        return FALSE;
    }

    DDSURFACEDESC ddsd;
    ZeroMemory(&ddsd, sizeof(ddsd));
    ddsd.dwSize = sizeof(ddsd);
    ddsd.dwFlags = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH;
    ddsd.ddsCaps.dwCaps = DDSCAPS_SYSTEMMEMORY | DDSCAPS_OFFSCREENPLAIN;
    ddsd.dwHeight = dwHeight;
    ddsd.dwWidth = dwWidth;


    //ZeroMemory(&ddsd.ddpfPixelFormat, sizeof(ddsd.ddpfPixelFormat.dwSize));
    ddsd.ddpfPixelFormat.dwSize = sizeof(ddsd.ddpfPixelFormat.dwSize);
    ddsd.ddpfPixelFormat.dwFlags = DDPF_RGB;
    ddsd.ddpfPixelFormat.dwRGBBitCount = 16;
    ddsd.ddpfPixelFormat.dwRBitMask = 0x00007C00 ;
    ddsd.ddpfPixelFormat.dwGBitMask = 0x000003E0;
    ddsd.ddpfPixelFormat.dwBBitMask = 0x0000001F;

    ddrval = lpDD->CreateSurface(&ddsd, &lpDDS, NULL);
    if (ddrval != DD_OK)
    {
        DestroyDDObjects();
        return FALSE;
    }

    m_dwWidth = dwWidth;
    m_dwHeight = dwHeight;
    return TRUE;
}

////////////////////////////////////////////////////////////////////////

void *CDirectScreen::Lock()
{
    assert2(!lockstruc.bLocked); 
   
    locked_ddsd.dwSize = sizeof(locked_ddsd);

    HRESULT ddrval;
    ddrval = lpDDS->Lock(NULL, &locked_ddsd, 0, NULL);
    if (ddrval != DD_OK)
    {
        assert2(0);
        return NULL;
    }

    lockstruc.bLocked = TRUE;

    return (void *)locked_ddsd.lpSurface;
}

////////////////////////////////////////////////////////////////////////

void CDirectScreen::Unlock()
{
    assert2(lockstruc.bLocked);
  
    if (lpDDS->Unlock(NULL) != DD_OK)   assert2(0);

    lockstruc.bLocked = FALSE;
}

////////////////////////////////////////////////////////////////////////

void  CDirectScreen::ReleaseDC(HDC dc)
{
    lpDDS->ReleaseDC(dc);
}

////////////////////////////////////////////////////////////////////////

HDC CDirectScreen::GetDC()
{
    HDC dc;
    HRESULT ddrval = lpDDS->GetDC(&dc);

    if (ddrval != DD_OK) return NULL;
    return dc;
}

////////////////////////////////////////////////////////////////////////

DWORD CDirectScreen::GetPitch()
{
    assert2(lockstruc.bLocked);
    return locked_ddsd.lPitch;
}

////////////////////////////////////////////////////////////////////////
//                       class CDIBScreen 
////////////////////////////////////////////////////////////////////////

CDIBScreen::CDIBScreen()
{
    lpData = NULL;
    dc = NULL;
    bm = NULL;
    lpFirstLine =  NULL;
    m_dwPitch = 0;
}

////////////////////////////////////////////////////////////////////////

CDIBScreen::~CDIBScreen()
{
    DestroyDIBObjects();
}

////////////////////////////////////////////////////////////////////////

BOOL CDIBScreen::Create(DWORD dwWidth, DWORD dwHeight)
{
    dc = CreateCompatibleDC(NULL);
    if (dc == NULL) return FALSE;

    ZeroMemory(&bminfo.bmiHeader, sizeof(bminfo.bmiHeader));
    bminfo.bmiHeader.biSize = sizeof(bminfo.bmiHeader);
    bminfo.bmiHeader.biPlanes = 1;
    bminfo.bmiHeader.biHeight = dwHeight;
    bminfo.bmiHeader.biWidth = dwWidth;
    bminfo.bmiHeader.biBitCount = 16;
    bminfo.bmiHeader.biCompression = BI_RGB;

    bm = CreateDIBSection(dc, &bminfo, DIB_RGB_COLORS, &lpData, NULL, 0);
    assert2(bm);
    if (bm == NULL)
    {
        DeleteDC(dc);
        return FALSE;
    }
    oldbm = (HBITMAP)SelectObject(dc, bm);

    m_dwWidth = bminfo.bmiHeader.biWidth;
    m_dwHeight = bminfo.bmiHeader.biHeight;


    // pitch
    int x = m_dwWidth & 3, r;

    if (x == 0) r = m_dwWidth;
    else r = m_dwWidth + (4 - x);

    if (m_dwHeight < 0) m_dwPitch  =  r; // top-down bitmap
    else m_dwPitch =  -r;
    m_dwPitch *= bminfo.bmiHeader.biBitCount >>3; // *bpp

    // first line
    if (bminfo.bmiHeader.biHeight < 0) lpFirstLine = lpData;
    else  lpFirstLine = (BYTE *)lpData + abs(GetPitch())*(abs(m_dwHeight) - 1);


    return TRUE;
}

////////////////////////////////////////////////////////////////////////

void CDIBScreen::DestroyDIBObjects()
{
    if (dc)
    {
        BOOL res;
        bm = (HBITMAP)SelectObject(dc, oldbm);
        res = DeleteObject(bm);
        bm = NULL;

        res = DeleteDC(dc);
        dc = NULL;
        lpData = NULL;
    }
}

////////////////////////////////////////////////////////////////////////

void *CDIBScreen::Lock()
{
    return lpFirstLine;
}

////////////////////////////////////////////////////////////////////////

DWORD CDIBScreen::GetPitch()
{
    return m_dwPitch;
}

////////////////////////////////////////////////////////////////////////

HDC CDIBScreen::GetDC()
{
    return dc;
}

////////////////////////////////////////////////////////////////////////

void CDIBScreen::ReleaseDC(HDC dc)
{
    // la la la
}

void CDIBScreen::Clear()
{
}


