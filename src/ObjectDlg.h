// ObjectDlg.h: interface for the CObjectDlg class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_OBJECTDLG_H__DF594EA0_D09A_11D4_A137_00C026EFFB81__INCLUDED_)
#define AFX_OBJECTDLG_H__DF594EA0_D09A_11D4_A137_00C026EFFB81__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ObjectManager.h"

class CObjectDlg  
{
public:
	CObjectDlg();
	virtual ~CObjectDlg();
	CObjectDlg(CObjectManager *ObMan, HWND hParent);

	static BOOL CALLBACK DlgProc(HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam); 
	void Show();
	void Hide();
protected:
	HWND m_hDlg;
	static CObjectManager *m_ObjMan;
	static int CurSel;
};

#endif // !defined(AFX_OBJECTDLG_H__DF594EA0_D09A_11D4_A137_00C026EFFB81__INCLUDED_)
