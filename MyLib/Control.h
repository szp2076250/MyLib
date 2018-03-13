#pragma once
#include <list>
#include <algorithm>
#include "2Draw.h"
#include "Sprite.h"
#include "Caret.h"

namespace LIB_CONTROL
{
	using namespace std;

	using namespace MyMessage;

	class Listener;

	class ImageAdapter
	{
	protected:
		IPIC * pImage;
		
		bool bDrawImage;

		RECT mImgRec;

	public:

		bool SetImage(IPIC * img);

		bool LoadFromFile(wstring strFileName);

		void SetImgRect(RECT desRect);

		RECT ImgRect() const; 

		ImageAdapter();

		~ImageAdapter();
	};

	class DragAdapter
	{
	protected:

		bool mCanDrag; //Drag

		bool mbDraging; //Draging

		int mLeft, mTop;

		virtual void Drag(Listener * pListener, int dx, int dy) = 0; ///dx dy 偏移

	public:
		bool IsDraging();

		void Stop();

		void SetDrag(bool DragState);

		DragAdapter();
	};



	class Control: public ImageAdapter,public DragAdapter
	{

	protected:

		RECT mRect;

		string mText;

		COLORREF mForceColor, mBackColor;

		COLORREF mHonverBackColor, mHoverForceColor;

		bool mVisible;

		bool mMouseInternal;

		bool mActive;

		bool mCanStretch; //Stretch

		UINT mControlTypeId;

		bool mBDownInternal;

		void SetID(UINT typeId);

		POINT mouseDragStartPoint;

		bool mOwnCaret;

		POINT mCaretPos; 

		TextLayout * mpTextpLayout;

	public:

		virtual void Hover(Listener * pListener, POINT pt) = 0;

		virtual UINT HitTest(Listener * pListener, POINT pt) = 0;

		virtual void Draw(Listener * pListener);

		virtual UINT LButtonDown(Listener * pListener,POINT pt);

		virtual UINT LButtonUp(Listener * pListener,POINT pt);

		virtual void MouseMove(Listener * pListener,POINT pt); //mouse move ///pt是关于Client的

		virtual void InputChar(Listener * pListener,char cAscii);

		void Drag(Listener * pListener, int dx, int dy) override;

		virtual void Focus(Listener * pListener,HWND hWnd);

		virtual void KillFocus(Listener * pListener);

		virtual void Sizing(RECT newRect);

		bool Stretch(); //是否是一个随着窗口大小变化而改变的控件

		UINT TypeId() const;

		bool Active() const;

		void SetActive(bool bActive);

		string Text() const;

		void SetText(string text);

		COLORREF ForceColor() const;

		void SetForceColor(COLORREF color);

		COLORREF BackColor() const;

		void SetBackColor(COLORREF color);

		COLORREF HoverBackColor() const;

		void SetHoverBackColor(COLORREF color);

		COLORREF HoverForceColor() const;

		void SetHoverForceColor(COLORREF color);

		bool IsVisible() const;

		void SetVisible(bool state);

		RECT getRect() const;

		void AdjustRect(RECT decREct);

		bool IsMouseInteral() const;

		void SetInternal(bool MouseInternal);

		LONG width() const;

		LONG height() const;

		Control();

		Control(string text, RECT rc = {0,0});

		Control(string text, RECT rc,COLORREF forceColor, COLORREF backColor, COLORREF hoverForceColor, COLORREF hoverBackColor);

		virtual ~Control();
	};

	
	class Listener
	{
	protected:

		list<Control *> mpControl;

		HWND mListenedWindow;

	public:

		bool attachWindow(HWND hWnd);

		bool attach(Control * pControl);

		bool detach(Control * pControl);

		void Draw();

		void InputChar(char c);

		void Hover(POINT pt);

		UINT LButtonDown(POINT pt);

		UINT LButtonUp(POINT pt);

		UINT HitTest(POINT pt);

		void MouseMove(POINT pt);

		void ChangeSize(RECT newRect);

		list<Control *> & Obj();
	};

	///<Button>按钮</Button>
	class Button : public Control
	{
	protected:
		COLORREF mBoardColor;

