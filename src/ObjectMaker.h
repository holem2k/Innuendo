// ObjectMaker.h: interface for the CObjectMaker class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_OBJECTMAKER_H__FEF473C0_CDF2_11D4_A137_00C026EFFB81__INCLUDED_)
#define AFX_OBJECTMAKER_H__FEF473C0_CDF2_11D4_A137_00C026EFFB81__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Polygon.h"

class CObjectMaker  
{
public:
	CObjectMaker();
	virtual ~CObjectMaker();
	CObject *makeCube(real x, real y, real z, real size, CTexture *t);
	CObject *makePyr(real x, real y, real z, real size, CTexture *t);
	CObject *makeRoom(real x, real y, real z, CTexture *tf, real stf, CTexture *tw, real stw, CTexture *tc, real stc);
};

#endif // !defined(AFX_OBJECTMAKER_H__FEF473C0_CDF2_11D4_A137_00C026EFFB81__INCLUDED_)
