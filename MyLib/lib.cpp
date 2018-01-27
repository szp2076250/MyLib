# include "stdafx.h"
# include "lib.h"
# include <windows.h>
# include <thread>

 void ErrorMessage(const char * _error)
 {
	 MessageBox(NULL,_error,"error",MB_OK);
	 exit(0);
 }

 void BaseWindow::MoveCenterWindow()
 {
	 int tempWidth = SCREEN_WIDTH;
	 int tempHeight = SCREEN_HEIGHT;
	 int centerX = tempWidth / 2;
	 int centerY = tempHeight / 2;
	 centerX = centerX - GetWidth() / 2;
	 centerY = centerY - GetHeight() / 2;
	 POINT tempPoint = {centerX,centerY};
	 SetLeftTopPos(tempPoint);
 }

BaseWindow::BaseWindow():mWidth(1024),mHeight(768),mIsfullscreen(false),mBaseHwnd(NULL),
		mClassname("lib"),mWindowname("LIB"),mWindowStyle(WS_OVERLAPPEDWINDOW),
	mWindowStyleEx(WS_EX_APPWINDOW),mInstance(GetModuleHandle(NULL)),mHdc(NULL),mBackground(NULL)
{
}

bool BaseWindow::ShowThisWindow()
{
	WNDCLASSEX wndcls;
	ZeroMemory(&wndcls,sizeof(wndcls));

	wndcls.cbClsExtra = 0;
	wndcls.cbSize     = sizeof(WNDCLASSEX);
	wndcls.cbWndExtra = 0;
	wndcls.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndcls.hIcon = LoadIcon(NULL, IDI_APPLICATION);

	if(mBackground!=NULL) wndcls.hbrBackground = mBackground;
	else wndcls.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);

	if(mInstance) 	wndcls.hInstance = mInstance;
	else wndcls.hInstance = NULL;
	
	if(!mCallBackFunc) 	wndcls.lpfnWndProc = WinProc;
	else wndcls.lpfnWndProc = mCallBackFunc;

	wndcls.lpszClassName = mClassname;
	wndcls.lpszMenuName = NULL;

	if (!mWindowStyle) 	wndcls.style = CS_HREDRAW | CS_VREDRAW;
	else wndcls.style = CS_HREDRAW | CS_VREDRAW;
	
	RegisterClassEx(&wndcls);

	InitBeforeCreate();
	MoveCenterWindow();

	mBaseHwnd = CreateWindowEx(mWindowStyleEx,mClassname,mWindowname,
		mWindowStyle,mLeftTop.x,mLeftTop.y,mWidth,mHeight,NULL,NULL,NULL,this);

	if(!mBaseHwnd)
	{
		ErrorMessage("��������ʧ��");
		return false;
	}

	AfterCreate();

	SetWindowLong(mBaseHwnd,GWL_USERDATA,(LONG)this);
	ShowWindow(mBaseHwnd,SW_SHOWNORMAL);
	UpdateWindow(mBaseHwnd);

	MessageLoop();
	return true;
}

bool BaseWindow::Show()
{
	auto proc = [](LPVOID lpParameter)-> DWORD WINAPI
	{
		auto semphore = OpenSemaphore(SEMAPHORE_ALL_ACCESS,false,"WindowOccur");
		ReleaseSemaphore(semphore, 1, NULL);
		BaseWindow * pWindow = reinterpret_cast<BaseWindow *>(lpParameter);
		// -------------- 
		pWindow->ShowThisWindow();
		return 0;
	};

	DWORD hThreadId;
	auto hThread = CreateThread(NULL, 0, proc, this, 0, &hThreadId); 
	if (!hThread) return false;
	auto error = GetLastError();

	CloseHandle(hThread);
	return true;
}

void BaseWindow::SetCallBackFunc(pCallBackFunc mFunc)
{
	mCallBackFunc = mFunc;
}

void BaseWindow::MessageLoop()
{
	MSG Message;
	PeekMessage(&Message,NULL,0,0,PM_NOREMOVE);
	static float fLastTime  = static_cast<float>(GetTickCount());
	while(Message.message!= WM_QUIT)
	{
		auto bHasMessage = PeekMessage(&Message,NULL,0,0,PM_REMOVE);
		if(bHasMessage)
		{
			TranslateMessage(&Message);
			DispatchMessage(&Message);
		}
		else
		{
			float fIntervalTime = static_cast<float>(GetTickCount())-fLastTime;
			Update(fIntervalTime);
			fLastTime = fIntervalTime;
		}	
	}
}

//WM_NCCREATE posted before WM_CREATE
LRESULT CALLBACK WinProc(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	static HDC hDC;
	static BaseWindow * window;

	switch (uMsg)
	{
		case WM_CREATE:
			hDC = GetDC(hWnd);
			window->SetHDC(hDC);
			window->OnCreate();
			break;
		case WM_DESTROY:
			window->Destory();
			if(hDC) ReleaseDC(hWnd, hDC);
			PostQuitMessage(0);
			break;
		case WM_PAINT:
			//window = (My_Window *)GetWindowLong(hWnd,GWL_USERDATA);
			if(window==NULL) 
				ErrorMessage("window==NULL");
			window->OnDraw();
			break;
		case WM_NCCREATE:
			window = (BaseWindow*)(((CREATESTRUCT *)lParam)->lpCreateParams);
			break;
		case WM_NCPAINT:
			window->OnNcPaint(wParam);
			break;
		case WM_NCCALCSIZE:
			window->OnNcCalcSize(wParam,lParam);
			break;
		case WM_KEYUP:
			break;
		case WM_KEYDOWN:
			break;
		case WM_MOUSEMOVE:
			break;
		case WM_LBUTTONDOWN:
			window->OnLButtonDown(wParam, lParam);
			break;
		case WM_LBUTTONUP:
			break;
		case WM_SIZING:
			window->ReDraw();
			break;
		case WM_COMMAND:
			auto ControlId = LOWORD(wParam);
			switch(ControlId)
			{
			case 9999:
				ErrorMessage("����");
				break;
			}
			break;
	}
	return DefWindowProc(hWnd,uMsg,wParam,lParam);
}

void BaseWindow::Update(float delta)
{
}

void BaseWindow::Destory()
{
	DestroyWindow(mBaseHwnd);
}

 BaseWindow::~BaseWindow()
{
}

 void BaseWindow::OnDraw()
 {
 }

 void BaseWindow::OnCreate()
 { 

 }

 void BaseWindow::OnNcPaint(WPARAM wParam)
 {	
 }

 void BaseWindow::InitBeforeCreate()
 {

 }

 void BaseWindow::ReDraw()
 {
	RECT rect;
	GetClientRect(mBaseHwnd,&rect);
	InvalidateRect(mBaseHwnd,&rect,true);
 }


 void BaseWindow::AfterCreate()
 {
	 //test for subwindow
	 auto hButton = CreateWindow(
		 "Button", "�ٺ�", WS_SYSMENU | WS_VISIBLE | WS_CHILD, 0, 0, 200, 200, mBaseHwnd, (HMENU)9999, NULL, 0);
	 auto error = GetLastError();
 }

 void BaseWindow::OnNcCalcSize(WPARAM wParam,LPARAM lParam)
 {
 }

 void BaseWindow::OnLButtonDown(WPARAM wParam, LPARAM lParam)
 {
 } 