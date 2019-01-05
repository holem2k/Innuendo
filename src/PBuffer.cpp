// PBuffer.cpp: implementation of the CPBuffer class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PBuffer.h"

//////////////////////////////////////////////////////////////////////
//                    class CPBuffer
//////////////////////////////////////////////////////////////////////

CPBuffer::CPBuffer(CSpanBuffer *sbuf) : m_sbuf(sbuf)
{
    Init();
}

//////////////////////////////////////////////////////////////////////

void CPBuffer::Refresh()
{
    Init();
    m_sbuf->DrawInPBuf(m_pBuffer);
}

//////////////////////////////////////////////////////////////////////

void CPBuffer::Init()
{
    for (int i = 0; i<PBUFFER_LEN; i++)
		for (int j = 0; j<PBUFFER_LEN; j++)
			m_pBuffer[i][j] = 0;
}

//////////////////////////////////////////////////////////////////////

//int CPBuffer::GetPValue(int i, int j)
//{
	
//}

//////////////////////////////////////////////////////////////////////
