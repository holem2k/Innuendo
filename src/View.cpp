// View.cpp: implementation of the CView class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "View.h"
#include "math.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CView::CView()
{
    // for opt
    Width = 1;
    Height = 1;
}

//////////////////////////////////////////////////////////////////////

CView::~CView()
{

}

//////////////////////////////////////////////////////////////////////

void CView::SetOrigin(real x, real y, real z)
{
    vOrigin.x = x;
    vOrigin.y = y;
    vOrigin.z = z;
}

//////////////////////////////////////////////////////////////////////

void CView::SetOrigin(CVector &v)
{
    vOrigin = v;
}

//////////////////////////////////////////////////////////////////////
void CView::SetFrustum(real focus, real width, real height)
{
    m_Focus = focus;
    Width = width;
    Height = height;
    CalcOptValues();
}

//////////////////////////////////////////////////////////////////////

void CView::SetAngles(real _alfa, real _beta, real _vega)
{
/*
    alfa = _alfa;
    beta = _beta;
    vega = _vega;

    // transformation sequence:
    // rotate -> X  alfa
    // rotate -> Y  beta
    // rotate -> Z  vega
    // rotate is clockwise if looking from end of corresponding axis

    real sin_alfa = (real)sin(alfa);
    real cos_alfa = (real)cos(alfa);

    real sin_beta = (real)sin(beta);
    real cos_beta = (real)cos(beta);

    real sin_vega = (real)sin(vega);
    real cos_vega = (real)cos(vega);

    mx1 = cos_vega*cos_beta;
    my1 = cos_vega*sin_beta*sin_alfa - sin_vega*cos_alfa;
    mz1 = cos_vega*sin_beta*cos_alfa + sin_vega*sin_alfa;

    

    mx2 = sin_vega*cos_beta;
    my2 = sin_vega*sin_beta*sin_alfa + cos_vega*cos_alfa;
    mz2 = sin_vega*sin_beta*cos_alfa - cos_vega*sin_alfa;

    mx3 = -sin_beta;
    my3 = cos_beta*sin_alfa;
    mz3 = cos_beta*cos_alfa;
*/

    real x = alfa =  _alfa;
    real y = beta =_beta;
    real z = vega = _vega;

    // transformation sequence:
    // rotate -> X  alfa
    // rotate -> Y  beta
    // rotate -> Z  vega
    // rotate is clockwise if looking from end of corresponding axis

    real sin_x = (real)sin(x);
    real cos_x = (real)cos(x);

    real sin_y = (real)sin(y);
    real cos_y = (real)cos(y);

    real sin_z = (real)sin(z);
    real cos_z = (real)cos(z);

    mx1 = cos_y*cos_z - sin_y*cos_x*sin_z;
    my1 = -cos_y*sin_z - sin_y*cos_x*cos_z;
    mz1 = sin_y*sin_x ;

    mx2 = sin_y*cos_z + cos_y*cos_x*sin_z;
    my2 = -sin_y*sin_z + cos_y*cos_x*cos_z;
    mz2 = -cos_y*sin_x;

    mx3 = sin_x*sin_z;
    my3 = sin_x*cos_z;
    mz3 = cos_x;

    // set look direction
    ViewToWorldVector(&CVector(0, 0, 1), &vLookDir);

}

//////////////////////////////////////////////////////////////////////

void CView::SetScreenSize(int ScreenWidth, int ScreenHeight)
{
    m_ScreenWidth = ScreenWidth;
    m_ScreenHeight = ScreenHeight;
    CalcOptValues();
}

//////////////////////////////////////////////////////////////////////
//            -.    
//          /  |  ^ 
//        /    |  |height
//       *----------------> z
//     ->|focus|<-            
//
    

