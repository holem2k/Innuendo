// SpanBuffer.cpp: implementation of the CSpanBuffer class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SpanBuffer.h"
#include "Common.h"
#include "Memoryman.h"

#include "Texture.h"

#ifdef _DEBUG
#include "stdio.h"
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSpanBuffer::CSpanBuffer(int maxy)
{
    assert(maxy <= MAX_Y);
    MaxY = maxy;
    for (int i = 0; i<MaxY; i++)
        SpanBuffer[i] = NULL;
}

//////////////////////////////////////////////////////////////////////

CSpanBuffer::~CSpanBuffer()
{
    InitSpanBuffer();
}

//////////////////////////////////////////////////////////////////////

void CSpanBuffer::InitSpanBuffer()
{
    for (int i = 0; i<MaxY; i++)
    {
        if (SpanBuffer[i] != NULL)
        {
            Span *span, *nextspan;
            span = SpanBuffer[i]; 
            do 
            {
                nextspan = span->Next;
                deleteSpan(span);
                span = nextspan;
            } while (span);
            SpanBuffer[i] = NULL;
        }
    }
}

//////////////////////////////////////////////////////////////////////

void CSpanBuffer::AddSpan(int y, CPolygon *Poly, int xLeft, int xRight, float zLeft, float dz, int bSloped)
{
    Span *curspan, *gluespan, **prevspan_next; 
    int xOrLeft;

    curspan = SpanBuffer[y];
    prevspan_next = &SpanBuffer[y];
    gluespan = NULL;

    xOrLeft = xLeft;

    while (curspan)
    {
        if (xRight <  curspan->xLeft) // whole span more left than current span
        {
            if (gluespan)
            {
                gluespan->xRight = xRight;
            }
            else
            {
                Span *span = newSpan();
                span->Assign(Poly, xLeft, xRight, zLeft, dz, bSloped); //**
                span->Next = curspan;
                (*prevspan_next) = span;
            }
            return;
        }
        else
        if (xLeft <= curspan->xRight) // span intersect current span
        {
            if (xLeft < curspan->xLeft) // some part of span more left than current span
            {
                if (gluespan)
                {
                    gluespan->xRight = curspan->xLeft - 1;
                    // gluespan still valid
                }
                else
                {
                    Span *span = newSpan();
                    span->Assign(Poly, xLeft, curspan->xLeft - 1, zLeft, dz, bSloped); //**
                    span->Next = curspan;
                    (*prevspan_next) = span;
                    prevspan_next = &span->Next;

                    gluespan = span;
                }
                // correct span;
                zLeft = zLeft + dz*(curspan->xLeft - xLeft);
                xLeft = curspan->xLeft;
            }

            //   curspan->xLeft <= xLeft <= curspan->xRight;
            bool visible, invisible;
            real z;
            visible = false;
            invisible = false;
            z = curspan->zLeft + curspan->dz*(xLeft - curspan->xLeft);
            if (z < zLeft/* - EPSz > zLeft + EPS*/) visible = true;
            else
                if (z > zLeft/* + EPSz < zLeft - EPS*/) invisible = true;
                else // this case is very seldom, FUCK, its main case(cause adjacent polys)
                {
                    
                    // left point of span lies on current span,
                    // check most right point
                    int checkpoint; real rightz;

                    if (xRight <= curspan->xRight)  checkpoint = xRight;
                    else checkpoint = curspan->xRight;

                    z = curspan->zLeft + curspan->dz*(checkpoint - curspan->xLeft);
                    rightz = zLeft + dz*(checkpoint - xLeft);

                    if (z < rightz/* - EPSz > rightz + EPS*/) visible = true;
                    else 
                        if (z > rightz/* + EPSz < rightz - EPS*/) invisible = true;
                }

            if (visible)
            {
                // inserted span is visible
                //          1        2
                // ^ z     ------------   curspan = temp
                // |         ====== 
                // |         |- xLeft
                // |       |- temp.xLeft  
                Span temp;
                temp = curspan;

                /**** check for current span part 1 ****/
                if (temp.xLeft < xLeft)
                {
                    Span *span1 = newSpan();
                    span1->Assign(temp.Poly, temp.xLeft, xLeft - 1, temp.zLeft, temp.dz, temp.bSloped); //**
                    // insert in list
                    span1->Next = curspan;
                    (*prevspan_next) = span1;
                    prevspan_next = &span1->Next;
                    gluespan = NULL;
                }

                /**** check for current span part 2 ****/
                if (xRight < temp.xRight)
                {
                    Span *span2 = newSpan();
                    span2->Assign(temp.Poly, xRight + 1, temp.xRight, temp.zLeft, temp.dz, temp.bSloped);
                    // insert in list
                    span2->Next = curspan->Next;
                    curspan->Next = span2;
                //    gluespan = NULL; will exit next if {}, gluespan will not needed any more 
                }

                /**** insert span ****/
                if (xRight <= temp.xRight)
                {
                    if (gluespan)
                    {
                        gluespan->xRight = xRight;
                        gluespan->Next = curspan->Next;
                        deleteSpan(curspan);
                    }
                    else  curspan->Assign(Poly, xLeft, xRight, zLeft, dz, bSloped);
                    return;
                }
                else
                {
                    if (gluespan)
                    {
                        gluespan->xRight = temp.xRight;
                        gluespan->Next = curspan->Next;
                        deleteSpan(curspan);
                        curspan = gluespan;
                        // gluespan is still valid
                    }
                    else
                    {
                        curspan->Assign(Poly, xLeft, temp.xRight, zLeft, dz, bSloped);
                        gluespan = curspan;
                    }
                    // correct span
                    zLeft += dz*(temp.xRight + 1 - xLeft);
                    xLeft = temp.xRight + 1;
                }
            }
            else
            if (invisible)
            {
                // inserted span is not visible
                //           =======
                // ^ z     ------------   curspan = temp
                // |         
                // |
                if (xRight <= curspan->xRight)
                {
                    return;
                }
                else
                {
                    // correct span
                    zLeft += dz*(curspan->xRight + 1 - xLeft);
                    xLeft = curspan->xRight + 1;
                    gluespan = NULL;
                }

            }
            else
            {
                // the spans coincide with each other
                // to do ...........
                if (xRight <= curspan->xRight) return;
                zLeft += dz*(curspan->xRight + 1 - xLeft);
                xLeft = curspan->xRight + 1;
                gluespan = NULL;
            }

        }

        prevspan_next = &(curspan->Next);
        curspan = curspan->Next;
    }

    if (gluespan)
    {
        gluespan->xRight = xRight;
    }
    else
    {
        Span *span = newSpan();
        span->Assign(Poly, xLeft, xRight, zLeft, dz, bSloped);
        span->Next = NULL;
        (*prevspan_next) = span;
    }
}

