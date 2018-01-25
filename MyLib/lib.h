# pragma once
# include <stdio.h>
# include <cstdlib>
# include <windows.h>

# ifdef INCLUDE_MY_DX9
# include "libD3D.h"
# endif

#define SCREEN_WIDTH  GetDeviceCaps(GetDC(NULL),HORZRES)
#define SCREEN_HEIGHT GetDeviceCaps(GetDC(NULL),VERTRES)
#define MAX_KEYS 256
# define SAFE_RELEASE(p) if(p){p->Release(); p=NULL;}

extern void ErrorMessage(const char * _error);

#define SINGLE_INSTANCE(classname) \
		private: \
			classname(){} \
		public:\
		inline static classname & getInstance(){\
			static classname  cInstance;\
			return cInstance;}

#define IS_RETURN_ERROR(condition,returnValue,error_message)\
		if(condition)\
		{\
		OutputDebugStringA("[ERROR]------------------------------------");\
		OutputDebugStringA(error_message);\
		OutputDebugStringA("\n");\
		return returnValue;}
		

#define IS_RETURN_FUNC(condition,returnValue,FUNC)\
		if(condition)\
		{\
		FUNC();\
		return returnValue;}
		

typedef LRESULT(_stdcall *pCallBackFunc)(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

class  BaseWindow
{
private:
	typedef struct { int x; int y; }Point;


	int mWidth;
	int mHeight;
	bool mIsfullscreen;
	const char * mClassname; 
	const char * mWindowname;

	DWORD mWindowStyleEx;
	DWORD mWindowStyle;
	HINSTANCE mInstance;
	HBRUSH mBackground;

	POINT mLeftTop;

	HDC  mHdc;
	HWND mBaseHwnd; 

	pCallBackFunc mCallBackFunc;

	void MoveCenterWindow();

public:

	//InvalidRect
	void ReDraw();

	BaseWindow();

	virtual ~BaseWindow();

	BaseWindow(int width, int height, bool isfullscreen, const char * windowname, const char * classname, DWORD WindowStyleEx, DWORD WindowStyle, void * lpvoid, Point leftUpper)
		:mWidth(width), mHeight(height), mIsfullscreen(isfullscreen), mWindowname(windowname), mClassname(classname),
		mWindowStyle(WindowStyle), mWindowStyleEx(WindowStyleEx),mCallBackFunc(NULL){}

	bool ShowThisWindow(void); //show this window

	void SetCallBackFunc(pCallBackFunc mFunc);
	void SetInstance(HINSTANCE hInstance) {mInstance = hInstance;}
	void SetWidth(int Width) {if(Width>0|| Width<SCREEN_WIDTH){mWidth = Width;}else{mWidth = 1024;}}
	void SetHeight(int Height) {if(Height>0|| Height<SCREEN_HEIGHT){mHeight = Height;}else{mHeight = 768;}}
	void SetWindowName(const char * windowname){mWindowname = windowname;}
	void SetClassName(const char * classname){mClassname = classname;}
	void SetWindowStyle(DWORD WindowStyle){mWindowStyle=WindowStyle;}
	void SetWindowStyleEx(DWORD WindowStyleEx){mWindowStyleEx = WindowStyleEx;}
	void SetLeftTopPos(POINT leftUpper){mLeftTop = leftUpper;}
	void SetBrush(const HBRUSH & bs) {mBackground = bs;}

	int GetWidth(void) {return mWidth;}
	int GetHeight(void) {return mHeight;}
	const char * GetWindowName(void){return mWindowname;}
	
	bool SetHDC(HDC hDC){mHdc = hDC; return hDC?true:false;}
	HDC  GetHDC(void){return mHdc;}
	HWND GetHwnd(void){return mBaseHwnd;}
	
	virtual void Destory();
	virtual void MessageLoop();
	virtual void Update(float delta);
	virtual void InitBeforeCreate();
	virtual void AfterCreate();

	virtual void OnDraw();	//default draw operate
	virtual void OnCreate();
	virtual void OnNcPaint(WPARAM wParam);
	virtual void OnNcCalcSize(WPARAM wParam,LPARAM lParam);
	virtual void OnLButtonDown(WPARAM wParam,LPARAM lParam);

	operator HDC()  {return mHdc;}
	operator HWND() {return mBaseHwnd;}

	friend 	
		LRESULT CALLBACK WinProc(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam);
};

