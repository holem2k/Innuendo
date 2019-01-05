#include "stdafx.h"
#include "resource.h"
#include "Commdlg.h"

#include "View.h"
#include <float.h>
#include "Win.h"
#include "Engine.h"
#include <crtdbg.h>
#include "Memoryman.h"
#include "stdio.h"
#include "assert.h"

#include "Light.h"
#include "WorldLoader.h"
#include "ObjectManager.h"

#include "TextureDlg.h"
#include "ViewerDlg.h"
#include "ObjectDlg.h"
#include "WinSizeDlg.h"
#include "LightDlg.h"

//#define pi 3.141592f


const int WindowSize[2][2] = {{320, 480}, {400, 600}};


class CApplication
{
    static CRenderMan *RenderMan;
    static CView   *View;
    static CWin    *Window;
	static CWorld *World;
	static CObjectManager *ObjectManager;
	static CViewerDlg *ViewerDlg;
	static CObjectDlg *ObjectDlg;
	static CLightDlg *LightDlg;
	static BOOL OpenedWorld;
	static BOOL OpenedDialog;
	static int WindowSize;
public:
    CApplication();
    ~CApplication();
    void Render();
    static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
    BOOL MessagePump();
    void Run();
	static void Open();
	static void Close();
	static void TextureKind();
	static void ViewerSettings();
	static void ObjectSettings();
	static void LightSettings();
	static void SetWindowSize();
};

CRenderMan *CApplication::RenderMan;
CView   *CApplication::View;
CWin    *CApplication::Window;
CWorld *CApplication::World;
CObjectManager *CApplication::ObjectManager;
CViewerDlg *CApplication::ViewerDlg;
CObjectDlg *CApplication::ObjectDlg;
CLightDlg *CApplication::LightDlg;

int CApplication::WindowSize;
BOOL CApplication::OpenedWorld;
BOOL CApplication::OpenedDialog;


//////////////////////////////////////////////////////////////////////

CApplication::CApplication()
{
    memInit();
	OpenedDialog = 0;
	OpenedWorld = 0;
	View = NULL;
	RenderMan = NULL;
	World = NULL;
	ObjectManager = NULL;

	ViewerDlg = NULL;
	ObjectDlg = NULL;
	LightDlg = NULL;
	WindowSize = 0;

    Window = new CWin;
    Window->Create(::WindowSize[0][1], ::WindowSize[0][0], WndProc);

	EnableMenuItem(Window->GetMenu(), ID_MENU_LIGHT, MF_GRAYED);
	EnableMenuItem(Window->GetMenu(), ID_MENU_OBJECT, MF_GRAYED);
	EnableMenuItem(Window->GetMenu(), ID_MENU_VIEWER, MF_GRAYED);
	EnableMenuItem(Window->GetMenu(), ID_MENU_TEXTURE, MF_GRAYED);
	EnableMenuItem(Window->GetMenu(), ID_MENU_CLOSE, MF_GRAYED);


};

//////////////////////////////////////////////////////////////////////

CApplication::~CApplication()
{
    delete RenderMan;
    delete World;
	delete Window;
	delete View;
	delete ObjectManager;
	delete ViewerDlg;
	delete ObjectDlg;
	delete LightDlg;
    memDone();
};

//////////////////////////////////////////////////////////////////////

