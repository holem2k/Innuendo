// Polygon.cpp: implementation of the CPolygon class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Polygon.h"
#include "assert!.h"
#include "Memoryman.h"
#include "Texture.h"
#include "stdio.h"
#include "Common.h"
#include "Light.h"

//////////////////////////////////////////////////////////////////////
//                class CPolygon
//////////////////////////////////////////////////////////////////////

int CPolygon::TextureKind;

//////////////////////////////////////////////////////////////////////

CPolygon::CPolygon()
{
    v_obj_num = 0;
    v_wld_num = 0;

    m_Texture = NULL;
    m_nTextureInfo = 0;

    // Default polygon is black without texture 
    static int color = 2;
    m_Color = RGB16(31, color, 10);
    color += 4;
    if (color > 32) color = 0;
    Colored(TRUE);

	TextureKind = 1;
}

//////////////////////////////////////////////////////////////////////

CPolygon::~CPolygon()
{
    // free "object space" vertexes
    for (int i = 0; i<v_obj_num; i++)
        deleteVertex(v_obj[i]);

    // free "world space" vertexes
    for (i = 0; i<v_wld_num; i++)
        deleteVertex(v_wld[i]);
}

//////////////////////////////////////////////////////////////////////
void CPolygon::Colored(BOOL bColored)
{
    m_bColored = bColored;
//    if (bColored) DrawFunc = DrawFlat;
//    else DrawFunc = DrawBilinearTextured;
}

//////////////////////////////////////////////////////////////////////

void CPolygon::Add(CVertex *v)
{
    assert2(v_obj_num<MAX_OBJECT_VERTEX_NUM);
    v->refs++;
    v_obj[v_obj_num++] = v;
}

//////////////////////////////////////////////////////////////////////

void CPolygon::Add(int n, CVertex *v, ...)
{
    va_list marker;
    va_start(marker, v);

    CVertex *tv = v;
    for (int i = 0; i<n; i++)
    {
        Add(tv);
        tv = va_arg(marker, CVertex *);
    }
    va_end(marker);
}

//////////////////////////////////////////////////////////////////////

void CPolygon::Face(bool bPositive)
{
    m_Face = (*v_obj[0] - *v_obj[1])&(*v_obj[2] - *v_obj[1]);
    if (!bPositive) m_Face = -m_Face;
}

//////////////////////////////////////////////////////////////////////

void CPolygon::Draw (void *pLine, int y, Span *span)
{
 //   (this->*DrawFunc)(pLine, y, span);
	if (TextureKind) DrawBilinearTextured(pLine, y, span);
	else DrawTextured(pLine, y, span);
}

//////////////////////////////////////////////////////////////////////

void CPolygon::DrawFlat(void *pLine, int y, Span *span)
{
    for (int i = span->xLeft; i<=span->xRight; i++)
        ((unsigned short *)pLine)[i] = m_Color;
}

//////////////////////////////////////////////////////////////////////

void CPolygon::DrawTextured(void *pLine, int y,  Span *span)
{
    TextureInfo *ti = &CTextureInfoCache::TIHeap[m_nTextureInfo];

    int step;
    int shln;

    int ruvValues[MAX_X/16*2 + 10];
    int i, n;
    real u, v;
    int xLeft, xRight, x, x_init;

    xLeft = span->xLeft;
    xRight = span->xRight;

    if (1)
    {
        x_init = xLeft&0xFFFFFFF0;
        n = ((xRight - x_init)>>4) + 1; 
        step = 16;
        shln = 4;
    }
    else
    {
        x_init = xLeft&0xFFFFFFE0;
        n = ((xRight - x_init)>>5) + 1; 
        step = 32;
        shln = 5;
    }

    real opt_v0 = ti->E*y + ti->F;
    real opt_v1;
    real opt_v2 = ti->Bu*y + ti->Cu;
    real opt_v3 = ti->Bv*y + ti->Cv;

    i = 0;
    x = x_init;

    for (int l = 0; l<=n; l++)
    {
        opt_v1 = 1.0f/(ti->D*x + opt_v0);

        u = (ti->Au*x + opt_v2)*opt_v1;
        ruvValues[i] = (int)u;

        v = (ti->Av*x + opt_v3)*opt_v1;
        ruvValues[i + 1] = (int)v;
        x += step;
        i += 2;
    }

    WORD Line[MAX_X];

    int iu, iv, diu,  div;

    l = 0;
    BYTE *pData = m_Texture->Data();
    WORD *pPalette = m_Texture->Palette();
    x = x_init;
    for (l = 0; l<i - 2; l += 2)
    {
        iu = ruvValues[l];
        iv = ruvValues[l + 1];
        diu = (ruvValues[l + 2] - iu)>>shln;
        div = (ruvValues[l + 3] - iv)>>shln;
        for (int t = 0; t < step; t++)
        {
            Line[x++] = pPalette[pData[((iu>>16)&0x7F) + ((iv>>9)&0x3F80)]];
            iu += diu;
            iv += div;
        }
    }

    for (i = xLeft; i<= xRight; i++)  
            ((WORD *)pLine)[i] = Line[i];

}

