// Texture.cpp: implementation of the CTexture class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Texture.h"
#include "Common.h"
#include "Polygon.h"
#include "Vertex.h"
#include "View.h"
#include "assert.h"
#include "math.h"

//////////////////////////////////////////////////////////////////////
//                         class CTexture
//////////////////////////////////////////////////////////////////////

unsigned char CTexture::WeightMul[256*4];

CTexture::CTexture()
{
    m_pData = NULL;
    m_pPalette = NULL;
    m_pBPalette = NULL;
    InitWeightMul();
}

//////////////////////////////////////////////////////////////////////

CTexture::~CTexture()
{
    Clear();
}

//////////////////////////////////////////////////////////////////////

void CTexture::Clear()
{
    delete m_pData;
    delete m_pPalette;
    delete m_pBPalette;
    m_pData = NULL;
    m_pPalette = NULL;
    m_pBPalette = NULL;
}

//////////////////////////////////////////////////////////////////////

BOOL CTexture::LoadFromFile(LPCTSTR lpFileName)
{
    HANDLE hFile;
    hFile = CreateFile(lpFileName,
        GENERIC_READ,
        FILE_SHARE_READ,
        NULL, 
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        NULL);

    if (hFile == INVALID_HANDLE_VALUE)
    {
        return FALSE;
    }

    DWORD dwFileSize;

    dwFileSize = GetFileSize(hFile, NULL);
    if (dwFileSize < sizeof(BITMAPINFO))
    {
        CloseHandle(hFile);
        return FALSE;
    }
    

    HANDLE hFMObject;
    hFMObject = CreateFileMapping(hFile, NULL, PAGE_READONLY, 0, 0, NULL);
    if (hFMObject == NULL)
    {
        CloseHandle(hFile);
        return FALSE;
    }

    void *pFile;
    pFile = MapViewOfFile(hFMObject, FILE_MAP_READ, 0, 0, 0);
    if (pFile == NULL)
    {
        CloseHandle(hFMObject);
        CloseHandle(hFile);
        return FALSE;   
    }


    BOOL result = TRUE;
    __try
    {
        BITMAPFILEHEADER *bfh = (BITMAPFILEHEADER *)pFile;
        if (bfh->bfType == 'MB')
        {
            BITMAPINFOHEADER *bh = (BITMAPINFOHEADER *)((unsigned char *)pFile + sizeof(*bfh));
            if (bh->biBitCount == 8 &&
                bh->biCompression == BI_RGB &&
                bh->biPlanes == 1 &&
                bh->biHeight > 0 /*bottom-up bitmap */)
            {
                Clear();
                // set dimensions
                m_dwWidth = bh->biWidth;
                m_dwHeight = bh->biHeight;
                // colors used
                DWORD dwColorUsed = bh->biClrUsed;
                if (dwColorUsed == 0) dwColorUsed = 256;

                RGBQUAD *pColor = (RGBQUAD *)((unsigned char *)bh + bh->biSize);
                BYTE *pRawData = (BYTE *)pColor + sizeof(RGBQUAD)*dwColorUsed;

                // read palette
                m_pPalette =  new WORD[dwColorUsed];
                for (DWORD i = 0; i<dwColorUsed; i++)
                {
                    unsigned char r = pColor[i].rgbRed>>3;
                    unsigned char g = pColor[i].rgbGreen>>3;
                    unsigned char b = pColor[i].rgbBlue>>3;
                    ((WORD *)m_pPalette)[i] = RGB16(r, g, b);
                }
                CreateBilinearPalette();

                // read image (without line alignment to 4)
                m_pData = new BYTE[bh->biSizeImage + bh->biWidth + 2];
                DWORD dwWidthAligned = bh->biSizeImage/bh->biHeight; 
                
                BYTE *pSrcCurrentData = pRawData + bh->biSizeImage;
                BYTE *pDstCurrentData = (BYTE *)m_pData;
                for (i = 0; i<m_dwHeight; i++)
                {
                    pSrcCurrentData -= dwWidthAligned;
                    CopyMemory(pDstCurrentData, pSrcCurrentData, m_dwWidth);
                    pDstCurrentData += m_dwWidth;
                }
                // for texture filtering
                CopyMemory(pDstCurrentData, m_pData, m_dwWidth); 
               
            }
            else result = FALSE;

        }
        else result = FALSE;
    }
    __except ((GetExceptionCode() == EXCEPTION_ACCESS_VIOLATION ||
		GetExceptionCode() == EXCEPTION_IN_PAGE_ERROR ) ?
        EXCEPTION_EXECUTE_HANDLER : EXCEPTION_CONTINUE_SEARCH)
    {
        result = FALSE;
    }
    
    UnmapViewOfFile(pFile);
    CloseHandle(hFMObject);
    CloseHandle(hFile);
    return result;

}

