// SpanBuffer.h: interface for the CSpanBuffer class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SPANBUFFER_H__77E1B14F_79DD_11D4_8DD4_00C026EFFB81__INCLUDED_)
#define AFX_SPANBUFFER_H__77E1B14F_79DD_11D4_8DD4_00C026EFFB81__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Polygon.h"
#include "Screen.h"
#include "Common.h"

struct Span
{
    CPolygon *Poly;
    int xLeft;
    int xRight;
    float dz;
    float zLeft;
    Span *Next;
    int bSloped;
    void Assign(CPolygon *_Poly, int _xLeft, int _xRight, float _zLeft,  float _dz, int _bSloped)
    {
        Poly = _Poly;   xLeft = _xLeft;  xRight = _xRight;  zLeft = _zLeft;  dz = _dz; bSloped = _bSloped;
    }
    operator=(Span *s)
    {
        Assign(s->Poly, s->xLeft, s->xRight, s->zLeft, s->dz, s->bSloped);
    }
    void GetPoints(CVector &p1, CVector &p2, int y)
    {
        p1.Assign((float)xLeft, (float)y, 1.0f/zLeft);
        p2.Assign((float)xRight, (float)y, 1.0f/(zLeft + (xRight - xLeft)*dz));
    }

};


class CSpanBuffer  
{
public:
    void InitSpanBuffer();
	CSpanBuffer(int MaxY);
	~CSpanBuffer();
    void AddSpan(int y, CPolygon *poly, int xLeft, int xRight, float zLeft, float dz, int bSloped);
    void Draw(CScreen *screen);
    bool iterBegin(int numy, Span *&span);
    void DrawInPBuf(int pbuf[PBUFFER_LEN][PBUFFER_LEN]);
#ifdef _DEBUG
    void Dump(int i);
#endif
protected:
    int MaxY;
    Span *SpanBuffer[MAX_Y];
	void PumpMessage();
};

#endif // !defined(AFX_SPANBUFFER_H__77E1B14F_79DD_11D4_8DD4_00C026EFFB81__INCLUDED_)
