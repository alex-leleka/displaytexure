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
#if defined _MSC_VER || defined __BORLANDC__
#define OEMRESOURCE
#endif

#include <windows.h>  //include all the basics
#include <tchar.h>    //string and other mapping macros
#include <string>

//define an unicode string type alias
typedef std::basic_string<TCHAR> ustring;
//=============================================================================
//message processing function declarations
LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);
int OnCreate(const HWND,CREATESTRUCT*);

//non-message function declarations
HWND CreateButton(const HWND,const HINSTANCE,DWORD,const RECT&,const int,
									const ustring&);
inline int ErrMsg(const ustring&);

//setup some button id's
enum {
	IDBC_PUSHBUTTON_1=200,
	IDBC_PUSHBUTTON_2,
	IDBC_AUTOCHECKBOX,
	IDBC_AUTORADIOBUTTON,
	IDBC_GROUPBOX,
	IDBC_ICON,
	IDBC_BITMAP
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

int WINAPI WinMain1(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow);
//=============================================================================
LRESULT CALLBACK WndProc(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_CREATE:
		return OnCreate(hwnd,reinterpret_cast<CREATESTRUCT*>(lParam));
	case WM_DESTROY:
		PostQuitMessage(0);    //signal end of application
		return 0;
	case WM_COMMAND:
		if(wParam == IDBC_PUSHBUTTON_1)
		{
			 MessageBox(NULL, L"Hello World11", L"MessageBox Title", 0);
		} else if (wParam == IDBC_PUSHBUTTON_2)
		{
			 WinMain1(0, 0,0,0);
		}
		return 0;
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
	RECT rc={10,10,200,40};


	rc.top+=50;
	CreateButton(hwnd,cs->hInstance,BS_PUSHBUTTON,rc,IDBC_PUSHBUTTON_1,
		_T("PUSH BUTTON1"));

	rc.top+=50;
	CreateButton(hwnd,cs->hInstance,BS_PUSHBUTTON,rc,IDBC_PUSHBUTTON_2,
		_T("PUSH BUTTON2"));

	rc.top+=50;
	CreateButton(hwnd,cs->hInstance,BS_AUTOCHECKBOX,rc,IDBC_AUTOCHECKBOX,
		_T("CHECK BOX"));

	rc.top+=50;
	CreateButton(hwnd,cs->hInstance,BS_AUTORADIOBUTTON,rc,IDBC_AUTOCHECKBOX,
		_T("RADIO BUTTON"));

	return 0;
}
//=============================================================================
inline int ErrMsg(const ustring& s)
{
	return MessageBox(0,s.c_str(),_T("ERROR"),MB_OK|MB_ICONEXCLAMATION);
}
//=============================================================================
HWND CreateButton(const HWND hParent,const HINSTANCE hInst,DWORD dwStyle,
									const RECT& rc,const int id,const ustring& caption)
{
	dwStyle|=WS_CHILD|WS_VISIBLE;
	return CreateWindowEx(0,                            //extended styles
		_T("button"),                 //control 'class' name
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