//////////////////////////////////////////////////////////////////////

void CTexture::InitWeightMul()
{
    for (int i = 0; i<16; i++)
        for (int j = 0; j<16; j++)
        {
            int k = (i*16 + j)*4;
            int sum = 0;
            sum += WeightMul[k] = round((i*j)/15.0);
            sum += WeightMul[k + 1] = round((15 - i)*j/15.0);
            sum += WeightMul[k + 2] = round(i*(15 - j)/15.0);
            sum += WeightMul[k + 3] = round((15 - i)*(15 - j)/15.0);
            for (int l = 0; l>4; l++)
            {
                if (sum < 15)
                {
                    WeightMul[k + l] += 1; 
                    sum++;
                }
            }
        }
}

//////////////////////////////////////////////////////////////////////

void CTexture::CreateBilinearPalette()
{
    assert(m_pPalette);
    assert(!m_pBPalette);

    m_pBPalette = new unsigned char[256*16*4];
    for (int i = 0; i<256; i++)
    {
        WORD c = ((WORD *)m_pPalette)[i];
        unsigned char r = RGB16RED(c);
        unsigned char g = RGB16GREEN(c);
        unsigned char b = RGB16BLUE(c);
        for (int j = 0; j<16; j++)
        {
            int nr = r*j;
            int ng = g*j;
            int nb = b*j;
            if (j == 15)
            {
                if (nr>2) nr -=2;
                if (ng>2) ng -=2;
                if (nb>2) nb -=2;
            }
            ((DWORD *)m_pBPalette)[16*i + j] = (nr<<20) + (ng<<10) + nb;
        }
    }
}

//////////////////////////////////////////////////////////////////////

BOOL CTexture::CreateSpotTexture()
{
	Clear();
	m_pPalette =  new WORD[256];
	for (int i = 0; i<256; i++)
	{
		//unsigned char color = 31 - 31*sin(i/255.0*M_PI/2)*sin(i/255.0*M_PI/2);
		unsigned char color = 31 - 31*i/255.0;
		((WORD *)m_pPalette)[i] = RGB16(color, color, color);
	}
	((WORD *)m_pPalette)[255] = 0;
    m_pData = new BYTE[TEXTURE_SIZE*TEXTURE_SIZE + TEXTURE_SIZE  + 2];
	for (i = 0; i < TEXTURE_SIZE; i++)
		for (int j = 0; j < TEXTURE_SIZE; j++)
		{
			int dist = (int)sqrt((64 - i)*(64 - i) +(64 - j)*(64 - j));
			((BYTE *)m_pData)[i*TEXTURE_SIZE + j] = dist > 63 ? 255 : dist*2;
		}

	/*
	Clear();
	m_pPalette =  new WORD[256];
	for (int i=0; i<63; i++) 
	{	
		((WORD *)m_pPalette)[i] = RGB16(i/4, i/4, i/5);
		((WORD *)m_pPalette)[i + 64] = RGB16(0, i/2, 0);
		((WORD *)m_pPalette)[i + 128] = RGB16(0, 0, i/2);
		((WORD *)m_pPalette)[i + 128] = RGB16(i/2, i/2, i/2);
	}
	m_pData = new BYTE[TEXTURE_SIZE*TEXTURE_SIZE + TEXTURE_SIZE  + 2];
	for (int x=0; x<128; x++)
		for (int y=0; y<128; y++)
		{	
			float dist=(float)sqrt((x-64)*(x-64)+(y-64)*(y-64));
			if (dist<62)
			{
				float col = (float)(1 - sin(dist/40)*sin(dist/40));
				((BYTE *)m_pData)[y*TEXTURE_SIZE + x] = (BYTE)(60*col);
			}  else ((BYTE *)m_pData)[y*TEXTURE_SIZE + x] = 0;
		}
*/		
	return 1;
}

//////////////////////////////////////////////////////////////////////
//                         class CTextureManager
//////////////////////////////////////////////////////////////////////


CTextureManager::CTextureManager()
{
    m_tl = NULL;
}

