// ObjectDlg.cpp: implementation of the CObjectDlg class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ObjectDlg.h"
#include "resource.h"
#include "stdio.h"
#include "Worldloader.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CObjectManager *CObjectDlg::m_ObjMan;
int CObjectDlg::CurSel;

CObjectDlg::CObjectDlg(CObjectManager *ObjMan, HWND hParent)
{
	m_ObjMan = ObjMan;
	m_hDlg = CreateDialog(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_DIALOG3), hParent, (DLGPROC)DlgProc);
	assert(m_hDlg);
	Hide();

	HWND hComboBox = GetDlgItem(m_hDlg, IDC_COMBO1);
	for (int i = 0; i < ObjMan->ObjectNum(); i++)
	{
		char buf[MAX_PATH];
		CVector v;
		int ObjectType;
		ObjMan->GetObject(i, v, ObjectType);
		sprintf(buf, "Îáúåêò %u '%s' - (%3.2f, %3.2f, %3.2f)", i + 1, ObjectName[ObjectType],
			v.x, v.y, v.z);
		SendMessage(hComboBox, CB_INSERTSTRING, -1, (LPARAM)buf);
	}
	SendMessage(hComboBox, CB_SETCURSEL, 0, 0);
	CurSel = 0;
}

//////////////////////////////////////////////////////////////////////

CObjectDlg::~CObjectDlg()
{

}

//////////////////////////////////////////////////////////////////////

BOOL CALLBACK CObjectDlg::DlgProc(HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	BOOL result = FALSE;
	int State;
    switch (message) 
    { 
		case WM_INITDIALOG:
			CheckDlgButton(hwndDlg, IDC_CHECK1, BST_CHECKED);
			EnableWindow(GetDlgItem(hwndDlg, IDC_COMBO1), FALSE);
			EnableWindow(GetDlgItem(hwndDlg, IDC_BUTTON_XL), FALSE);
			EnableWindow(GetDlgItem(hwndDlg, IDC_BUTTON_YL), FALSE);
			EnableWindow(GetDlgItem(hwndDlg, IDC_BUTTON_ZL), FALSE);
			EnableWindow(GetDlgItem(hwndDlg, IDC_BUTTON_XR), FALSE);
			EnableWindow(GetDlgItem(hwndDlg, IDC_BUTTON_YR), FALSE);
			EnableWindow(GetDlgItem(hwndDlg, IDC_BUTTON_ZR), FALSE);
			result = TRUE;
			break;
        case WM_COMMAND: 
            switch (HIWORD(wParam)) 
            { 
			case CBN_SELCHANGE:
				CurSel = SendMessage((HWND)lParam, CB_GETCURSEL, 0, 0);
				if (CurSel == CB_ERR)
				{
					CurSel = -1;
				}
				result = TRUE;
				break;
			case BN_CLICKED:
				switch(LOWORD(wParam))
				{
				case IDC_CHECK1:
					State = IsDlgButtonChecked(hwndDlg, IDC_CHECK1) != BST_CHECKED;
					m_ObjMan->IsRunning(!State);

					EnableWindow(GetDlgItem(hwndDlg, IDC_COMBO1), State);
					EnableWindow(GetDlgItem(hwndDlg, IDC_BUTTON_XL), State);
					EnableWindow(GetDlgItem(hwndDlg, IDC_BUTTON_YL), State);
					EnableWindow(GetDlgItem(hwndDlg, IDC_BUTTON_ZL), State);
					EnableWindow(GetDlgItem(hwndDlg, IDC_BUTTON_XR), State);
					EnableWindow(GetDlgItem(hwndDlg, IDC_BUTTON_YR), State);
					EnableWindow(GetDlgItem(hwndDlg, IDC_BUTTON_ZR), State);

					break;
				case IDC_BUTTON_XL:
					if (CurSel != -1) m_ObjMan->RunObject(CurSel, ISROTX, -1);
					break;
				case IDC_BUTTON_XR:
					if (CurSel != -1) m_ObjMan->RunObject(CurSel, ISROTX, 1);
					break;
				case IDC_BUTTON_YL:
					if (CurSel != -1) m_ObjMan->RunObject(CurSel, ISROTY, -1);
					break;
				case IDC_BUTTON_YR:
					if (CurSel != -1) m_ObjMan->RunObject(CurSel, ISROTY, 1);
					break;
				case IDC_BUTTON_ZL:
					if (CurSel != -1) m_ObjMan->RunObject(CurSel, ISROTZ, -1);
					break;
				case IDC_BUTTON_ZR:
					if (CurSel != -1) m_ObjMan->RunObject(CurSel, ISROTZ, 1);
					break;
			
				}
				break;

			}

            switch (LOWORD(wParam)) 
            { 
			case IDOK: 

				// Fall through. 
			case IDCANCEL: 
				ShowWindow(hwndDlg, FALSE);
				result = TRUE; 
				break;
            } 
	}
	return  result;

}

//////////////////////////////////////////////////////////////////////

void CObjectDlg::Show()
{
	assert(m_hDlg);
	ShowWindow(m_hDlg, TRUE);

}

//////////////////////////////////////////////////////////////////////

void CObjectDlg::Hide()
{
	assert(m_hDlg);
	ShowWindow(m_hDlg, FALSE);
}

//////////////////////////////////////////////////////////////////////
