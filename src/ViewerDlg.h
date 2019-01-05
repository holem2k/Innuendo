// ViewerDlg.h: interface for the CViewerDlg class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_VIEWERDLG_H__24802721_CF87_11D4_A137_00C026EFFB81__INCLUDED_)
#define AFX_VIEWERDLG_H__24802721_CF87_11D4_A137_00C026EFFB81__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ObjectManager.h"
#include "View.h"

class CViewerDlg  
{
public:
	CViewerDlg(CObjectManager *ObMan, CView *View, HWND hParent);
	virtual ~CViewerDlg();
	static BOOL CALLBACK DlgProc(HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam); 
	void Show();
	void Hide();
protected:
	HWND m_hDlg;
	static CObjectManager *m_ObjMan;
	static CView *m_View;
	static int dalfa, dbeta, dvega;
	static int CurSel;
};

#endif // !defined(AFX_VIEWERDLG_H__24802721_CF87_11D4_A137_00C026EFFB81__INCLUDED_)
