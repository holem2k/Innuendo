// Light.h: interface for the CLight class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LIGHT_H__9BB04101_A934_11D4_8DD6_00C026EFFB81__INCLUDED_)
#define AFX_LIGHT_H__9BB04101_A934_11D4_8DD6_00C026EFFB81__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "View.h"
#include "Texture.h"

const int LC_WHITE = 7;
const int LC_RED = 4;
const int LC_GREEN = 2;
const int LC_BLUE = 1;


class CLight  
{
public:
	CLight();
	virtual ~CLight();
    void SetPosition(real x, real y, real z);
    void SetDirection(real x, real y, real z);
    void SetAngle(real angle);
    CView *View() { return &m_View; }
	int Color() { return  m_Color; }
	void Color(int Color) { m_Color = Color;}
protected:
    CView m_View;
	int m_Color;
};

#endif // !defined(AFX_LIGHT_H__9BB04101_A934_11D4_8DD6_00C026EFFB81__INCLUDED_)
