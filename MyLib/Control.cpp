#include "Control.h"
#include <algorithm>

namespace LIB_CONTROL
{
	UINT Handle::ID = 678 + 1;

	void Handle::UpdatePosition()
	{
		MoveWindow(mHwnd, mX, mY, mWidth, mHeight, true);
	}
	void Handle::UpdateCache()
	{
		SetWindowPos(mHwnd,HWND_TOP,mX,mY,mWidth,
			mHeight,SWP_FRAMECHANGED);
	}

	Handle::Handle():mX(0),mY(0),mWidth(),mHeight(),mStyle(WS_CHILD|WS_VISIBLE),mClassName("BUTTON")
	{

	}

	bool Handle::Create()
	{
		mID = ID++;
		mHwnd = CreateWindowA(mClassName.c_str(), mText.empty()?"hahaha":mText.c_str(), mStyle, mX, mY, 
			mWidth, mHeight, mParent, HMENU(mID), NULL,NULL);
		if (!mHwnd) return false;
		return true;
	}

	UINT Handle::GetID() const
	{
		return mID;
	}

	HWND Handle::GetOwn() const
	{
		return mHwnd;
	}

	void Handle::SetPosition(int x, int y)
	{
		mX = x, mY = y;
		if (mHwnd) UpdatePosition();
	}

	void Handle::SetWidth(int width)
	{
		mWidth = width;
		if (mHwnd) UpdatePosition();
	}

	void Handle::SetHeight(int height)
	{
		mHeight = height;
		if (mHwnd) UpdatePosition();
	}

	void Handle::AddStyle(DWORD style)
	{
		if (mHwnd)
		{
			mStyle = mStyle | style;
			auto tempStyle = GetWindowLong(mHwnd,GWL_STYLE);
			SetWindowLong(mHwnd,GWL_STYLE,tempStyle|style);
			UpdateCache();
		}
	}

	void Handle::ReduceStyle(DWORD style)
	{
		if (mHwnd)
		{
			mStyle = mStyle & ~style;
			auto tempStyle = GetWindowLong(mHwnd, GWL_STYLE);
			SetWindowLong(mHwnd, GWL_STYLE, tempStyle &  ~style);
			UpdateCache();
		}
	}

	bool Handle::AttachParent(HWND parent)
	{
		if (!parent) return false;
		else
			mParent = parent;
		return true;
	}


	LRESULT CALLBACK ControlProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		switch (uMsg)
		{
		case WM_DRAWITEM:
		case WM_COMMAND:

		default:
			break;
		}
		return DefWindowProc(hWnd,uMsg,wParam,lParam);
	}


	bool View::IsVisible()
	{
		if (!mHwnd) return false;
		if (GetWindowLong(mHwnd, GWL_STYLE)&WS_VISIBLE) return true;
		return false;
	}

	bool View::SetVisible(bool CanSee)
	{
		if (!mHwnd) return false;
		auto style = GetWindowLong(mHwnd, GWL_STYLE);
		if(!CanSee) SetWindowLong(mHwnd, GWL_STYLE, style &~ WS_VISIBLE);
		else
		SetWindowLong(mHwnd, GWL_STYLE, style | WS_VISIBLE);
		return true;
	}

	void View::SetText(string st)
	{
		if (mHwnd) SetWindowText(mHwnd, st.c_str());
	}


	Control::Control(string Type)
	{
		mClassName = Type.c_str();
	}

	Control::Control():Handle()
	{

	}

	void Control::OnDraw(WPARAM wParam, LPARAM lParam)
	{

	}
	void Control::OnClick(WPARAM wParam, LPARAM lParam)
	{

	}

	bool Control::CreateObject(int x, int y,int width,int height)
	{
		mWidth = width;
		mHeight = height;
		if (mClassName.empty()) return false;
		mX = x;
		mY = y;
		return Handle::Create();
	}

}