//////////////////////////////////////////////////////////////////////

CTextureManager::~CTextureManager()
{
    delete m_tl;
}

//////////////////////////////////////////////////////////////////////

void CTextureManager::Add(CTexture *t)
{
    m_tl = new CTextureList(t, m_tl);
}

//////////////////////////////////////////////////////////////////////

CTexture *CTextureManager::LoadTexture(LPCTSTR lpTextureName)
{
    CTexture *t = new CTexture;
    if (t->LoadFromFile(lpTextureName))	Add(t);
	else
	{
		delete t;
		t = NULL;
	}
	return t;
}

//////////////////////////////////////////////////////////////////////

void CTextureManager::Add(int n, CTexture *t, ...)
{
    va_list marker;
    va_start(marker, t);

    CTexture *tv = t;
    for (int i = 0; i<n; i++)
    {
        Add(tv);
        tv = va_arg(marker, CTexture *);
    }
    va_end(marker);
};

//////////////////////////////////////////////////////////////////////
//                         class CTextureInfoCache
//////////////////////////////////////////////////////////////////////

TextureInfo CTextureInfoCache::TIHeap[TI_HEAP_SIZE];
int CTextureInfoCache::TIHeapSize;



CTextureInfoCache::CTextureInfoCache()
{
    TIHeapSize = 0;

    m_a_View = NULL;
}

//////////////////////////////////////////////////////////////////////

CTextureInfoCache::~CTextureInfoCache()
{

}

//////////////////////////////////////////////////////////////////////

void CTextureInfoCache::AddTextureInfo(CVertex *vv[], CPolygon *p)
{
    assert(m_a_View);

    TextureInfo *ti = &TIHeap[TIHeapSize];
    p->m_nTextureInfo = TIHeapSize;
    TIHeapSize++;

    real Ox, Oy, Oz;
    real Vx, Vy, Vz;
    real Ux, Uy, Uz;
    real f1, f2, opt_f1_f2_mul;
    int vO, vX, vY;
    real wTex, hTex;
    int dx, dy;

    f1 = m_a_View->opt_m_ScreenWidth_Width_m_Focus_div_mul;
    f2 = m_a_View->opt_m_ScreenHeight_Height_m_Focus_div_mul;
    dx = m_a_View->opt_m_ScreenWidth_2_div;
    dy = m_a_View->opt_m_ScreenHeight_2_div;

    opt_f1_f2_mul = f1*f2;

    wTex = p->Texture()->Width()/p->TexAppInfo.xScale*FLOAT_TO_FIX_MUL;
    hTex = p->Texture()->Height()/p->TexAppInfo.yScale*FLOAT_TO_FIX_MUL;

    vO = p->TexAppInfo.vO;
    Ox = vv[vO]->x;
    Oy = vv[vO]->y;
    Oz = vv[vO]->z;

    vX = p->TexAppInfo.vX;
    Ux = vv[vX]->x - Ox;
    Uy = vv[vX]->y - Oy;
    Uz = vv[vX]->z - Oz;

    vY = p->TexAppInfo.vY;
    Vx = vv[vY]->x - Ox;
    Vy = vv[vY]->y - Oy;
    Vz = vv[vY]->z - Oz;

	real a = (Ux*Vx + Uy*Vy + Uz*Vz)/(Ux*Ux + Uy*Uy + Uz*Uz);
	Vx = Vx - Ux*a;
	Vy = Vy - Uy*a;
	Vz = Vz - Uz*a;

    ti->Au = (Vz*Oy - Vy*Oz)*f2*wTex;
    ti->Bu = (Vx*Oz - Vz*Ox)*f1*wTex;
    ti->Cu = (Vy*Ox - Vx*Oy)*opt_f1_f2_mul*wTex - ti->Au*dx - ti->Bu*dy;

    ti->Av = (Uy*Oz - Uz*Oy)*f2*hTex;
    ti->Bv = (Uz*Ox - Ux*Oz)*f1*hTex;
    ti->Cv = (Ux*Oy - Uy*Ox)*opt_f1_f2_mul*hTex- ti->Av*dx - ti->Bv*dy;

    ti->D = (Vy*Uz - Vz*Uy)*f2;
    ti->E = (Vz*Ux - Vx*Uz)*f1;
    ti->F = (Vx*Uy - Vy*Ux)*opt_f1_f2_mul- ti->D*dx - ti->E*dy;

}
