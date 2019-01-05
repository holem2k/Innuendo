// Engine.cpp: implementation of the CEngine class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Engine.h"
#include "Light.h"
#include "math.h"

//////////////////////////////////////////////////////////////////////
//                    class CSimpleRender
//////////////////////////////////////////////////////////////////////


void CSimpleRender::Render(CView *view)
{
    PrepareRender(view);

    CSector *sector; CObject *obj; CPolygon *p; CPolygonList *pl;
    CSectorList *sl = m_World->SectorList();
    while (sl) // sectors
    {
        sector = sl->Sector();
        CObjectList *ol = sector->ObjectList();
        while (ol) // objects
        {
            obj = ol->Object();
            
            v_buf_num = 0;
            s_buf_num = 0;
            
            pl = obj->PolygonList();
            while (pl) // polygons
            {
                p = pl->Polygon();
                ProcessPolygon(p, view);
                pl = pl->Next(); 
            }
            cacheClear(); // for next object 
            ol = ol->Next();
        }
        sl = sl->Next();
    }
}

//////////////////////////////////////////////////////////////////////
//                    class CLightRender
//////////////////////////////////////////////////////////////////////

CLightRender::CLightRender(CWorld *world, CSpanBuffer *sbuffer)
{
    m_SpanBuffer = sbuffer;
    m_Rasterizer = new CRasterizer;
    m_Rasterizer->AttachSpanBuffer(sbuffer);
    m_World = world;
}

//////////////////////////////////////////////////////////////////////

CLightRender::~CLightRender()
{
    delete m_Rasterizer;
}

//////////////////////////////////////////////////////////////////////

void CLightRender::PrepareRender(CView *view)
{
    m_Rasterizer->SetClipSize(view->ScreenWidth(), view->ScreenHeight());
}

//////////////////////////////////////////////////////////////////////

void CLightRender::Rasterize(CPolygon *p, CVertex *s_vertexes[], CVertex *v_vertexes[], int v_num, CVector *normal)
{
	BOOL raster = FALSE;
	float denom = sqrt(normal->x*normal->x + normal->y*normal->y);
	if (denom != 0) 
	{
		if (atan2(normal->z, denom) < -M_PI/180*5) raster = TRUE;
	}
	else raster = TRUE;
    if (raster) m_Rasterizer->SpanPolygon(p, s_vertexes, v_num, normal);
}

//////////////////////////////////////////////////////////////////////
//                    class CRender
//////////////////////////////////////////////////////////////////////

CRender::CRender(CWorld *world, CSpanBuffer *sbuffer)
{
    m_SpanBuffer = sbuffer;
    m_Rasterizer = new CRasterizer;
    m_Rasterizer->AttachSpanBuffer(sbuffer);
    m_TextureInfoCache = new CTextureInfoCache;
    m_World = world;
}

//////////////////////////////////////////////////////////////////////

CRender::~CRender()
{
    delete m_Rasterizer;
    delete m_TextureInfoCache;
}

//////////////////////////////////////////////////////////////////////

void CRender::PrepareRender(CView *view)
{
    m_Rasterizer->SetClipSize(view->ScreenWidth(), view->ScreenHeight());
    m_TextureInfoCache->AttachView(view);
    m_TextureInfoCache->TIHeapSize = 0;
}

void CRender::Rasterize(CPolygon *p, CVertex *s_vertexes[], CVertex *v_vertexes[],  int v_num, CVector *normal)
{
    BOOL bSomeSpan = m_Rasterizer->SpanPolygon(p, s_vertexes, v_num, normal);
    if (bSomeSpan && p->Texture()) m_TextureInfoCache->AddTextureInfo(v_vertexes, p);
}

//////////////////////////////////////////////////////////////////////

void CSimpleRender::ProcessPolygon(CPolygon *p, CView *view)
{
    int v_poly_num;

    CVertex *v_poly[8];
    CVertex *s_poly[8];

    real zmin, zmax;
    int i;

    /// world -> view transformation
    v_poly_num = 0;
    for (i = 0; i<p->v_wld_num; i++)
    {
        CVertex *v, *cv;
        v = p->GetWVertex(i);
        cv = v->cacheVertex();
        if (cv)
        {
            // vertex cached...
            v_poly[v_poly_num++] = cv;
        }
        else
        {
            CVertex *ccv;
            view->WorldToView(v, ccv = &v_buf[v_buf_num++]);
            v_poly[v_poly_num++] = ccv;
            cacheAdd(v, ccv);
        }
    }
    /*** sort by z ***/
    zmin = zmax = v_poly[0]->z;
    for (i = 1; i<v_poly_num; i++)
    {
        real tempz = v_poly[i]->z;
        if (tempz > zmax) zmax = tempz;
        else  
            if (tempz < zmin) zmin = tempz;
    }
    /*** end sort by z ***/
    //real focus = view->Focus()*CLIP_VALUE_Z;
	real focus = CLIP_PLANE_Z;
    if (zmax > focus + EPS)
    {
        /* some part of polys is in front of you */
        if (zmin < focus - EPS)
        {
            /* clip poly in view space against focus plain */
            CVertex v_clip_buf[8];
            CVertex *v_poly_clip[8];
            // "allocate" space for clipped polygon
            int v_poly_clip_num;
            for (int i = 0; i < 8; i++) v_poly_clip[i] = &v_clip_buf[i];

            view->ClipPolygonAgainstFocus(v_poly, v_poly_num, v_poly_clip, v_poly_clip_num);

            // make view -> screen transformation
            for (i = 0; i<v_poly_clip_num; i++)
            {
                s_poly[i] = &s_buf[i];
                view->ViewToScreen(v_poly_clip[i], s_poly[i]);
            }
            CVector Face = (*s_poly[0] - *s_poly[1])&(*s_poly[2] - *s_poly[1]);
            // rasterize if visible
            if (Face.z < -EPS) Rasterize(p, s_poly, v_poly, v_poly_clip_num, &Face);

        }
        else
        {
            // make view -> screen transformation
            for (i = 0; i<v_poly_num; i++)
            {
                s_poly[i] = &s_buf[i];
                view->ViewToScreen(v_poly[i], s_poly[i]);
            }
            CVector Face = (*s_poly[0] - *s_poly[1])&(*s_poly[2] - *s_poly[1]);
            // rasterize if visible
            if (Face.z < -EPS) Rasterize(p, s_poly, v_poly, v_poly_num, &Face);
        }
    }
}