BOOL CView::ClipPolygonAgainstFrustum(CPolygon *p)
{
/*    /////// Clip against frustum
    real A;
    int signs[MAX_VIEW_VERTEX_NUM];

    int vv_num;
    CVertex vv[MAX_VIEW_VERTEX_NUM];

    int vClip_num;
    CVertex vClip[MAX_VIEW_VERTEX_NUM];

    real a, b, c;
    int i, j;

    // copy poly's vertexes in local buffer
    vv_num = p->vv_num;
    for (i = 0; i<vv_num; i++) vv[i] = p->vv[i];

    // Clip in ZoX
    A = m_Focus/Width*2;
    for (i = 0; i<2; i++, A = -A)
    {
        int inside = 1;
        int outside = 1;
        for (j = 0; j<vv_num; j++)
        {
            // inside  1, outside -1
            real sign = vv[j].z - A*vv[j].x;
            if (sign < EPS && sign> -EPS)  signs[j] = 0;
            else
            if (sign > 0)
            {
                signs[j] = 1;
                outside = 0;
            }
            else
            {
                signs[j] = -1; 
                inside = 0;
            }
        }
        if (inside) continue;
        if (outside)
        {
            p->vv_num = 0;
            return FALSE;
        }

        /////////////////////////////////
        //  clip poly
        vClip_num = 0;
        int prevsign = signs[0];
        for (j = 0; j<vv_num + 1; j++)
        {
            int jj = j%vv_num;
            if (signs[jj] == 1)
            {
                // 1  1 |  или  |0  1 |
                if (prevsign != -1) { if (j != vv_num) vClip[vClip_num ++] = vv[jj];} // point inside frustum
                // -1 | 1
                else
                {
                    // find intersection
                    int prevj = j - 1;
                    a = vv[jj].x - vv[prevj].x;
                    b = vv[jj].z - vv[prevj].z;
                    c = vv[jj].y - vv[prevj].y;

                    real x, y, z;
                    x = (b*vv[prevj].x - a*vv[prevj].z)/(b - a*A);
                    z = A*x;
                    if (fabs(a) < EPS) y = c*(z - vv[prevj].z)/b + vv[prevj].y;
                    else y = c*(x - vv[prevj].x)/a + vv[prevj].y;

                    vClip[vClip_num++] = CVertex(x, y, z);
                    if (j != vv_num) vClip[vClip_num++] = vv[jj];
                }
            }
            else
            if (signs[jj] == -1)
            {
                // 1 | -1
                if (prevsign == 1)
                {
                    // find intersection
                    int prevj = j - 1;
                    a = vv[jj].x - vv[prevj].x;
                    b = vv[jj].z - vv[prevj].z;
                    c = vv[jj].y - vv[prevj].y;

                    real x, y, z;
                    x = (b*vv[prevj].x - a*vv[prevj].z)/(b - a*A);
                    z = A*x;
                    if (fabs(a) < EPS) y = c*(z - vv[prevj].z)/b + vv[prevj].y;
                    else y = c*(x - vv[prevj].x)/a + vv[prevj].y;

                    vClip[vClip_num++] = CVertex(x, y, z);
                }
            }
            else // 0
            {
                if (j != vv_num && (signs[(j + 1)%vv_num] == 1 ||
                    signs[(j + vv_num - 1)%vv_num] == 1))
                {
                    vClip[vClip_num ++] = vv[jj];
                }
            }
            prevsign = signs[jj];
        } // for (j

        vv_num = vClip_num;
        if (vv_num == 0) 
        {
            p->vv_num = 0;
            return FALSE;
        }
        for (j = 0; j<vClip_num; j++) vv[j] = vClip[j];
    }


    // Clip in ZoY
    A = m_Focus/Height*2;
    for (i = 0; i<2; i++, A = -A)
    {
        int inside = 1;
        int outside = 1;
        for (j = 0; j<vv_num; j++)
        {
            // inside  1, outside -1
            real sign = vv[j].z - A*vv[j].y;
            if (sign < EPS && sign> -EPS)  signs[j] = 0;
            else
            if (sign > 0)
            {
                signs[j] = 1;
                outside = 0;
            }
            else
            {
                signs[j] = -1; 
                inside = 0;
            }
        }
        if (inside) continue;
        if (outside)
        {
            p->vv_num = 0;
            return FALSE;
        }

        /////////////////////////////////
        //  clip poly
        vClip_num = 0;
        int prevsign = signs[0];
        for (j = 0; j<vv_num + 1; j++)
        {
            int jj = j%vv_num;
            if (signs[jj] == 1)
            {
                // 1  1 |  или  |0  1 |
                if (prevsign != -1) { if (j != vv_num) vClip[vClip_num ++] = vv[jj];} // point inside frustum
                // -1 | 1
                else
                {
                    // find intersection
                    int prevj = j - 1;
                    a = vv[jj].y - vv[prevj].y;
                    b = vv[jj].z - vv[prevj].z;
                    c = vv[jj].x - vv[prevj].x;

                    real x, y, z;
                    y = (b*vv[prevj].y - a*vv[prevj].z)/(b - a*A);
                    z = A*y;
                    if (fabs(a) < EPS) x = c*(z - vv[prevj].z)/b + vv[prevj].x;
                    else x = c*(y - vv[prevj].x)/a + vv[prevj].x;

                    vClip[vClip_num++] = CVertex(x, y, z);
                    if (j != vv_num) vClip[vClip_num++] = vv[jj];
                }
            }
            else
            if (signs[jj] == -1)
            {
                // 1 | -1
                if (prevsign == 1)
                {
                    // find intersection
                    int prevj = j - 1;
                    a = vv[jj].y - vv[prevj].y;
                    b = vv[jj].z - vv[prevj].z;
                    c = vv[jj].x - vv[prevj].x;

                    real x, y, z;
                    y = (b*vv[prevj].y - a*vv[prevj].z)/(b - a*A);
                    z = A*y;
                    if (fabs(a) < EPS) x = c*(z - vv[prevj].z)/b + vv[prevj].x;
                    else x = c*(y - vv[prevj].y)/a + vv[prevj].x;

                    vClip[vClip_num++] = CVertex(x, y, z);
                }
            }
            else // 0
            {
                if (j != vv_num && (signs[(j + 1)%vv_num] == 1 ||
                    signs[(j + vv_num - 1)%vv_num] == 1))
                {
                    vClip[vClip_num ++] = vv[jj];
                }
            }
            prevsign = signs[jj];
        } // for (j

        vv_num = vClip_num;
        if (vv_num == 0) 
        {
            p->vv_num = 0;
            return FALSE;
        }
        for (j = 0; j<vClip_num; j++) vv[j] = vClip[j];
    }


    for (j = 0; j<vv_num; j++) p->vv[j] = vv[j];
    return TRUE;
    */
return FALSE;
}

