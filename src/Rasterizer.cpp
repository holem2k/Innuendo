// Rasterizer.cpp: implementation of the CRasterizer class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Common.h"
#include "assert.h"
#include "stdio.h"
#include "math.h"
#include "Rasterizer.h"

//////////////////////////////////////////////////////////////////////

#pragma warning (disable : 4035)
int __cdecl FixedIntToIntCeil(int a)
{
    _asm 
    {
        mov eax, a;
        test eax, 0x0000FFFF;
        jnz not_integer;
        sar eax, 10h; 
        jmp finish; 
not_integer:
        sar eax, 10h;
        inc eax;
finish:
    }
}
#pragma warning (default: 4035)

//////////////////////////////////////////////////////////////////////

#pragma warning (disable : 4035)
int __cdecl FixedIntToIntFloor(int a)
{
    _asm 
    {
        mov eax, a;
        sar eax, 10h
    }
}

//////////////////////////////////////////////////////////////////////
#pragma warning (default: 4035)


//////////////////////////////////////////////////////////////////////
#pragma warning (disable : 4035)
int __cdecl batchFixedIntToIntCeil(void *p, int min, int max)
{
    _asm 
    {
        mov ebx, min;
        mov edi,p; // edi pointer to data
        lea edi, [edi + 4*ebx]
        mov ecx, max;  // setup loop
        sub ecx, ebx;
        inc ecx;

loop2:
        cmp ecx, 0;
        jz end2;

        mov eax, [edi];
        test eax, 0x0000FFFF;
        jnz not_integer2;
        sar eax, 10h; 
        jmp finish2; 
not_integer2:
        sar eax, 10h;
        inc eax;
finish2:

        mov [edi], eax;
        add edi, 4;
        dec ecx

        jmp loop2;
end2:
    }
}
#pragma warning (default: 4035)

//////////////////////////////////////////////////////////////////////

#pragma warning (disable : 4035)
int __cdecl batchFixedIntToIntFloor(void *p, int min, int max)
{
    _asm 
    {
        mov ebx, min;
        mov edi,p; // edi pointer to data
        lea edi, [edi + 4*ebx]
        mov ecx, max;  // setup loop
        sub ecx, ebx;
        inc ecx;

loop3:
        cmp ecx, 0;
        jz end3;

        mov eax, [edi];
        sar eax, 10h
        mov [edi], eax;
//        sar [edi], 10h

        add edi, 4
        dec ecx;
        jmp loop3;
end3:
    }
}
#pragma warning (default: 4035)

//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRasterizer::CRasterizer()
{
    m_SpanBuffer = NULL;
}

//////////////////////////////////////////////////////////////////////

CRasterizer::~CRasterizer()
{

}

//////////////////////////////////////////////////////////////////////