//////////////////////////////////////////////////////////////////////
//                    class CRenderMan
//////////////////////////////////////////////////////////////////////

CRenderMan::CRenderMan(CWorld *world, CView *view, CWin *win)
{
    m_View = view;
    m_Win = win;
    m_World = world;
    m_Screen = new CDIBScreen;
    m_Screen->Create(view->ScreenWidth(), view->ScreenHeight());
    
    m_MainSpanBuffer = new CSpanBuffer(view->ScreenHeight());
    m_MainRender = new CRender(world, m_MainSpanBuffer);
	m_MainLightSpanBuffer = new  CLightSpanBuffer();

    m_SpotLightSpanBuffer = new CSpanBuffer(PBUFFER_LEN);
    m_LightRender = new CLightRender(world, m_SpotLightSpanBuffer);
	m_pbuf = new CPBuffer(m_SpotLightSpanBuffer);

    m_SpotLighter = new CSpotLighter(m_MainSpanBuffer, m_View, m_SpotLightSpanBuffer, m_MainLightSpanBuffer);
}

//////////////////////////////////////////////////////////////////////

CRenderMan::~CRenderMan()
{
    delete m_Screen;
    // user view renderer
    delete m_MainSpanBuffer;
	delete m_MainRender;
	delete m_MainLightSpanBuffer;
    // light renderer
    delete m_SpotLightSpanBuffer;
    delete m_LightRender;
	delete m_pbuf;
    delete m_SpotLighter;
}

//////////////////////////////////////////////////////////////////////

void CRenderMan::RenderWorld()
{
    m_MainSpanBuffer->InitSpanBuffer();
    m_MainRender->Render(m_View);
    m_MainSpanBuffer->Draw(m_Screen);

    for (int i = 0; i<m_World->CurSector()->LightsNum(); i++)
    {
        CLight *Light = m_World->CurSector()->Light(i);
        m_SpotLightSpanBuffer->InitSpanBuffer();
        m_LightRender->Render(Light->View());
		m_pbuf->Refresh();
		m_MainLightSpanBuffer->InitLightSpanBuffer();
        m_SpotLighter->MakeSpotLight(Light);
		m_MainLightSpanBuffer->Draw(m_Screen, m_pbuf);
    }

}

//////////////////////////////////////////////////////////////////////

void CRenderMan::Draw()
{
    HDC sdc = m_Screen->GetDC();
    m_Win->BitBlt(0, 0, m_View->ScreenWidth(), m_View->ScreenHeight(), sdc, 0, 0);
    m_Screen->ReleaseDC(sdc);
}

//////////////////////////////////////////////////////////////////////
//                    class CSpotLighter
//////////////////////////////////////////////////////////////////////

CSpotLighter::CSpotLighter(CSpanBuffer *sbuf, CView *view, CSpanBuffer *lsbuf,
						   CLightSpanBuffer *LightSpanBuffer)
{
    m_sbuf = sbuf;
    m_lsbuf = lsbuf;
    m_view = view;
	m_LightSpanBuffer = LightSpanBuffer;
}

//////////////////////////////////////////////////////////////////////

CSpotLighter::~CSpotLighter()
{
//    delete m_pbuf;
}

//////////////////////////////////////////////////////////////////////

void CSpotLighter::MakeSpotLight(CLight *light)
{
    //for (int i = 0; i<320; i++) m_sbuf->Dump(i);
	//m_pbuf->Refresh();
    int numy = 0;
    Span  *span;
    while (m_sbuf->iterBegin(numy, span))
    {
        while (span)
        {
            CVector p1, p2;
            span->GetPoints(p1, p2, numy);
            //if (numy > 202 && numy < 204)
				m_LightSpanBuffer->AddLightSpan(numy, &p1, &p2, m_view, light, (int)span->Poly);
            span = span->Next;
        }
        numy++;
    }
}