//////////////////////////////////////////////////////////////////////

void CPolygon::DrawBilinearTextured(void *pLine, int y,  Span *span)
{
    TextureInfo *ti = &CTextureInfoCache::TIHeap[m_nTextureInfo];

    int ruvValues[MAX_X/16*2 + 10];
    int i, n;
    real u, v;
    int xLeft, xRight, x, step, shln, x_init;

    xLeft = span->xLeft;
    xRight = span->xRight;
    if (span->bSloped)
    {
        x_init = xLeft&0xFFFFFFF0;
        n = ((xRight - x_init)>>4) + 1; //**
        step = 16;
        shln = 4;
    }
    else
    {
        x_init = xLeft&0xFFFFFFE0;
        n = ((xRight - x_init)>>5) + 1; //**
        step = 32;
        shln = 5;
    }

    real opt_v0 = ti->E*y + ti->F;
    real opt_v1;
    real opt_v2 = ti->Bu*y + ti->Cu;
    real opt_v3 = ti->Bv*y + ti->Cv;

    i = 0;
    x = x_init;

    for (int l = 0; l<=n; l++)
    {
        opt_v1 = 1/(ti->D*x + opt_v0);

        u = (ti->Au*x + opt_v2)*opt_v1;
        v = (ti->Av*x + opt_v3)*opt_v1;

        ruvValues[i] = (int)u;
        ruvValues[i + 1] = (int)v;
        x += step;
        i += 2;
    }

    WORD Line[MAX_X];
    l = 0;
    int iu, iv, diu,  div;


    BYTE *pData = m_Texture->Data();
    DWORD *pPalette = (DWORD *)m_Texture->BPalette();
    x = x_init;
    for (l = 0; l<i - 2; l += 2)
    {
        iu = ruvValues[l];
        iv = ruvValues[l + 1];
        diu = (ruvValues[l + 2] - iu)>>shln;
        div = (ruvValues[l + 3] - iv)>>shln;
        for (int t = 0; t < step; t++)
        {
            unsigned char frac = ((iu>>12)&0xF) + ((iv>>8)&0xF0);
            unsigned int Weights = ((unsigned int *)&CTexture::WeightMul[0])[frac];
            DWORD Color;
            unsigned int ofs = ((iu>>16)&0x7F) + ((iv>>9)&0x3F80);
  
            BYTE cxy = pData[ofs];
            Color = pPalette[(cxy<<4) + ((Weights&0xF000000)>>24)];
            BYTE c_xy = pData[ofs + 1];
            Color += pPalette[(c_xy<<4) + ((Weights&0xF00)>>8)];
            BYTE cx_y = pData[ofs + 128];
            Color += pPalette[(cx_y<<4) + ((Weights&0xF0000)>>16)];
            BYTE c_x_y = pData[ofs + 129];
            Color += pPalette[(c_x_y<<4) + (Weights&0xF)];
            WORD RealColor = (WORD)(((Color&0x3FF)>>4) + ((Color&0x7c000)>>9) + ((Color&0x3F000000)>>14));
            Line[x++] = RealColor;

            iu += diu;
            iv += div;
        }
    }

    for (i = xLeft; i<= xRight; i++)  
            ((WORD *)pLine)[i] = Line[i];
}

//////////////////////////////////////////////////////////////////////


void CPolygon::ClearWorldVertex()
{
    for (int i = 0; i<v_wld_num; i++) deleteVertex(v_wld[i]); 
    v_wld_num = 0;
};

//////////////////////////////////////////////////////////////////////

void CPolygon::ApplyTexture(CTexture *t, int vO, int vX, int vY, real xScale, real yScale)
{
    m_Texture = t;
    TexAppInfo.vO = vO;
    TexAppInfo.vX = vX;
    TexAppInfo.vY = vY;
    TexAppInfo.xScale = xScale;
    TexAppInfo.yScale = yScale;

//    DrawFunc = DrawBilinearTextured;
//    DrawFunc = DrawTextured;
}