LRESULT CALLBACK CApplication::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	RECT Rect;
	HDC hdc;
    switch (message)
    {
    case WM_CLOSE: PostQuitMessage(0);
        break;
    case WM_PAINT: 
		if (!OpenedWorld)
		{
			hdc = BeginPaint(hWnd, &ps);
			GetClientRect(hWnd, &Rect);
			FillRect(hdc, &Rect, (HBRUSH)(COLOR_BTNTEXT + 1));
			EndPaint(hWnd, &ps);
		}
		else
		if (OpenedDialog/*1*/)
		{
			        if (RenderMan) RenderMan->Draw();
		}
        break;
    case WM_COMMAND: 
		switch (wParam)
		{
		case ID_MENU_OPEN:
			Open();
			break;
		case ID_MENU_CLOSE:
			Close();
			break;
		case ID_MENU_EXIT:
			PostQuitMessage(0);
			break;
		case ID_MENU_TEXTURE:
			TextureKind();
			break;
		case ID_MENU_VIEWER:
			ViewerSettings();
			break;
		case ID_MENU_OBJECT:
			ObjectSettings();
			break;
		case ID_MENU_WINSIZE:
			SetWindowSize();
			break;
		case ID_MENU_LIGHT:
			LightSettings();
		}
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

//////////////////////////////////////////////////////////////////////

void CApplication::Render()
{

}

//////////////////////////////////////////////////////////////////////

BOOL CApplication::MessagePump()
{
    MSG msg;
	for (int i = 0; i < 10; i++)
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT) return FALSE;
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
    return TRUE;
}

//////////////////////////////////////////////////////////////////////

void CApplication::Run()
{
    DWORD t1, t2, time;
    int i = 0; time = 0; float fps = 1.0;
    while(MessagePump())
    {
		if (OpenedWorld)
		{
			t1 = GetTickCount();

			__try
			{
				ObjectManager->Run();
				RenderMan->RenderWorld();
				RenderMan->Draw();
			}
			__except(GetExceptionCode() == EXCEPTION_ACCESS_VIOLATION ?
				EXCEPTION_EXECUTE_HANDLER : EXCEPTION_CONTINUE_SEARCH)
			{
				MessageBox(0, "Error", "Error", MB_OK);
			}
			t2 = GetTickCount();             i++;
			time += t2 - t1;
			if (i >= 20)
			{
				fps = 1000.0f*i/(time);
				i = 0; time = 0;
			}
				char s[10];
				sprintf(s, "%2.2f fps", fps);
				HDC dc = Window->GetDC();
				SetBkMode(dc, TRANSPARENT); 
				TextOut(dc, 00, 00, s, strlen(s));
				Window->ReleaseDC(dc);
		}
    }
}

//////////////////////////////////////////////////////////////////////

void CApplication::Open()
{
    OPENFILENAME ofn;       
    char szFile[260] = "scene1.wld";       
    char szTitle[] = "Выберите файл";
  
    // Initialize OPENFILENAME
    ZeroMemory(&ofn, sizeof(OPENFILENAME));
    ofn.lStructSize = sizeof(OPENFILENAME);
    ofn.hwndOwner = Window->GetHandle();
    ofn.lpstrFile = szFile;
    ofn.nMaxFile = sizeof(szFile);
    ofn.lpstrFilter = "*.wld\0*.wld\0";
    ofn.nFilterIndex = 1;
    ofn.lpstrFileTitle = szTitle;
    ofn.nMaxFileTitle = lstrlen(szTitle);
    ofn.lpstrInitialDir = NULL;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
    ofn.lpstrTitle = szTitle;
    
    // Display the Open dialog box. 

    OpenedDialog = TRUE;
    if (GetOpenFileName(&ofn))
    {
		CWorldLoader wl;
		if (wl.LoadWorld(szFile, World, ObjectManager))
		{
			View = new CView;
			///////////
			View->SetFrustum(40, 40*1.5, 40);
			View->SetScreenSize(::WindowSize[WindowSize][1],
				::WindowSize[WindowSize][0]);
			///////////////
			RenderMan = new CRenderMan(World, View, Window);
			ViewerDlg = new CViewerDlg(ObjectManager, View, Window->GetHandle());
			ObjectDlg = new CObjectDlg(ObjectManager, Window->GetHandle());
			LightDlg = new CLightDlg(ObjectManager, Window->GetHandle());
			OpenedWorld = TRUE;

			if (ObjectManager->ObjectNum())
			EnableMenuItem(Window->GetMenu(), ID_MENU_OBJECT, MF_ENABLED);

			if (ObjectManager->LightNum())
			EnableMenuItem(Window->GetMenu(), ID_MENU_LIGHT, MF_ENABLED);

			EnableMenuItem(Window->GetMenu(), ID_MENU_WINSIZE, MF_GRAYED);
			EnableMenuItem(Window->GetMenu(), ID_MENU_VIEWER, MF_ENABLED);
			EnableMenuItem(Window->GetMenu(), ID_MENU_TEXTURE, MF_ENABLED);
			EnableMenuItem(Window->GetMenu(), ID_MENU_CLOSE, MF_ENABLED);
			EnableMenuItem(Window->GetMenu(), ID_MENU_OPEN, MF_GRAYED);
		}
		else
		{
			MessageBox(0, "Ошибка загрузки файла !", "Ошибка", MB_OK);
			World = NULL;
			ObjectManager = NULL;
		}
	}
	OpenedDialog = FALSE;
}

