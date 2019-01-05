// LightSpanBuffer.cpp: implementation of the CLightSpanBuffer class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "LightSpanBuffer.h"
#include "View.h"
#include "Light.h"
#include "math.h"

//////////////////////////////////////////////////////////////////////

int CLightSpanBuffer::DistLookUpTable[TEXTURE_SIZE][TEXTURE_SIZE];
WORD CLightSpanBuffer::ColorAttLookUpTable[64];

//////////////////////////////////////////////////////////////////////

CLightSpanBuffer::CLightSpanBuffer()
{
	N[LEFTSIDE].Assign(1, 0); 	K[LEFTSIDE].Assign(3, 3);
	N[RIGHTSIDE].Assign(-1, 0); K[RIGHTSIDE].Assign(PBUFFER_LEN - 4, PBUFFER_LEN - 4);
	N[BOTTOMSIDE].Assign(0, 1); K[BOTTOMSIDE].Assign(PBUFFER_LEN - 4, 3);
	N[TOPSIDE].Assign(0, -1);   K[TOPSIDE].Assign(3, PBUFFER_LEN - 4);
	for (int i = 0; i<128; i++)
		for (int j = 0; j<128; j++)
			DistLookUpTable[i][j] = (int)sqrt((64 - i)*(64 - i)+ (64 - j)*(64 - j));

}

//////////////////////////////////////////////////////////////////////

CLightSpanBuffer::~CLightSpanBuffer()
{
	InitLightSpanBuffer();
}

//////////////////////////////////////////////////////////////////////

void CLightSpanBuffer::InitLightSpanBuffer()
{
	LsNum = 0;
}

//////////////////////////////////////////////////////////////////////

void CLightSpanBuffer::AddLightSpan(int numy, CVector *v1, CVector *v2, CView *View, CLight *light, int PBufferMask)
{
	CView *LightView = light->View();
	View->ScreenToWorld(v1, v1);
	LightView->WorldToView(v1, v1);
	View->ScreenToWorld(v2, v2);
	LightView->WorldToView(v2, v2);
	if (!Clip(v1, v2)) return;
	LightView->ViewToScreen(v1, v1);
	LightView->ViewToScreen(v2, v2);

	float t1, t2;
	if (Clip((int)v1->x, (int)v1->y, (int)v2->x, (int)v2->y, t1, t2) == IS_INTERSECTION)
	{
		// find t1 & t2 points
		CVector pt0, pt1, pt2;
		v1->z = 1.0f/v1->z;
		v2->z = 1.0f/v2->z;
		pt0.x = v1->x + (v2->x - v1->x)*t1;
		pt0.y = v1->y + (v2->y - v1->y)*t1;
		pt0.z = 1.0f/(v1->z + (v2->z - v1->z)*t1);

		pt2.x = v1->x + (v2->x - v1->x)*t2;
		pt2.y = v1->y + (v2->y - v1->y)*t2;
		pt2.z = 1.0f/(v1->z + (v2->z - v1->z)*t2);

		CVector span_p1, span_p2;
		LightView->ScreenToScreen(View, &pt0, &span_p1);
		LightView->ScreenToScreen(View, &pt2, &span_p2);

		// calc texture coord of mid lightspan
		CVector v;
		v.x = (span_p1.x + span_p2.x)/2;
		v.y = numy;
		v.z = 2*span_p1.z*span_p2.z/(span_p1.z + span_p2.z);
		View->ScreenToScreen(LightView, &v, &pt1);

		// fill LightSpan structure;
		LightSpan *lightspan = &LsBuffer[LsNum++];
		lightspan->xLeft = round(span_p1.x);
		lightspan->xRight = round(span_p2.x);
		lightspan->y = numy;
		lightspan->u0 = (int)(pt0.x*FLOAT_TO_FIX_MUL);
		lightspan->v0 = (int)(pt0.y*FLOAT_TO_FIX_MUL);
		lightspan->u1 = (int)(pt1.x*FLOAT_TO_FIX_MUL);
		lightspan->v1 = (int)(pt1.y*FLOAT_TO_FIX_MUL);
		lightspan->u2 = (int)(pt2.x*FLOAT_TO_FIX_MUL);
		lightspan->v2 = (int)(pt2.y*FLOAT_TO_FIX_MUL);
		lightspan->Color = light->Color();
		lightspan->PBufferMask = PBufferMask;
		
	}
}

//////////////////////////////////////////////////////////////////////

