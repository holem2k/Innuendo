// TextureDlg.cpp: implementation of the CTextureDlg class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TextureDlg.h"
#include "resource.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTextureDlg::CTextureDlg(HWND hParent, int TextureKind)
{
	m_TextureKind = TextureKind;
	m_hParent = hParent;
}

//////////////////////////////////////////////////////////////////////

CTextureDlg::~CTextureDlg()
{

}

//////////////////////////////////////////////////////////////////////

BOOL CALLBACK CTextureDlg::DlgProc(HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message) 
    { 
		case WM_INITDIALOG:
			if (m_TextureKind == TK_BILERP)
			{
				CheckDlgButton(hwndDlg, IDC_RADIO1, BST_UNCHECKED);
				CheckDlgButton(hwndDlg, IDC_RADIO2, BST_CHECKED);
			}
			else
			{
				CheckDlgButton(hwndDlg, IDC_RADIO1, BST_CHECKED);
				CheckDlgButton(hwndDlg, IDC_RADIO2, BST_UNCHECKED);
			}
			return TRUE;
			break;


        case WM_COMMAND: 
            switch (LOWORD(wParam)) 
            { 
			case IDOK: 
				m_TextureKind = IsDlgButtonChecked(hwndDlg, IDC_RADIO1) == BST_CHECKED ? TK_NORMAL : TK_BILERP;
				
				// Fall through. 
			case IDCANCEL: 
				EndDialog(hwndDlg, wParam); 
				return TRUE; 
            } 
	}
	return  FALSE;
}

//////////////////////////////////////////////////////////////////////

int CTextureDlg::DoModal()
{
	return DialogBox(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_DIALOG1), m_hParent, (DLGPROC)DlgProc);
}

//////////////////////////////////////////////////////////////////////

int CTextureDlg::m_TextureKind;
HWND CTextureDlg::m_hParent;

//////////////////////////////////////////////////////////////////////
