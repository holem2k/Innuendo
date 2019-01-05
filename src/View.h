// View.h: interface for the CView class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_VIEW_H__7D03E8E7_7169_11D4_8DD4_00C026EFFB81__INCLUDED_)
#define AFX_VIEW_H__7D03E8E7_7169_11D4_8DD4_00C026EFFB81__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "Common.h"
#include "Vertex.h"
#include "Polygon.h"


//#define CLIP_VALUE_Z 1
const int CLIP_PLANE_Z = 20;

class CView  
{

public:
	CView();
	~CView();
    void SetOrigin(real x, real y, real z);
    void SetOrigin(CVector &v);
    void SetFrustum(real focus, real Width, real Height);
    void SetAngles(real alfa, real beta, real vega);
    void SetScreenSize(int ScreenWidth, int ScreenHeight);
    void WorldToView(CVector *v, CVector *cv)
    {
        CVector t;
        t = *v - vOrigin;
        cv->x = mx1*t.x + my1*t.y + mz1*t.z;
        cv->y = mx2*t.x + my2*t.y + mz2*t.z;
        cv->z = mx3*t.x + my3*t.y + mz3*t.z;
    }
    void ViewToWorldVector(CVector *v, CVector *cv)
    {
        CVector t;
        t = *v;
        cv->x = mx1*t.x + mx2*t.y + mx3*t.z;
        cv->y = my1*t.x + my2*t.y + my3*t.z;
        cv->z = mz1*t.x + mz2*t.y + mz3*t.z;
    }
    void ViewToScreen(CVector *v, CVector *cv)
    {
        real opt_one_z_div = 1.0f/(v->z);
        cv->z = v->z;
        cv->x = opt_m_ScreenWidth_2_div + v->x*opt_one_z_div*opt_m_ScreenWidth_Width_m_Focus_div_mul;
        cv->y = opt_m_ScreenHeight_2_div + v->y*opt_one_z_div*opt_m_ScreenHeight_Height_m_Focus_div_mul;
    }
    void ScreenToWorld(CVector *v, CVector *cv)
    {
        CVector t;
        t.z = v->z;
        t.x = v->z*(v->x - opt_m_ScreenWidth_2_div)/opt_m_ScreenWidth_Width_m_Focus_div_mul;
        t.y = v->z*(v->y - opt_m_ScreenHeight_2_div)/opt_m_ScreenHeight_Height_m_Focus_div_mul;
        cv->x = mx1*t.x + mx2*t.y + mx3*t.z;
        cv->y = my1*t.x + my2*t.y + my3*t.z;
        cv->z = mz1*t.x + mz2*t.y + mz3*t.z;
        *cv = *cv + vOrigin;
    }
	void ScreenToScreen(CView *view, CVector *s, CVector *d)
	{
			CVector t;
            ScreenToWorld(s, &t);
            view->WorldToView(&t, &t);
            view->ViewToScreen(&t, d);
	}
    BOOL ClipPolygonAgainstFrustum(CPolygon *p);
    void ClipPolygonAgainstFocus(CVertex *vv[], int vv_num, CVertex *vvc[], int &vvc_num);
    int ScreenWidth() const{ return m_ScreenWidth; }
    int ScreenHeight() const { return m_ScreenHeight; }
    real Focus() const { return m_Focus;}

protected:
    void CalcOptValues();
    // world -> view trans. coefs
    CVector vOrigin;
    CVector vLookDir;
    real alfa;
    real beta;
    real vega;

    real mx1, my1, mz1;
    real mx2, my2, mz2;
    real mx3, my3, mz3;
    
    real Width, Height, m_Focus;
    int m_ScreenWidth, m_ScreenHeight;
    // opt
public:
    int opt_m_ScreenWidth_2_div,  opt_m_ScreenHeight_2_div;
    real opt_m_ScreenWidth_Width_m_Focus_div_mul;
    real opt_m_ScreenHeight_Height_m_Focus_div_mul;
};

#endif // !defined(AFX_VIEW_H__7D03E8E7_7169_11D4_8DD4_00C026EFFB81__INCLUDED_)
