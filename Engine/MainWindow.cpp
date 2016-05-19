//=============================================================================
//BUTTON CONTROLS - Copyright © 2000,2005 Ken Fitlike
//=============================================================================
//API functions used: CreateWindowEx,DefWindowProc,DispatchMessage,GetMessage,
//GetSystemMetrics,LoadImage,MessageBox,PostQuitMessage,RegisterClassEx,
//SendMessage,ShowWindow,UpdateWindow,TranslateMessage,WinMain.
//=============================================================================
//This demonstrates the creation of the following button types: default push 
//button,push button,check box,radio button and groupbox. The image style of 
//'bitmap' buttons (BS_BITMAP) and 'icon' buttons (BS_ICON) can be used in 
//conjunction with other button styles to create image checkboxes, radio 
//buttons etc. The default is a push button.
//=============================================================================
/*
	displaytexure demo https://github.com/alex-leleka/displaytexure 
	alex leleka (c) 2016
*/
#if defined _MSC_VER || defined __BORLANDC__
#define OEMRESOURCE
#endif

#include <windows.h>  //include all the basics
#include <tchar.h>    //string and other mapping macros
#include <string>
#include "engine.h" // render engine

//define an unicode string type alias
typedef std::basic_string<TCHAR> ustring;
//=============================================================================
//message processing function declarations
LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);
int OnCreate(const HWND,CREATESTRUCT*);

//non-message function declarations
HWND CreateControl(const HWND,const HINSTANCE,DWORD,const RECT&,const int,
									const ustring&, const LPWSTR);
inline UINT AddString(const HWND,const ustring&);

inline int ErrMsg(const ustring&);

//control ids
enum {
	IDBC_PUSHBUTTON_APPLY_OPTIONS=200,
	IDBC_PUSHBUTTON_SHOW_VIEWPORT,
	IDCC_DROPDOWNLIST_MODELS,
	IDBC_AUTORADIOBUTTON1NOBLUR,
	IDBC_AUTORADIOBUTTON1BLURRIGHTSIDE,
	IDBC_AUTORADIOBUTTON1BLURTHEELLIPSE,
};
//=============================================================================
int WINAPI WinMain(HINSTANCE hInst,HINSTANCE,LPSTR pStr,int nCmd)
{
	ustring classname=_T("MAINWINDOW");
	WNDCLASSEX wcx={0};  //used for storing information about the wnd 'class'

	wcx.cbSize         = sizeof(WNDCLASSEX);           
	wcx.lpfnWndProc    = WndProc;
	wcx.hInstance      = hInst;
	wcx.hIcon         = reinterpret_cast<HICON>(LoadImage(0,IDI_APPLICATION,
		IMAGE_ICON,0,0,LR_SHARED));
	wcx.hCursor       = reinterpret_cast<HCURSOR>(LoadImage(0,IDC_ARROW,
		IMAGE_CURSOR,0,0,LR_SHARED));
	wcx.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_BTNFACE+1);   
	wcx.lpszClassName = classname.c_str(); 
	if (!RegisterClassEx(&wcx))
	{
		ErrMsg(_T("Failed to register wnd class"));
		return -1;
	}

	int desktopwidth=GetSystemMetrics(SM_CXSCREEN);
	int desktopheight=GetSystemMetrics(SM_CYSCREEN);

	int wndPosX = 100, wndPosY = 100; // top left corner
	int wndWidth = 300, wndHeight = 400;

	HWND hwnd=CreateWindowEx(0,                     //extended styles
		classname.c_str(),     //name: wnd 'class'
		_T("DemoMain"), //wnd title
		WS_OVERLAPPEDWINDOW|WS_CLIPCHILDREN|WS_CLIPSIBLINGS,   //wnd style
		wndPosX,
		wndPosY,
		wndWidth,
		wndHeight,
		0,                     //parent wnd handle
		0,                     //menu handle/wnd id
		hInst,                 //app instance
		0);                    //user defined info
	if (!hwnd)
	{
		ErrMsg(_T("Failed to create wnd"));
		return -1;
	}

	ShowWindow(hwnd,nCmd); 
	UpdateWindow(hwnd);
	//start message loop
	MSG msg;
	while (GetMessage(&msg,0,0,0)>0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return static_cast<int>(msg.wParam);
}

int GetBlurPatternIndex(HWND hwnd)
{
	int nBlurPattern = 0;
	if (IsDlgButtonChecked(hwnd, IDBC_AUTORADIOBUTTON1NOBLUR))
		nBlurPattern = 0;
	if (IsDlgButtonChecked(hwnd, IDBC_AUTORADIOBUTTON1BLURRIGHTSIDE))
		nBlurPattern = 1;
	if (IsDlgButtonChecked(hwnd, IDBC_AUTORADIOBUTTON1BLURTHEELLIPSE))
		nBlurPattern = 2;
	return nBlurPattern;
}

int GetLightingModelIndex(HWND hwnd)
{
	HWND hwndComboBox = GetDlgItem(hwnd, IDCC_DROPDOWNLIST_MODELS); 
	LRESULT item = SendMessage(hwndComboBox, CB_GETCURSEL, 0, 0); // index of the selected item in combo box
	if (item == CB_ERR)
		return 0;
	return item;
}

