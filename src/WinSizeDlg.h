// WinSizeDlg.h: interface for the CWinSizeDlg class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WINSIZEDLG_H__DF594EA1_D09A_11D4_A137_00C026EFFB81__INCLUDED_)
#define AFX_WINSIZEDLG_H__DF594EA1_D09A_11D4_A137_00C026EFFB81__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

const int WS_SIZE1 = 0;
const int WS_SIZE2 = 1;


class CWinSizeDlg  
{
public:
	CWinSizeDlg(HWND hParent, int WinSize);
	virtual ~CWinSizeDlg();
	int WindowSize() { return m_WinSize; }
	int DoModal();
	static BOOL CALLBACK DlgProc(HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam); 
private:
	static int m_WinSize;
	static HWND m_hParent;
};

#endif // !defined(AFX_WINSIZEDLG_H__DF594EA1_D09A_11D4_A137_00C026EFFB81__INCLUDED_)
