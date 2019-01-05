// LightDlg.cpp: implementation of the CLightDlg class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "LightDlg.h"
#include "resource.h"
#include "stdio.h"
#include "Light.h"

//////////////////////////////////////////////////////////////////////

CObjectManager *CLightDlg::m_ObjMan;
int CLightDlg::CurSel;
HWND CLightDlg::m_hDlg;

//////////////////////////////////////////////////////////////////////

CLightDlg::CLightDlg(CObjectManager *ObjMan, HWND hParent)
{
	CVector v;
	int color;

	m_ObjMan = ObjMan;

	m_hDlg = CreateDialog(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_DIALOG5), hParent, (DLGPROC)DlgProc);
	assert(m_hDlg);

	Hide();
	HWND hComboBox = GetDlgItem(m_hDlg, IDC_COMBO1);
	for (int i = 0; i < ObjMan->LightNum(); i++)
	{
		char buf[MAX_PATH];
		ObjMan->GetLight(i, v, color);
		sprintf(buf, "Источник света %u - (%3.2f, %3.2f, %3.2f)", i + 1, v.x, v.y, v.z);
		SendMessage(hComboBox, CB_INSERTSTRING, -1, (LPARAM)buf);

	}
	CurSel = 0;
	SendMessage(hComboBox, CB_SETCURSEL, 0, 0);
	ObjMan->GetLight(0, v, color);
	SetColor(color);
}

//////////////////////////////////////////////////////////////////////

CLightDlg::~CLightDlg()
{
	DestroyWindow(m_hDlg);

}

//////////////////////////////////////////////////////////////////////

BOOL CALLBACK CLightDlg::DlgProc(HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam)
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
				if (CurSel == CB_ERR)
				{
					CurSel = -1;
				}
				result = TRUE;
				break;
			case BN_CLICKED:
				if (CurSel != -1)
				{
					switch(LOWORD(wParam))
					{
					case IDC_RADIO_WHITE:
						m_ObjMan->RunLight(CurSel, 0, 0, LC_WHITE);
						break;
					case IDC_RADIO_RED:
						m_ObjMan->RunLight(CurSel, 0, 0, LC_RED);
						break;
					case IDC_RADIO_GREEN:
						m_ObjMan->RunLight(CurSel, 0, 0, LC_GREEN);
						break;
					case IDC_RADIO_BLUE:
						m_ObjMan->RunLight(CurSel, 0, 0, LC_BLUE);
						break;
						
					case IDC_BUTTON_LEFT:
						m_ObjMan->RunLight(CurSel, ISROTZ, -1, 0);
						break;
					case IDC_BUTTON_RIGHT:
						m_ObjMan->RunLight(CurSel, ISROTZ, 1, 0);
						break;
					case IDC_BUTTON_UP:
						m_ObjMan->RunLight(CurSel, ISROTX, -1, 0);
						break;
					case IDC_BUTTON_DOWN:
						m_ObjMan->RunLight(CurSel, ISROTX, 1, 0);
						break;
					}

				}
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

void CLightDlg::Show()
{
	assert(m_hDlg);
	ShowWindow(m_hDlg, TRUE);

}

//////////////////////////////////////////////////////////////////////

void CLightDlg::Hide()
{
	assert(m_hDlg);
	ShowWindow(m_hDlg, FALSE);
}

//////////////////////////////////////////////////////////////////////

void CLightDlg::SetColor(int color)
{
	switch (color)
	{
	case LC_WHITE:
		CheckDlgButton(m_hDlg, IDC_RADIO_WHITE, BST_CHECKED);
		break;
	case LC_GREEN:
		CheckDlgButton(m_hDlg, IDC_RADIO_GREEN, BST_CHECKED);
		break;
	case LC_BLUE:
		CheckDlgButton(m_hDlg, IDC_RADIO_BLUE, BST_CHECKED);
		break;
	case LC_RED:
		CheckDlgButton(m_hDlg, IDC_RADIO_RED, BST_CHECKED);
		break;
	}
}

//////////////////////////////////////////////////////////////////////