//=============================================================================
LRESULT CALLBACK WndProc(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	static TEngineCallbacks callbacks = {nullptr, nullptr};

	switch (uMsg)
	{
	case WM_CREATE:
		return OnCreate(hwnd,reinterpret_cast<CREATESTRUCT*>(lParam));
	case WM_DESTROY:
		PostQuitMessage(0);    //signal end of application
		return 0;
	case WM_COMMAND:
		if(wParam == IDBC_PUSHBUTTON_APPLY_OPTIONS)
		{
			if (callbacks[eCB_LightingModel])
				callbacks[eCB_LightingModel](GetLightingModelIndex(hwnd));

			if (callbacks[eCB_BlurPattern])
				callbacks[eCB_BlurPattern](GetBlurPatternIndex(hwnd));

		} else if (wParam == IDBC_PUSHBUTTON_SHOW_VIEWPORT)
		{
			RunEngine(callbacks);
		} else if (LOWORD(wParam) == IDCC_DROPDOWNLIST_MODELS)
		{
			if(HIWORD(wParam)==CBN_SELCHANGE)
			{
				if (callbacks[eCB_LightingModel])
					callbacks[eCB_LightingModel](GetLightingModelIndex(hwnd));
			}
		} else if (LOWORD(wParam) == IDBC_AUTORADIOBUTTON1NOBLUR || 
			LOWORD(wParam) == IDBC_AUTORADIOBUTTON1BLURRIGHTSIDE || LOWORD(wParam) == IDBC_AUTORADIOBUTTON1BLURTHEELLIPSE)
		{
			if (callbacks[eCB_BlurPattern])
				callbacks[eCB_BlurPattern](GetBlurPatternIndex(hwnd));
		}
	default:
		//let system deal with msg
		return DefWindowProc(hwnd,uMsg,wParam,lParam);  
	}
}
//=============================================================================
int OnCreate(const HWND hwnd,CREATESTRUCT *cs)
{
	//handles the WM_CREATE message of the main, parent window; return -1 to fail
	//window creation
	RECT rc={10,10,210,40};

	CreateControl(hwnd,cs->hInstance,BS_PUSHBUTTON,rc,IDBC_PUSHBUTTON_SHOW_VIEWPORT,
		_T("Show viewport"), _T("button"));

	rc.top+=50;
	CreateControl(hwnd,cs->hInstance,BS_PUSHBUTTON,rc,IDBC_PUSHBUTTON_APPLY_OPTIONS,
		_T("Apply options"), _T("button"));

	rc.top+=50;
	rc.bottom = 100;
	HWND hCombo=CreateControl(hwnd,cs->hInstance,CBS_DROPDOWNLIST	,rc,IDCC_DROPDOWNLIST_MODELS,_T("Lighting model"), _T("combobox"));
	AddString(hCombo,_T("Lambertian lighting"));
	AddString(hCombo,_T("Blinn-Phong lighting"));
	AddString(hCombo,_T("Minnaert vertex lighting"));
	unsigned nItemIndex = 0;
	SendMessage(hCombo, CB_SETCURSEL, (WPARAM)nItemIndex, (LPARAM)0); // select first option (this doesn't send SELCHANGE even)

	rc.top+=30;
	rc.bottom = 30;
	CreateControl(hwnd,cs->hInstance,BS_AUTORADIOBUTTON,rc,IDBC_AUTORADIOBUTTON1NOBLUR,
		_T("No blur"), _T("button"));
	rc.top+=30;
	CreateControl(hwnd,cs->hInstance,BS_AUTORADIOBUTTON,rc,IDBC_AUTORADIOBUTTON1BLURRIGHTSIDE,
		_T("Blur right window half"), _T("button"));
	rc.top+=30;
	CreateControl(hwnd,cs->hInstance,BS_AUTORADIOBUTTON,rc,IDBC_AUTORADIOBUTTON1BLURTHEELLIPSE,
		_T("Blur the ellipse in the middle"), _T("button"));

	return 0;
}
//=============================================================================
inline int ErrMsg(const ustring& s)
{
	return MessageBox(0,s.c_str(),_T("ERROR"),MB_OK|MB_ICONEXCLAMATION);
}
//=============================================================================
HWND CreateControl(const HWND hParent,const HINSTANCE hInst,DWORD dwStyle,
										const RECT& rc,const int id,const ustring& caption, const LPWSTR ctrlClassName)
{
	dwStyle|=WS_CHILD|WS_VISIBLE;
	return CreateWindowEx(0,                            //extended styles
		ctrlClassName,               //control 'class' name
		caption.c_str(),              //control caption
		dwStyle,                      //control style 
		rc.left,                      //position: left
		rc.top,                       //position: top
		rc.right,                     //width
		rc.bottom,                    //height
		hParent,                      //parent window handle
		//control's ID
		reinterpret_cast<HMENU>(static_cast<INT_PTR>(id)),
		hInst,                        //application instance
		0);                           //user defined info
}
//=============================================================================
inline UINT AddString(const HWND hCombo,const ustring& s)
{
	return static_cast<UINT>(SendMessage(hCombo,CB_ADDSTRING,0,
		reinterpret_cast<LPARAM>(s.c_str())));
}
//=============================================================================