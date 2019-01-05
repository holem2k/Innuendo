// ViewerDlg.cpp: implementation of the CViewerDlg class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ViewerDlg.h"
#include "resource.h"
#include "stdio.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


CObjectManager *CViewerDlg::m_ObjMan;
CView *CViewerDlg::m_View;
int CViewerDlg::dalfa, CViewerDlg::dbeta, CViewerDlg::dvega;
int CViewerDlg::CurSel;

CViewerDlg::CViewerDlg(CObjectManager *ObjMan, CView *View, HWND hParent)
{
	m_ObjMan = ObjMan;
	m_View = View;
	m_hDlg = CreateDialog(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_DIALOG2), hParent, (DLGPROC)DlgProc);
	assert(m_hDlg);
	Hide();
	HWND hComboBox = GetDlgItem(m_hDlg, IDC_COMBO1);
	for (int i = 0; i < ObjMan->ViewpointNum(); i++)
	{
		char buf[MAX_PATH];
		CVector v;
		int a;
		ObjMan->GetViewpoint(i, v, a, a, a);
		sprintf(buf, "Положение наблюдателя %u - (%3.2f, %3.2f, %3.2f)", i + 1, v.x, v.y, v.z);
		SendMessage(hComboBox, CB_INSERTSTRING, -1, (LPARAM)buf);

	}
	SendMessage(hComboBox, CB_SETCURSEL, 0, 0);
	ObjMan->ApplyToView(0, View);
	dalfa = dbeta = dvega = 0;
	CurSel = 0;
}

//////////////////////////////////////////////////////////////////////

CViewerDlg::~CViewerDlg()
{
	DestroyWindow(m_hDlg);
}

//////////////////////////////////////////////////////////////////////

BOOL CALLBACK CViewerDlg::DlgProc(HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	BOOL result = FALSE;
    switch (message) 
    { 
		case WM_INITDIALOG:
			result = TRUE;
			break;
        case WM_COMMAND: 
            switch (HIWORD(wParam)) 
            { 
			case CBN_SELCHANGE:
				CurSel = SendMessage((HWND)lParam, CB_GETCURSEL, 0, 0);
				if (CurSel != CB_ERR)
				{
					m_ObjMan->ApplyToView(CurSel, m_View);
					dalfa = dbeta = dvega = 0;
				}
				result = TRUE;
				break;
			case BN_CLICKED:
				switch(LOWORD(wParam))
				{
				case IDC_BUTTON_LEFT:
					dvega -= 5;
					break;
				case IDC_BUTTON_RIGHT:
					dvega += 5;
					break;
				case IDC_BUTTON_UP:
					dalfa -= 5;
					break;
				case IDC_BUTTON_DOWN:
					dalfa += 5;
					break;
				case IDC_BUTTON_ROTLEFT:
					dbeta += 5;
					break;
				case IDC_BUTTON_ROTRIGHT:
					dbeta -= 5;
					break;
			
				}
					m_ObjMan->ApplyToView(CurSel, m_View, dalfa, dbeta, dvega);

				break;

			}

            switch (LOWORD(wParam)) 
            { 
			case IDOK: 

				// Fall through. 
			case IDCANCEL: 
				//EndDialog(hwndDlg, wParam); 
				ShowWindow(hwndDlg, FALSE);
				result = TRUE; 
				break;
            } 
	}
	return  result;
}

//////////////////////////////////////////////////////////////////////

void CViewerDlg::Show()
{
	assert(m_hDlg);
	ShowWindow(m_hDlg, TRUE);

}

//////////////////////////////////////////////////////////////////////

void CViewerDlg::Hide()
{
	assert(m_hDlg);
	ShowWindow(m_hDlg, FALSE);
}

//////////////////////////////////////////////////////////////////////