BOOL CRasterizer::SpanPolygon(CPolygon *p, CVertex *sv[], int sv_num, CVector *Normal)
{
    assert(sv_num >= 3);

    int nTop = 0;
    int nBottom = 0;
    int nLeft = 0;
    int nRight = 0;

    real TopValue, BottomValue, LeftValue, RightValue;
    BottomValue = TopValue = sv[0]->y;
    LeftValue = RightValue =  sv[0]->x;

    int i;
    // sort vertexes
    for (i = 1; i<sv_num; i++)
    {
        if (sv[i]->y > TopValue)
        {
            TopValue = sv[i]->y;
            nTop = i;
        }
        else
        if (sv[i]->y < BottomValue)
        {
           BottomValue = sv[i]->y;
           nBottom = i;
        }
        if (sv[i]->x > RightValue)
        {
            RightValue = sv[i]->x;
            nRight = i;
        }
        else
        if (sv[i]->x < LeftValue)
        {
            LeftValue = sv[i]->x;
            nLeft = i;
        }
    }

    int i_BottomValue = (int)ceil(BottomValue);
    int i_TopValue = (int)floor(TopValue);
    int i_LeftValue = (int)(LeftValue);
    int i_RightValue = (int)(RightValue);

    // reject external polys
    if (i_BottomValue > nTopClip || i_TopValue < nBottomClip ||
        i_LeftValue > nRightClip || i_RightValue < nLeftClip) 
    {
        //bValidEdges = FALSE;
        return FALSE;
    }

    // find left and right sides
    int *LeftEdge, *RightEdge;
    int *LeftEdgeZ, *RightEdgeZ;

    LeftEdge = EdgeBuf1;
    LeftEdgeZ = EdgeBuf1Z;
    RightEdge = EdgeBuf2;
    RightEdgeZ = EdgeBuf2Z;


    //////////////////////////
     //  rasterizes edges  //
    //////////////////////////
    int *Edge,  *ZEdge;
    CVertex *v1, *v2;

    float x1, x2, y1, y2, z1, z2;
    float rdx, rdz;
    int i_dx, i_dz;
    int i_y1, i_y2, i_x, i_z;
    int len;
    int k;

    // push the loops
    i = nBottom;
    v2 = sv[i];	
    x2 = v2->x;
    y2 = v2->y;
    z2 = 1.0f/v2->z; /*z2 = v2->z*/;

    // bottom -> top loop
    do 
    {
        /* next edge */
        IncMod(i, sv_num);

        Edge = LeftEdge;
        ZEdge = LeftEdgeZ;

        v1 = v2;
        v2 = sv[i];

        x1 = x2;
        y1 = y2;
        z1 = z2;

        x2 = v2->x;
        y2 = v2->y;
        z2 = 1.0f/v2->z;/*z2 = v2->z;*/

        if (y2 - y1 > 0)
        {
              i_y1 = (int)ceil(y1); 
              i_y2 = (int)floor(y2);

              if (i_y2 >= nBottomClip && i_y1 <= nTopClip)
              {
                  if (i_y1 < nBottomClip) i_y1 = nBottomClip; // CLIP1 - bottom
                  if (i_y2 > nTopClip) i_y2 = nTopClip;       //CLIP2 - top

                  real opt_iy1_y1_sub = i_y1 - y1;
                  real opt_one_y2_y1_sub_div = 1.0f/(y2 - y1);

                  rdx = (x2 - x1)*opt_one_y2_y1_sub_div;
                  i_dx = (int)(rdx*65536);
                  i_x = (int)((x1 + rdx*opt_iy1_y1_sub)*65536);

                  rdz = (z2 - z1)*opt_one_y2_y1_sub_div;
                  i_dz = (int)(rdz*FLOAT_TO_FIX32_MUL				);/*i_dz = (int)(rdz*65536);*/
                  i_z = (int)((z1 + rdz*opt_iy1_y1_sub)*FLOAT_TO_FIX32_MUL);/*i_z = (int)((z1 + rdz*opt_iy1_y1_sub)*65536);*/

                  len = i_y2 - i_y1; // = -1, 0, >0
                  
                  Edge += i_y1;
                  ZEdge += i_y1;
                  
                  // main loop
                  for (k = 0; k<=len; k++)
                  {
                      *(Edge++) = i_x;
                      i_x += i_dx;
                      *(ZEdge++) = i_z;
                      i_z += i_dz;
                  }
              }
        }
    } while (i != nTop);


    // top -> bottom loop
    // i, x2, y2, z2, v2 are still valid
    do 
    {
        /* next edge */
        IncMod(i, sv_num);
        Edge = RightEdge;
        ZEdge = RightEdgeZ;

        v1 = v2;
        v2 = sv[i];

        x1 = x2;
        y1 = y2;
        z1 = z2;

        x2 = v2->x;
        y2 = v2->y;
        z2 = 1.0f/v2->z;/*z2 = v2->z;*/

        if (y1 - y2 > 0)
        {
              i_y1 = (int)floor(y1); 
              i_y2 = (int)ceil(y2);

              if (i_y1 >= nBottomClip && i_y2 <= nTopClip)
              {
                  if (i_y1 > nTopClip) i_y1 = nTopClip; // CLIP1 - top
                  if (i_y2 < nBottomClip) i_y2 = nBottomClip; // CLIP2 - bottom

                  real opt_y1_iy2_sub = i_y2 - y1;
                  real opt_one_y1_y2_sub_div = 1.0f/(y2 - y1);

                  rdx = (x2 - x1)*opt_one_y1_y2_sub_div; 
                  i_x = (int)((x1 + rdx*opt_y1_iy2_sub)*65536.0);
                  i_dx = (int)(rdx*65536.0);

                  rdz = (z2 - z1)*opt_one_y1_y2_sub_div;
                  /*i_z = (int)((z1 + rdz*opt_y1_iy2_sub)*65536.0);
                  i_dz = (int)(rdz*65536.0);*/
                  i_z = (int)((z1 + rdz*opt_y1_iy2_sub)*FLOAT_TO_FIX32_MUL);
                  i_dz = (int)(rdz*FLOAT_TO_FIX32_MUL);
                  
                  len = i_y1 - i_y2;  // = -1, 0, >0
                  Edge += i_y2;
                  ZEdge += i_y2;
                  
                  // main loop
                  for (k = 0; k<=len; k++)
                  {
                      *(Edge++) = i_x;
                      i_x += i_dx;
                      *(ZEdge++) = i_z;
                      i_z += i_dz;
                  }
              }

        }
    } while (i != nBottom);


    //////////////////////
//    bValidEdges = TRUE;
    if (i_BottomValue < nBottomClip) MinY = nBottomClip;
    else MinY = i_BottomValue;
    if (i_TopValue > nTopClip) MaxY = nTopClip;
    else MaxY = i_TopValue;
    //////////////////////


    int AvgY = (MinY + MaxY)/2;
    if (LeftEdge[AvgY] > RightEdge[AvgY])
    {
        LeftEdge = EdgeBuf2;
        LeftEdgeZ = EdgeBuf2Z;
        RightEdge = EdgeBuf1;
        RightEdgeZ = EdgeBuf1Z;
    }

    int i_LeftEdge, i_RightEdge, i_Span, dxLeft;
    real dz = -Normal->x/Normal->z; // dz is constant for WHOLE poly (wich =df= is part of plane);
    int bSloped = dz>.5 || dz<-.5;

    batchFixedIntToIntCeil(LeftEdge, MinY, MaxY);
    batchFixedIntToIntFloor(RightEdge, MinY, MaxY);
    for (i = MinY; i<=MaxY; i++)
    {   
		real rdz;
		if (RightEdge[i] - LeftEdge[i]) rdz = (RightEdgeZ[i] - LeftEdgeZ[i])*FIX32_TO_FLOAT_MUL/(RightEdge[i] - LeftEdge[i]);
		else rdz = 0;
        /////// process right edge
        i_RightEdge = RightEdge[i];
        if (i_RightEdge > nRightClip) i_RightEdge = nRightClip; // right clip 
        
        /////// process left edge
        i_LeftEdge = LeftEdge[i];
        if (i_LeftEdge < nLeftClip) // left clip
        {
            dxLeft = nLeftClip - i_LeftEdge;
            /////// add span to span buffer
            i_Span =  i_RightEdge - nLeftClip;
            if (i_Span >= 0)
                m_SpanBuffer->AddSpan(i, p, nLeftClip, i_RightEdge, (LeftEdgeZ[i])*FIX32_TO_FLOAT_MUL + rdz*dxLeft, rdz, bSloped);

        }
        else 
        {
            /////// add span to span buffer
            i_Span =  i_RightEdge - i_LeftEdge;
            if (i_Span >= 0)
                m_SpanBuffer->AddSpan(i, p, i_LeftEdge, i_RightEdge, LeftEdgeZ[i]*FIX32_TO_FLOAT_MUL, rdz, bSloped);
        }

    }
 
    return TRUE;
}

//////////////////////////////////////////////////////////////////////

void CRasterizer::SetClipSize(int nHor, int nVert)
{
    nBottomClip = 0;
    nTopClip = nVert - 1;
    nLeftClip = 0;
    nRightClip = nHor - 1;
}