int CLightSpanBuffer::Clip(int x1, int y1, int x2, int y2, float &t1, float &t2)
{
	float tmin = 0, tmax = 1;
	for (int i = 0; i < 4; i++)
	{
		Point2Di D(x2 - x1, y2 - y1);
		Point2Di W(x1, y1);
		W = W - K[i];
		float WN = W*N[i];
		float DN = D*N[i];
		if (DN != 0)
		{
			float t = -WN/DN;
			if (DN < 0)
			{
				if (t < 0) return NO_INTERSECTION;
				if (t < tmax) tmax = t;
			}
			else
			{
				if (t > 1) return NO_INTERSECTION;
				if (t > tmin) tmin = t;
			}
		}
		else
		{
			if (WN < 0) return NO_INTERSECTION;
		}
	}
	int result = NO_INTERSECTION;
	if (tmin < tmax)
	{
		t1 = tmin;
		t2 = tmax;
		result = IS_INTERSECTION;
	}
	return result;
}

//////////////////////////////////////////////////////////////////////

int CLightSpanBuffer::Clip(CVector *v1, CVector *v2)
{
	int f1 = v1->z < CLIP_PLANE_Z; 
	int f2 = v2->z < CLIP_PLANE_Z; 
	if (f1 || f2)
	{
		if (f1 && f2) return 0;
		if (f1)
		{
			float t = (CLIP_PLANE_Z - v1->z)/(v2->z - v1->z);
			v1->x += (v2->x - v1->x)*t;
			v1->y += (v2->y - v1->y)*t;
			v1->z = CLIP_PLANE_Z;
		}
		else
		{
			float t = (CLIP_PLANE_Z - v2->z)/(v1->z - v2->z);
			v2->x += (v1->x - v2->x)*t;
			v2->y += (v1->y - v2->y)*t;
			v2->z = CLIP_PLANE_Z;
		}
	}	
	return 1;
}

//////////////////////////////////////////////////////////////////////

void CLightSpanBuffer::Draw(CScreen *screen, CPBuffer *pPBuffer)
{

    void *pLine = screen->Lock();
    DWORD dwPitch = screen->GetPitch();

	for (int i = 0; i<LsNum; i++)
	{
		LightSpan *lightspan = &LsBuffer[i];
        WORD *pLineCur  = (WORD *)((unsigned char *)pLine + dwPitch*lightspan->y);
		lightspan->Draw(pLineCur, pPBuffer);
	}
};

//////////////////////////////////////////////////////////////////////

void LightSpan::Draw(PIXEL *pLine, CPBuffer *pPBuffer)
{
	int u, v, du, ddu, dv, ddv;
	u = u0;
	v = v0;
	if (xLeft != xRight)
	{
		real a1 = -3*u0 + 4*u1 - u2;
		real b1 = -3*v0 + 4*v1 - v2;
		real a2 = 2*u0 - 4*u1 + 2*u2;
		real b2 = 2*v0 - 4*v1 + 2*v2;

		real rdx = 1.0/(xRight - xLeft);

		real rdx2 = rdx*rdx;
		du = a2*rdx2 + a1*rdx;
		ddu = 2*a2*rdx2;
		dv = b2*rdx2 + b1*rdx;
		ddv = 2*b2*rdx2;
	}

	
	for (int i = xLeft; i<=xRight; i++)
	{
		int su;
		int sv;
	
		su = u>>16;
		sv = v>>16;

		if ((u&0xFFFF) > 0x7FFF) su++;
		if ((v&0xFFFF) > 0x7FFF) sv++;
		if (su < 0 || su > 127 || sv < 0 || sv > 127 )
		{
			return;
		}
		int len = CLightSpanBuffer::DistLookUpTable[su][sv];
		if (PBufferMask == pPBuffer->GetPValue(sv, su) ||
			PBufferMask == pPBuffer->GetPValue(sv + 1, su) ||
			PBufferMask == pPBuffer->GetPValue(sv - 1, su) || 
			PBufferMask == pPBuffer->GetPValue(sv, su + 1) ||
			PBufferMask == pPBuffer->GetPValue(sv, su - 1) ||
			PBufferMask == pPBuffer->GetPValue(sv + 1, su + 1) ||
			PBufferMask == pPBuffer->GetPValue(sv + 1, su - 1) ||
			PBufferMask == pPBuffer->GetPValue(sv - 1, su + 1) ||
			PBufferMask == pPBuffer->GetPValue(sv - 1, su - 1))
			if (len < 60)
			{
				int backcolor = pLine[i];
				int r = Color & LC_RED ?  (RGB16RED(backcolor)*(len) +  28*(64-len))>>6 : RGB16RED(backcolor);
				int g = Color & LC_GREEN ? (RGB16GREEN(backcolor)*(len) +  28*(64-len))>>6 :  RGB16GREEN(backcolor);
				int b = Color & LC_BLUE ? (RGB16BLUE(backcolor)*(len) +  28*(64-len))>>6 : RGB16BLUE(backcolor);
				pLine[i] = RGB16(r, g, b);
			}
			u += du;
			du += ddu;
			v += dv;
			dv += ddv;
	}
}