//////////////////////////////////////////////////////////////////////
//                         class CObject
//////////////////////////////////////////////////////////////////////

CObject::CObject()
{
    m_pl = NULL;
    m_matrix = NULL;
}

//////////////////////////////////////////////////////////////////////

CObject::~CObject()
{
    delete m_pl; // delete LIST of poly's
    delete m_matrix;
}

//////////////////////////////////////////////////////////////////////

void CObject::Add(CPolygon *p)
{
    m_pl = new CPolygonList(p, m_pl);
}

//////////////////////////////////////////////////////////////////////

void CObject::Add(int n, CPolygon *p, ...)
{
    va_list marker;
    va_start(marker, p);

    CPolygon *tp = p;
    for (int i = 0; i<n; i++)
    {
        Add(tp);
        tp = va_arg(marker, CPolygon *);
    }
    va_end(marker);
}

//////////////////////////////////////////////////////////////////////

void CObject::LocateObjectInWorld()
{
    CPolygonList *pl = m_pl;
    while (pl)
    {
        CPolygon *p = pl->Polygon();
        p->ClearWorldVertex();
        for (int i = 0; i<p->v_obj_num; i++)
        {
            CVertex *v = p->v_obj[i];
            CVertex *cv = v->cacheVertex();
            if (cv)
            {
                /* vertex present in cache...*/
                p->v_wld[p->v_wld_num++] = cv;
                cv->refs++; // one more refs to this vertex
            }
            else
            {
                /* vertex not in the cache.. */
                if (m_matrix) // there is some transformation from obj. sp to world sp.
                {
                    CVertex *nv = newVertex();
                    // make transform
                    m_matrix->Transform(v, nv);
                    p->v_wld[p->v_wld_num++] = nv;
                    nv->refs++; // one more refs to this vertex
                    cacheAdd(v, nv);
                }
                else 
                {
                    // world. space == object space
                    p->v_wld[p->v_wld_num++] = v;
                    v->refs++; // one more refs to this vertex
                    // cacheAdd(v,v); // faster ?
                }
            }
        }
        pl = pl->Next();
    }
    cacheClear();
}

//////////////////////////////////////////////////////////////////////
//                         class CSector
//////////////////////////////////////////////////////////////////////

CSector::CSector()
{
    m_ol = NULL;
    m_enum_p = NULL;
    m_lights_num = 0;
}

//////////////////////////////////////////////////////////////////////

CSector::~CSector()
{
    delete m_ol; // delete LIST of objects
    for (int i = 0; i<m_lights_num; i++)
        delete m_lights[i]; // delete lights
}

//////////////////////////////////////////////////////////////////////

void CSector::Add(CObject *o)
{
    m_ol = new CObjectList(o, m_ol);
}

//////////////////////////////////////////////////////////////////////

void CSector::Add(int n, CObject *o, ...)
{
    va_list marker;
    va_start(marker, o);

    CObject *to = o;
    for (int i = 0; i<n; i++)
    {
        Add(to);
        to = va_arg(marker, CObject *);
    }
    va_end(marker);
}

//////////////////////////////////////////////////////////////////////

void CSector::AddLight(CLight *light)
{
    if (m_lights_num < MAX_LIGHTS)
    {
        m_lights[m_lights_num++] = light;
    }
}

//////////////////////////////////////////////////////////////////////
//                     class  CWorld
//////////////////////////////////////////////////////////////////////

CWorld::CWorld()
{
    m_sl = NULL;
    m_CurSector = NULL;
    m_tm = new CTextureManager;
}

//////////////////////////////////////////////////////////////////////

CWorld::~CWorld()
{
    delete m_sl;
    delete m_tm;
}

//////////////////////////////////////////////////////////////////////

void CWorld::Add(CSector *s)
{
    m_sl = new CSectorList(s, m_sl);
    m_CurSector = s;
};

//////////////////////////////////////////////////////////////////////

void CWorld::Add(int n, CSector *s, ...)
{
    va_list marker;
    va_start(marker, s);

    CSector *ts = s;
    for (int i = 0; i<n; i++)
    {
        Add(ts);
        ts = va_arg(marker, CSector *);
    }
    va_end(marker);

};
//////////////////////////////////////////////////////////////////////