//////////////////////////////////////////////////////////////////////

void CSpanBuffer::Draw(CScreen *screen)
{
    void *pLine = screen->Lock();
    DWORD dwPitch = screen->GetPitch();

    for (int i = 0; i<MaxY; i++)
    {
        Span *span = SpanBuffer[i];
        while (span)
        {
            span->Poly->Draw(pLine, i, span);
            span = span->Next;
        }
        pLine = (unsigned char *)pLine + dwPitch;
		//if (i%20 == 0) PumpMessage();
    }
    screen->Unlock();
}

//////////////////////////////////////////////////////////////////////
bool CSpanBuffer::iterBegin(int numy, Span *&span)
{
    span =  numy < MaxY ?  SpanBuffer[numy] : NULL;
    return numy < MaxY;  
}

//////////////////////////////////////////////////////////////////////


void CSpanBuffer::DrawInPBuf(int pbuf[PBUFFER_LEN][PBUFFER_LEN])
{
/*    for (int i = 0; i<MaxY; i++)
    {
        Span *span = SpanBuffer[i];
        while (span)
        {
            assert(span->xRight<128);
            for (int j = span->xLeft; j <= span->xRight; j++)
                pbuf[PBUFFER_LEN*i + j] = (int)span->Poly;
            span = span->Next;
        }
    }
	*/
	assert(MaxY<=PBUFFER_LEN);
    for (int i = 0; i<MaxY; i++)
    {
        Span *span = SpanBuffer[i];
        while (span)
        {
            assert(span->xRight<PBUFFER_LEN);
            for (int j = span->xLeft; j <= span->xRight; j++)
                pbuf[i][j] = (int)span->Poly;
            span = span->Next;
        }
    }
}

//////////////////////////////////////////////////////////////////////

#ifdef _DEBUG
void CSpanBuffer::Dump(int i)
{
    OutputDebugString("\r\n");
    char s[200];
    wsprintf(s, "s-buffer, line %u:\r\n", i); 
    OutputDebugString(s);
    if (SpanBuffer[i] == NULL)
    {
        OutputDebugString("List is empty.\r\n");
        return;
    }
    OutputDebugString("Left     Right    Z      dZ\r\n");
    Span *span = SpanBuffer[i];
    do 
    {
        sprintf(s, "%3u      %3u      %3.2f   %3.2f\r\n", span->xLeft, span->xRight, span->zLeft, span->dz);
        OutputDebugString(s);
        span = span->Next;
    } while (span);
    OutputDebugString("\r\n");
}
#endif

//////////////////////////////////////////////////////////////////////

void CSpanBuffer::PumpMessage()
{
	MSG msg;
	if (PeekMessage(&msg, 0, 0, 0, PM_NOREMOVE))
	{
		if (msg.message != WM_QUIT)
		{
			PeekMessage(&msg, 0, 0, 0, PM_REMOVE);
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
}

//////////////////////////////////////////////////////////////////////


