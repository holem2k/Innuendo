// TextureDlg.h: interface for the CTextureDlg class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TEXTUREDLG_H__24802720_CF87_11D4_A137_00C026EFFB81__INCLUDED_)
#define AFX_TEXTUREDLG_H__24802720_CF87_11D4_A137_00C026EFFB81__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

const int TK_NORMAL = 0;
const int TK_BILERP = 1;

class CTextureDlg  
{
public:
	CTextureDlg(HWND hParent, int TextureKind);
	virtual ~CTextureDlg();
	int TextureKind() { return m_TextureKind; }
	int DoModal();
	static BOOL CALLBACK DlgProc(HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam); 
private:
	static int m_TextureKind;
	static HWND m_hParent;
};

#endif // !defined(AFX_TEXTUREDLG_H__24802720_CF87_11D4_A137_00C026EFFB81__INCLUDED_)