//////////////////////////////////////////////////////////////////////
// Function name	: CView::ClipPolygonAgainstFocus
// Description	    : clip polygon against focus plain
// Return type		: void 
// Argument         : [in] CVertex *vv[]   - polygon vertexes
// Argument         : [in] int vv_num      - number of vertexes
// Argument         : [out] CVertex *vvc[] - vertex of clipped polygon(MEMORY MUST BE ALLOCATED BEFORE USE THIS FUNC)
// Argument         : [out] int &vvc_num   - number of clipped vertexes

void CView::ClipPolygonAgainstFocus(CVertex *vv[], int vv_num, CVertex *vvc[], int &vvc_num)
{
    int signs[MAX_OBJECT_VERTEX_NUM];
    real a, b, c;
    int j;

    vvc_num = 0;
    //real ClipPlaneZ = CLIP_VALUE_Z*m_Focus;
	real ClipPlaneZ = CLIP_PLANE_Z;

    // define vertex position relativily clip plane
    for (j = 0; j<vv_num; j++)
    {
        // inside  1, outside -1
        real sign = vv[j]->z - ClipPlaneZ;

        if (sign > EPS)  signs[j] = 1;
        else 
            if (sign < -EPS)   signs[j] = -1; 
            else  signs[j] = 0;
    }

    /////////////////////////////////
    //  clip poly
    vvc_num = 0;
    int prevsign = signs[0];
    for (j = 0; j<vv_num + 1; j++)
    {
        int jj = j%vv_num;
        if (signs[jj] == 1)
        {
            // 1  1 |  или  |0  1 |
            if (prevsign != -1) { if (j != vv_num) *vvc[vvc_num++] = *vv[jj];} 
            // -1 | 1
            else
            {
                // find intersection

                int prevj = j - 1;
                CVertex *v2, *v1;
                v2 = vv[jj]; v1 = vv[prevj];

                real x, y, z;

                z = ClipPlaneZ;
                c = v2->z - v1->z;
                real opt_val1 = (z - v1->z)/c; // c != 0

                a = v2->x - v1->x;
                x = v1->x + a*opt_val1; 

                b = v2->y - v1->y;
                y = v1->y + b*opt_val1;

                vvc[vvc_num++]->Assign(x, y, z);
                if (j != vv_num) *vvc[vvc_num++] = *vv[jj];
            }
        }
        else
            if (signs[jj] == -1)
            {
                // 1 | -1
                if (prevsign == 1)
                {
                    // find intersection
                    int prevj = j - 1;
                    real x, y, z;
                    z = ClipPlaneZ;
                    c = vv[jj]->z - vv[prevj]->z;
                    real opt_val1 = (z - vv[prevj]->z)/c; // c != 0

                    a = vv[jj]->x - vv[prevj]->x;
                    x = vv[prevj]->x + a*opt_val1;

                    b = vv[jj]->y - vv[prevj]->y;
                    y = vv[prevj]->y + b*opt_val1;
                    
                    vvc[vvc_num++]->Assign(x, y, z);
                }
            }
            else // 0
            {
                if (j != vv_num && (signs[(j + 1)%vv_num] == 1 ||
                    signs[(j + vv_num - 1)%vv_num] == 1))
                {
                    *vvc[vvc_num++] = *vv[jj];
                }
            }
            prevsign = signs[jj];
    } 
    assert(vvc_num > 2);
}

//////////////////////////////////////////////////////////////////////

void CView::CalcOptValues()
{
    opt_m_ScreenWidth_2_div  = m_ScreenWidth/2 ;
    opt_m_ScreenHeight_2_div = m_ScreenHeight/2;
    opt_m_ScreenWidth_Width_m_Focus_div_mul = m_ScreenWidth/Width*m_Focus;
    opt_m_ScreenHeight_Height_m_Focus_div_mul = m_ScreenHeight/Height*m_Focus;
}