//////////////////////////////////////////////////////////////////////

void CApplication::Close()
{
	OpenedWorld = FALSE;
    delete RenderMan; RenderMan =  NULL;
    delete World; World = NULL;
	delete View; View =  NULL;
	delete ObjectManager; ObjectManager =  NULL;
	delete ViewerDlg; ViewerDlg = NULL;
	delete ObjectDlg; ObjectDlg = NULL;
	delete LightDlg; LightDlg = NULL;

	EnableMenuItem(Window->GetMenu(), ID_MENU_LIGHT, MF_GRAYED);
	EnableMenuItem(Window->GetMenu(), ID_MENU_WINSIZE, MF_ENABLED);
	EnableMenuItem(Window->GetMenu(), ID_MENU_OBJECT, MF_GRAYED);
	EnableMenuItem(Window->GetMenu(), ID_MENU_VIEWER, MF_GRAYED);
	EnableMenuItem(Window->GetMenu(), ID_MENU_TEXTURE, MF_GRAYED);
	EnableMenuItem(Window->GetMenu(), ID_MENU_CLOSE, MF_GRAYED);
	EnableMenuItem(Window->GetMenu(), ID_MENU_OPEN, MF_ENABLED);
	InvalidateRect(Window->GetHandle(), NULL, FALSE);
	UpdateWindow(Window->GetHandle());
}

//////////////////////////////////////////////////////////////////////

void CApplication::TextureKind()
{
	CTextureDlg dlg(Window->GetHandle(), CPolygon::TextureKind);
	OpenedDialog = TRUE;
	if (dlg.DoModal() == IDOK)
	{
		CPolygon::TextureKind = dlg.TextureKind();
	}
	OpenedDialog = FALSE;
}

//////////////////////////////////////////////////////////////////////

void CApplication::ViewerSettings()
{
	ViewerDlg->Show();
}

//////////////////////////////////////////////////////////////////////

void CApplication::ObjectSettings()
{
	ObjectDlg->Show();
}

//////////////////////////////////////////////////////////////////////

void CApplication::SetWindowSize()
{
	CWinSizeDlg dlg(Window->GetHandle(), WindowSize);
	OpenedDialog = TRUE;
	if (dlg.DoModal() == IDOK)
	{
		WindowSize = dlg.WindowSize();
		RECT Rect;
		GetWindowRect(Window->GetHandle(), &Rect);
		MoveWindow(Window->GetHandle(),
			Rect.left,
			Rect.top,
			::WindowSize[WindowSize][1],
			::WindowSize[WindowSize][0] + GetSystemMetrics(SM_CYCAPTION), TRUE);
	}
	OpenedDialog = FALSE;
}

//////////////////////////////////////////////////////////////////////

void CApplication::LightSettings()
{
	LightDlg->Show();
}

//////////////////////////////////////////////////////////////////////


int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{

    CApplication *Application = new CApplication;
    Application->Run();
    delete Application;

#ifdef _DEBUG
   _CrtDumpMemoryLeaks();
#endif

    return 0;
}


