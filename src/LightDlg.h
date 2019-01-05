// LightDlg.h: interface for the CLightDlg class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LIGHTDLG_H__792F3D20_D3B4_11D4_A137_00C026EFFB81__INCLUDED_)
#define AFX_LIGHTDLG_H__792F3D20_D3B4_11D4_A137_00C026EFFB81__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ObjectManager.h"


class CLightDlg  
{
public:
	CLightDlg(CObjectManager *ObMan, HWND hParent);
	virtual ~CLightDlg();
	static BOOL CALLBACK DlgProc(HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam); 
	void Show();
	void Hide();
protected:
	static HWND m_hDlg;
	static CObjectManager *m_ObjMan;
	static int CurSel;
	static void SetColor(int Color);
};

#endif // !defined(AFX_LIGHTDLG_H__792F3D20_D3B4_11D4_A137_00C026EFFB81__INCLUDED_)
