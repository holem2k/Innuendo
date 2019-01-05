// Engine.h: interface for the CEngine class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ENGINE_H__4134E561_7D23_11D4_8DD4_00C026EFFB81__INCLUDED_)
#define AFX_ENGINE_H__4134E561_7D23_11D4_8DD4_00C026EFFB81__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "View.h"
#include "SpanBuffer.h"
#include "LightSpanBuffer.h"
#include "Rasterizer.h"
#include "Win.h"
#include "Screen.h"
#include "Cacher.h"
#include "PBuffer.h"

//////////////////////////////////////////////////////////////////////
//   
//////////////////////////////////////////////////////////////////////

class CSimpleRender: public CVertexCacher
{
public:
    void Render(CView *view);
protected:
    void ProcessPolygon(CPolygon *p,  CView *view);
    virtual void PrepareRender(CView *view) = 0;
    virtual void Rasterize(CPolygon *p, CVertex *s_vertexes[], CVertex *v_vertexes[], int v_num, CVector *normal) = 0;

    CRasterizer *m_Rasterizer;
    CWorld *m_World;
    CSpanBuffer *m_SpanBuffer;

    int v_buf_num;
    CVertex v_buf[100];
    int s_buf_num;
    CVertex s_buf[100];
};

//////////////////////////////////////////////////////////////////////
//   
//////////////////////////////////////////////////////////////////////


class CRender: public CSimpleRender
{
public:
	CRender(CWorld *world, CSpanBuffer *sbuffer);
	virtual ~CRender();
protected:
    CTextureInfoCache *m_TextureInfoCache;
    virtual void PrepareRender(CView *view);
    virtual void Rasterize(CPolygon *p, CVertex *s_vertexes[], CVertex *v_vertexes[],  int v_num, CVector *normal);
private:
    CRender(CRender &);
};

//////////////////////////////////////////////////////////////////////
//   
//////////////////////////////////////////////////////////////////////


class CLightRender: public CSimpleRender
{
public:
	CLightRender(CWorld *world, CSpanBuffer *sbuffer);
	virtual ~CLightRender();
protected:
    virtual void PrepareRender(CView *view);
    virtual void Rasterize(CPolygon *p, CVertex *s_vertexes[], CVertex *v_vertexes[],  int v_num, CVector *normal);
private:
    CLightRender(CLightRender &);
};

//////////////////////////////////////////////////////////////////////
//   
//////////////////////////////////////////////////////////////////////


class CSpotLighter
{
public:
    CSpotLighter(CSpanBuffer *sbuf, CView *view, CSpanBuffer *lsbuf, 
		CLightSpanBuffer *LightSpanBuffer);
    ~CSpotLighter();
    void MakeSpotLight(CLight *light);
protected:
    CSpanBuffer *m_sbuf;
    CSpanBuffer *m_lsbuf;
    CView *m_view;
	CLightSpanBuffer *m_LightSpanBuffer;
//    CPBuffer *m_pbuf;
private:
    CSpotLighter(const CSpotLighter &);
    void operator=(const CSpotLighter &);
};

//////////////////////////////////////////////////////////////////////
//   
//////////////////////////////////////////////////////////////////////


class CRenderMan
{
public:
    CRenderMan(CWorld *world, CView *view, CWin *win);
    ~CRenderMan();
    void RenderWorld();
    void Draw();
protected:
	CLightSpanBuffer *m_MainLightSpanBuffer;
    CSpanBuffer *m_MainSpanBuffer;
    CRender *m_MainRender;

    CSpanBuffer *m_SpotLightSpanBuffer;
    CLightRender *m_LightRender;
	CPBuffer *m_pbuf;

    CSpotLighter *m_SpotLighter;

    CWin *m_Win;
    CView *m_View;
    CWorld *m_World;

    CScreen *m_Screen;
private:
    CRenderMan(const CRenderMan &) {};
	void operator = (const CRenderMan &) {};

};


#endif // !defined(AFX_ENGINE_H__4134E561_7D23_11D4_8DD4_00C026EFFB81__INCLUDED_)
