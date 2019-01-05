// WinSizeDlg.cpp: implementation of the CWinSizeDlg class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "WinSizeDlg.h"
#include "resource.h"

//////////////////////////////////////////////////////////////////////

int CWinSizeDlg::m_WinSize;
HWND CWinSizeDlg::m_hParent;

//////////////////////////////////////////////////////////////////////


CWinSizeDlg::CWinSizeDlg(HWND hParent, int WinSize)
{
	m_WinSize = WinSize;
	m_hParent = hParent;

}

//////////////////////////////////////////////////////////////////////

CWinSizeDlg::~CWinSizeDlg()
{

}

//////////////////////////////////////////////////////////////////////

BOOL CALLBACK CWinSizeDlg::DlgProc(HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message) 
    { 
		case WM_INITDIALOG:
			if (m_WinSize == WS_SIZE1)
			{
				CheckDlgButton(hwndDlg, IDC_RADIO2, BST_UNCHECKED);
				CheckDlgButton(hwndDlg, IDC_RADIO1, BST_CHECKED);
			}
			else
			{
				CheckDlgButton(hwndDlg, IDC_RADIO2, BST_CHECKED);
				CheckDlgButton(hwndDlg, IDC_RADIO1, BST_UNCHECKED);
			}
			return TRUE;
			break;


        case WM_COMMAND: 
            switch (LOWORD(wParam)) 
            { 
			case IDOK: 
				m_WinSize = IsDlgButtonChecked(hwndDlg, IDC_RADIO1) == BST_CHECKED ? WS_SIZE1 : WS_SIZE2;
				
				// Fall through. 
			case IDCANCEL: 
				EndDialog(hwndDlg, wParam); 
				return TRUE; 
            } 
	}
	return  FALSE;
}


//////////////////////////////////////////////////////////////////////

int CWinSizeDlg::DoModal()
{
	return DialogBox(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_DIALOG4), m_hParent, (DLGPROC)DlgProc);
}

//////////////////////////////////////////////////////////////////////
