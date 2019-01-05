// Texture.h: interface for the CTexture class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TEXTURE_H__37741701_8816_11D4_8DD7_00C026EFFB81__INCLUDED_)
#define AFX_TEXTURE_H__37741701_8816_11D4_8DD7_00C026EFFB81__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "Common.h"

class CPolygon;
class CVertex;
class CView;

const int TEXTURE_SIZE = 128;

//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////

struct TextureApplyInfo
{
    int vO, vX, vY;
    real xScale, yScale;
    BOOL bCorrect;
};

//////////////////////////////////////////////////////////////////////

class CTexture  
{
public:
	CTexture();
	~CTexture();
    BOOL LoadFromFile(LPCTSTR lpFileName);
	BOOL CreateSpotTexture();
    BYTE *Data() { return (BYTE *)m_pData; };
    WORD *Palette() { return (WORD *)m_pPalette; }
    WORD *BPalette() const { return (WORD *)m_pBPalette; }
    DWORD Width() const { return m_dwWidth; }
    DWORD Height() const { return m_dwHeight; }
    static unsigned char WeightMul[256*4]; // x*y, (15 - x)*y, x*(15 - y), (15 - x)*(15 - y)
protected:
    void *m_pData;
    void *m_pPalette;
    void *m_pBPalette;

    DWORD m_dwWidth;
    DWORD m_dwHeight;
    void Clear();
    void InitWeightMul();
    void CreateBilinearPalette();
};


//////////////////////////////////////////////////////////////////////

class CTextureList 
{
    CTexture *m_Texture;
    CTextureList *m_Next;
public:
    CTextureList(CTexture *Texture, CTextureList *TextList) {m_Next = TextList; m_Texture = Texture;  };
    ~CTextureList() { delete m_Texture;  delete m_Next; };
    CTextureList *Next(CTextureList *pl) { m_Next  = pl; };
    CTextureList *Next() { return m_Next; };
    CTexture *Texture(CTexture *p) { m_Texture = p; };
    CTexture *Texture() { return m_Texture; };
};


//////////////////////////////////////////////////////////////////////

class CTextureManager 
{
    CTextureList *m_tl;
public:
    CTextureManager();
    ~CTextureManager(); 
    void Add(CTexture *t);
    void Add(int n, CTexture *t, ...);
	CTexture *LoadTexture(LPCTSTR lpTextureName);
};

//////////////////////////////////////////////////////////////////////
// Информация о текстуре для всего многоугольника
//

struct TextureInfo
{
    real Au, Bu, Cu;
    real Av, Bv, Cv;
    real D, E, F;
};


//////////////////////////////////////////////////////////////////////

class CTextureInfoCache
{
    CView *m_a_View;
public:
    CTextureInfoCache();
    ~CTextureInfoCache();
    static TextureInfo TIHeap[TI_HEAP_SIZE];
    static int TIHeapSize;

    void AddTextureInfo(CVertex *vv[], CPolygon *p);
    void AttachView(CView *view) { m_a_View = view; };
};
//////////////////////////////////////////////////////////////////////


#endif // !defined(AFX_TEXTURE_H__37741701_8816_11D4_8DD7_00C026EFFB81__INCLUDED_)
 