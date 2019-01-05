// Rasterizer.h: interface for the CRasterizer class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RASTERIZER_H__116D87C1_753E_11D4_8DD4_00C026EFFB81__INCLUDED_)
#define AFX_RASTERIZER_H__116D87C1_753E_11D4_8DD4_00C026EFFB81__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "Polygon.h"
#include "SpanBuffer.h"
#include "Common.h"


int __cdecl FixedIntToIntCeil(int a);
int __cdecl FixedIntToIntFloor(int a);

class CRasterizer  
{
    CSpanBuffer *m_SpanBuffer;
public:
    int EdgeBuf1[MAX_Y];
    int EdgeBuf1Z[MAX_Y];

    int EdgeBuf2[MAX_Y];
    int EdgeBuf2Z[MAX_Y];
    int MinY, MaxY;
//    BOOL bValidEdges;

    int nBottomClip;
    int nTopClip;
    int nLeftClip;
    int nRightClip;


    CRasterizer();
    ~CRasterizer();
    BOOL SpanPolygon(CPolygon *p, CVertex *sv[], int sv_num,  CVector *Normal);
    void AttachSpanBuffer(CSpanBuffer *psb) { m_SpanBuffer = psb; }
    void SetClipSize(int nHor, int nVert);
};


#endif // !defined(AFX_RASTERIZER_H__116D87C1_753E_11D4_8DD4_00C026EFFB81__INCLUDED_)