		bool mDrawBoard;

	public:

		void SetButtonDownInternal(bool isDownInternal);

		void SetBoardColor(COLORREF cBoardColor);

		void NeedDrawBoard(bool bDrawBoard);

		virtual void Draw(Listener * pListener) override;

		virtual void Hover(Listener * pListener, POINT pt) override;

		virtual UINT HitTest(Listener * pListener,POINT pt) override;

		Button();

		Button(string text,RECT rc);

		Button(string text, RECT rc, COLORREF forceColor, COLORREF backColor, COLORREF hoverForceColor, COLORREF hoverBackColor);
	};

	class CloseButton : public Button 
	{
	public:

		void Draw(Listener * pListener) override final;

		void Hover(Listener * pListener, POINT pt) override final;

		UINT LButtonUp(Listener * pListener,POINT pt) override final;

		UINT HitTest(Listener * pListener, POINT pt) override final;

		void Sizing(RECT newRect) override;

		CloseButton();

		CloseButton(string text, RECT rc, COLORREF forceColor, COLORREF backColor, COLORREF hoverForceColor, COLORREF hoverBackColor);

	};

	class MiniButton : public Button
	{
	public:

		void Draw(Listener * pListener) override final;

		UINT LButtonUp(Listener * pListener,POINT pt) override final;

		void Sizing(RECT newRect) override;

		MiniButton();

		MiniButton(string text, RECT rc, COLORREF forceColor, COLORREF backColor, COLORREF hoverForceColor, COLORREF hoverBackColor);
	};

	class MaxButton : public Button // max and restore button...
	{
	protected:

		void DrawMaxImg();

		void DrawRestoreImg();

		bool isMax;

	public:

		void setState(bool isMax);

		void Draw(Listener * pListener) override final;

		UINT LButtonUp(Listener * pListener,POINT pt) override final;

		void Sizing(RECT newRect) override;

		MaxButton();

		MaxButton(string text, RECT rc, COLORREF forceColor, COLORREF backColor, COLORREF hoverForceColor, COLORREF hoverBackColor);
	};

	///<ImageButton>图形按钮</ImageButton>
	class ImageButton : public Button
	{
		IPIC * pHonverImage;

	public:

		void Draw(Listener * pListener) override;

		ImageButton(wstring picLoc,RECT rImgRect);

		ImageButton(IPIC * pBitmap, RECT rImgRect);
	};

	///<TitleBar>标题栏</TitleBar>
	class TitleBar : public Control
	{
		Sprite::Sprite mIconSprite;

	public:
		void Draw(Listener * pListener) override;

		void Hover(Listener * pListener, POINT pt) override;

		UINT HitTest(Listener * pListener, POINT pt) override;

		void Sizing(RECT newRect) override final;

		TitleBar(string text, RECT rc);

		TitleBar(string text, RECT rc, COLORREF forceColor, COLORREF backColor, COLORREF hoverForceColor, COLORREF hoverBackColor);

		~TitleBar();
	};

	///<EditBox>编辑框</EditBox>
	class EditBox : public Control
	{
		float mFrontSize;

		string mFrontName;

	public:

		void ChangeFrontSize(float newSize);

		void ChangeFrontName(string newFrontName);

		void Draw(Listener * pListener) override;

		void Hover(Listener * pListener, POINT pt) override;

		UINT HitTest(Listener * pListener, POINT pt) override;

		EditBox(RECT rc, string defaultText = "");

		EditBox(string defaultText, RECT rc, COLORREF forceColor, COLORREF backColor, COLORREF hoverForceColor, COLORREF hoverBackColor);

	};

	///<ComposeControl>组合控件</ComposeControl>
	class ComposeControl :public Control
	{
	protected:

		list<ComposeControl *>  mControl; //mControl 所有坐标相对于 最外面的包围矩形

		Listener * pmListener;

	public:

		bool Attach(Listener * pListener);

		bool Add(Control * pControl);

		virtual void Draw(Listener * pListener);

		virtual UINT HitTest(POINT pt);

		virtual void Hover(POINT pt);

	};
}



