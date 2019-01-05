// PBuffer.h: interface for the CPBuffer class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PBUFFER_H__8B389E80_B8FC_11D4_A137_00C026EFFB81__INCLUDED_)
#define AFX_PBUFFER_H__8B389E80_B8FC_11D4_A137_00C026EFFB81__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "common.h"
#include "SpanBuffer.h"

//////////////////////////////////////////////////////////////////////
//   
//////////////////////////////////////////////////////////////////////

class CPBuffer
{
public:
    CPBuffer(CSpanBuffer *sbuf);
    //~CPBuffer();
    void Refresh();
	int GetPValue(int i, int j) const { return m_pBuffer[i][j]; };
protected:
    void Init();
    int m_pBuffer[PBUFFER_LEN][PBUFFER_LEN];
    CSpanBuffer *m_sbuf;
private:
	CPBuffer(const CPBuffer &) {};
	void operator = (const CPBuffer &) {};
};


#endif // !defined(AFX_PBUFFER_H__8B389E80_B8FC_11D4_A137_00C026EFFB81__INCLUDED_)
