# pragma once

#include "MyLayout.h"

namespace LIB_WINDOW
{
	namespace MOUSE
	{
		class Mouse
		{
		public:

			int x,y;
			
			UINT mMouseState;

			Mouse();
		};
	}

using namespace LIB_CONTROL;

class  BaseWindow
{
protected:
	int mWidth;

	int mHeight;

	bool mIsfullscreen;

	bool mDefaultWrapper;

	RECT mWindowRect;

	wstring mClassname; 

	wstring mWindowname;

	DWORD mWindowStyleEx;

	DWORD mWindowStyle;

	HINSTANCE mInstance;

	Conver::Point mLeftTop;

	MOUSE::Mouse mMouse;

	HDC  mHdc;

	HWND mBaseHwnd; 

	string mLayoutFileName;

	pCallBackFunc mCallBackFunc;

protected:
	 
	RECT Rect() const;

	bool SetRect(RECT desRect);

	void MoveCenterWindow();

	bool ShowThisWindow(void); //show this window

	void UpdateSize()const;

	void UpdateCache(bool topMost);

	void UpdatePosition(WPARAM wParam,LPARAM lParam);

public:
	Listener mListener;

	BaseWindow();

	BaseWindow(string layoutFileName);

	BaseWindow(int width, int height,const wstring windowname, const wstring classname, 
		DWORD WindowStyleEx, DWORD WindowStyle, POINT leftUpper);

	virtual ~BaseWindow();

	void ReDraw(bool bReDraw); 	//WM_PAINT

	HANDLE Show();

	void UseWrapper();

	bool IsDefaultWrapper();

	bool Close();

	void SetCallBackFunc(pCallBackFunc mFunc);

	void SetInstance(HINSTANCE hInstance);

	void SetWidth(int Width);

	void SetHeight(int Height);

	void SetWindowName(const wstring windowname);

	void AddWindowStyle(DWORD WindowStyle);

	void ReduceWindowStyle(DWORD WindowStyle);

	void AddWindowStyleEx(DWORD WindowStyleEx);

	void ReduceWindowStyleEx(DWORD WindowStyleEx);

	void SetLeftTopPos(POINT leftUpper);

	void SetWindowAlpha(int alpha); //窗口透明度 0~255 透明度

	int GetWidth(void)const;

	int GetHeight(void)const;

	wstring GetWindowName(void)const;

	HDC  GetHDC(void)const;

	HWND GetHwnd(void)const;
	
	virtual void Destory();

	virtual void MessageLoop();

	virtual void Update(float delta);

	virtual void InitBeforeCreate();

	virtual void AfterCreate();

	virtual void OnDraw();	//default draw operate

	virtual void OnCreate();

	virtual bool OnSetCursor(WPARAM wParam,LPARAM lParam); //set cursor event

	virtual void OnNcPaint(HRGN rgn);	///<rgn>重绘区域</rgn>

	virtual void OnNcCalcSize(WPARAM wParam,LPARAM lParam);

	virtual void OnLButtonDown(WPARAM wParam,LPARAM lParam);

	virtual void OnLButtonUp(WPARAM wParam, LPARAM lParam);

	virtual UINT OnUnicodeChar(WPARAM wParam, LPARAM lParam); /// WM_IME_CHAR 

	virtual void OnMouseMove(WPARAM wParam, LPARAM lParam);

	virtual bool OnNcActive(WPARAM wParam, LPARAM lParam);

	virtual UINT OnSize(WPARAM wParam,LPARAM lParam);

	virtual bool OnSizing(WPARAM wParam, LPARAM lParam);

	virtual UINT OnHitTest(LPARAM lParam);

	operator HDC()  {return mHdc;}

	operator HWND() {return mBaseHwnd;}

	friend
	LRESULT CALLBACK WinProc(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam);

	//Control Function
	void AddClickFuncByID(UINT id, function<void(void)> pFunc, bool rewrite = false);

	Control * findElementByID(UINT id);

	bool SetValueByID(UINT id, wstring stringValue);

	wstring getValueByID(UINT id);

	void SetRangeIDValue(UINT startId, UINT endId, wstring stringValue);
};
}


using namespace LIB_WINDOW;
