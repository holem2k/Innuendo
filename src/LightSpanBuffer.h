// LightSpanBuffer.h: interface for the CLightSpanBuffer class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LIGHTSPANBUFFER_H__86561240_B84C_11D4_A137_00C026EFFB81__INCLUDED_)
#define AFX_LIGHTSPANBUFFER_H__86561240_B84C_11D4_A137_00C026EFFB81__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "common.h"
#include "Point2Di.h"
#include "Screen.h"
#include "PBuffer.h"

	
enum enSides { LEFTSIDE, RIGHTSIDE, BOTTOMSIDE, TOPSIDE };
enum enClipResult { NO_INTERSECTION, IS_INTERSECTION };



struct LightSpan
{
        int y;
        int xLeft;
        int xRight;
        int u0, v0;
        int u1, v1;
        int u2, v2;
		//CTexture *LightTexture;
		int Color;
		int  PBufferMask;
		void Draw (PIXEL *pLine, CPBuffer *pPBuffer);
};


class CLightSpanBuffer  
{
public:
	CLightSpanBuffer();
	virtual ~CLightSpanBuffer();
	void AddLightSpan(int  numy, CVector *v1, CVector *v2, CView *view, CLight *light, int PBufferMask);
	void InitLightSpanBuffer();
	void Draw(CScreen *screen, CPBuffer *pbuffer);
	static int DistLookUpTable[TEXTURE_SIZE][TEXTURE_SIZE];
	static WORD ColorAttLookUpTable[64];
protected:
	int Clip(int x1, int y1, int x2, int y2, float &t1, float &t2);
	int Clip(CVector *v1, CVector *v2);
	Point2Di N[4];
	Point2Di K[4];
	LightSpan LsBuffer[20000];
	int LsNum;
};

#endif // !defined(AFX_LIGHTSPANBUFFER_H__86561240_B84C_11D4_A137_00C026EFFB81__INCLUDED_)
