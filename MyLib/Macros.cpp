#include "Macros.h"
#include <d2d1.h>
#include <utility> //pair
#include <memory>

void ErrorMessage(const char * _error)
{
	MessageBox(NULL, _error, "error", MB_OK);
	exit(0);
}

HCURSOR ArrowShape::SHAPE_ARROW = LoadCursor(NULL, IDC_ARROW);
HCURSOR ArrowShape::SHAPE_I = LoadCursor(NULL, IDC_IBEAM);
HCURSOR ArrowShape::SHAPE_CROSS = LoadCursor(NULL, IDC_CROSS);

namespace MutexLock
{
	mutex ReadWriteLock;
}

namespace Conver
{

	D2D1_POINT_2F  PointToD2DPointF(POINT & pt)
	{
		return D2D1_POINT_2F(
			D2D1::Point2F(STCAST(float, pt.x), STCAST(float, pt.y)));
	}

	D2D1_RECT_F  RectToD2DRectF(RECT & rc)
	{
		return D2D1_RECT_F(
			D2D1::RectF(STCAST(float, rc.left),
				STCAST(float, rc.top),
				STCAST(float, rc.right),
				STCAST(float, rc.bottom)));
	}

	bool PointInRect(int x, int y,RECT rc)
	{
		if (x<rc.right && x >rc.left && y > rc.top && y < rc.bottom) return true;
		return false;
	}

	bool RectInRect(RECT smallRect, RECT bigRect)
	{
		POINT lp = {smallRect.left,smallRect.top};
		POINT rb = {smallRect.right,smallRect.bottom};
		if (PointInRect(lp.x, lp.y, bigRect) && PointInRect(rb.x, rb.y, bigRect)) return false;
		return true;
	}

	bool ZeroRect(RECT targetRect)
	{
		if (targetRect.left == 0 && targetRect.right == 0
			&& targetRect.right == 0 && targetRect.bottom == 0) return true;
		return false;
	}

	bool VaildRect(RECT targetRect)
	{
		IS_RETURN_ERROR(ZeroRect(targetRect),false,"�����");
		if (targetRect.left > targetRect.right || targetRect.top > targetRect.bottom) return false;
		return true;
	}

	RECT ClipRectBoard(RECT srcRect, UINT dx, UINT dy)
	{
		return MyRect(srcRect.left + dx, srcRect.top + dy, srcRect.right - dx, srcRect.bottom - dy);
	}


	RECT SubRect(RECT srcRect, RECT desRect)
	{
		RECT tempRect = {srcRect.left-desRect.left,srcRect.top-desRect.top,srcRect.right-desRect.right,
		srcRect.bottom-desRect.bottom};
		return tempRect;
	}

	RECT ClipRectBoardEx(RECT srcRect, LONG lLeft, LONG lTop, LONG lRight, LONG lBottom)
	{
		RECT tempRC = { srcRect.left - lLeft ,srcRect.top - lTop ,
			srcRect.right - lRight,srcRect.bottom - lBottom };
		return tempRC;
	}

	RECT  RectOffSet(RECT srcRect, LONG lLeft, LONG lTop, LONG lRight, LONG lBottom)
	{
		srcRect = { srcRect.left + lLeft, srcRect.top + lTop,
			srcRect.right + lRight, srcRect.bottom + lBottom };
		return srcRect;
	}

	RECT GetMaxSizeRect()
	{
		RECT srcRect = {0,0,SCREEN_WIDTH,SCREEN_HEIGHT};
		auto ret = FindWindowA("Shell_TrayWnd",NULL);
		if (!ret) return srcRect;
		RECT desRect;
		BOOL bOk = GetWindowRect(ret,&desRect);
		IS_RETURN_ERROR(!bOk, RECT{}, "GetMaxSizeRect����");
		desRect = ClipRectBoardEx(srcRect, 0, 0, 0, RECTHEIGHT(desRect));
		return desRect;
	}

	wstring GetDirName(wstring path)
	{
		unsigned int pos1 =0 ,pos2 = 0;
		pos1 = path.rfind(L'/');
		pos2 = path.rfind(L'\\');
		if (pos2 < pos1) pos1 = pos2;
		if (pos1 == string::npos && pos2 == string::npos) pos1 = 0;

		return path.substr(0, pos1 + 1);
	}

	RECT  ClientToScreenRc(HWND hWnd,RECT & rc)
	{
		POINT lt = { rc.left,rc.top }, rt = {rc.right,rc.bottom};
		ClientToScreen(hWnd, &lt);
		ClientToScreen(hWnd, &rt);
		RECT tempRect = { lt.x,lt.y,rt.x,rt.y };
		rc = { lt.x,lt.y,rt.x,rt.y };
		return { lt.x,lt.y,rt.x,rt.y };
	}

	RECT  ScreenToClientRc(HWND hWnd, RECT & rc)
	{
		POINT lt = { rc.left,rc.top }, rt = { rc.right,rc.bottom };
		ScreenToClient(hWnd, &lt);
		ScreenToClient(hWnd, &rt);
		RECT tempRect = { lt.x,lt.y,rt.x,rt.y };
		rc = tempRect;
		return tempRect;
	}
	
	string Format(char * format, ...)
	{
		va_list va;
		va_start(va, format);
		char buf[256];
		vsprintf_s(buf, format, va);
		va_end(va);
		return buf;
	}

	string Format(const char * format, ...)
	{
		va_list va;
		va_start(va, format);
		char buf[256];
		vsprintf_s(buf, format, va);
		va_end(va);
		return buf;
	}

	wstring Format(wchar_t * format, ...)
	{
		va_list va;
		va_start(va, format);
		wchar_t buf[256];
		vswprintf_s(buf,256,format,va);
		va_end(va);
		return buf;
	}

	POINT  CenterPoint(RECT rc,int offsetX,int offsetY)
	{
		POINT pt = {rc.left + (rc.right - rc.left)/2 - offsetX/2,
			rc.top + (rc.bottom - rc.top)/2 - offsetY/2 };
		return pt;
	}

	POINT LeftTopPoint(RECT rc)
	{
		return Point(rc.left,rc.top);
	}

	POINT TopCenterPoint(RECT rc)
	{
		return Point(rc.left + (rc.right - rc.left) / 2, rc.top);
	}

	POINT BottomCenterPoint(RECT rc,int height)
	{
		if (height == -1) height = rc.bottom;
		return Point(rc.left + (rc.right - rc.left) / 2, height);
	}


	POINT LeftCenterPoint(RECT rc)
	{
		POINT pt = { rc.left ,rc.top + (rc.bottom - rc.top) / 2  };
		return pt;
	}

	POINT RightCenterPoint(RECT rc)
	{
		POINT pt = { rc.right ,rc.top + (rc.bottom - rc.top) / 2 };
		return pt;
	}

	POINT RightBottomPoint(RECT rc)
	{
		POINT pt = {rc.right,rc.bottom};
		return pt;
	}

	POINT LeftBottomPoint(RECT rc)
	{
		POINT pt = { rc.left,rc.bottom };
		return pt;
	}

	POINT RightTopPoint(RECT rc)
	{
		POINT pt = { rc.right,rc.top };
		return pt;
	}

	char * WCharToAChar(wchar_t * Wchar, UINT codePage)
	{
		auto size = WideCharToMultiByte(codePage,0,Wchar,-1,NULL,0,NULL,NULL);
		auto TheCoveredString = new char[size]();
		auto ret  = WideCharToMultiByte(codePage, 0, Wchar, -1, TheCoveredString, size, NULL, NULL);
		if (!ret) { SAFE_DELETE(TheCoveredString); return nullptr; }
		TheCoveredString[size] = '\0';
		return TheCoveredString;
	}

	string WCharToAChar(wstring Wchar, UINT codePage)
	{
		return WCharToAChar(COCAST(wchar_t *,Wchar.c_str()),codePage);
	}

	char * WCharToAChar(const wchar_t * Wchar, UINT codePage /*= CP_UTF8*/)
	{
		return WCharToAChar(COCAST(wchar_t *,Wchar),codePage);
	}

	wchar_t * ACharToWChar(char * Achar, UINT codePage)
	{
		auto size = MultiByteToWideChar(codePage, 0, Achar, -1, NULL, 0);
		auto TheCoveredString = new wchar_t[size]();
		auto ret = MultiByteToWideChar(codePage, 0, Achar, -1, TheCoveredString, size);
		if (!ret) { SAFE_DELETE(TheCoveredString); return nullptr; }
		TheCoveredString[size] = '\0';
		return TheCoveredString;
	}

	wstring ACharToWChar(string Achar, UINT codePage)
	{
		return ACharToWChar(COCAST(char *, Achar.c_str()), codePage);
	}

	wchar_t * ACharToWChar(const char * Achar, UINT codePage /*= CP_UTF8*/)
	{
		return ACharToWChar(COCAST(char *, Achar), codePage);
	}

	int utoi(wchar_t * Wchar)
	{
		return atoi(WCharToAChar(Wchar));
	}

	int utoi(const wchar_t * WChar)
	{
		return utoi(COCAST(wchar_t*,WChar));
	}

	int utoi(wstring wStr)
	{
		return utoi(wStr.c_str());
	}

	bool utob(wchar_t * Wchar)
	{
		if (atoi(WCharToAChar(Wchar)) == 0) return false;
		return true;
	}

	bool utob(const wchar_t * WChar)
	{
		return utob(COCAST(wchar_t*, WChar));
	}

	bool utob(wstring wStr)
	{
		return utob(wStr.c_str());
	}

	float utof(wchar_t * Wchar)
	{
		return STCAST(float,atof(WCharToAChar(Wchar)));
	}

	float utof(const wchar_t * WChar)
	{
		return utof(COCAST(wchar_t*, WChar));;
	}

	float utof(wstring wStr)
	{
		return utof(wStr.c_str());
	}

	MyRect::MyRect(INT left, INT top, INT right, INT bottom):mLeft(left),mRight(right),
		mTop(top),mBottom(bottom)
	{
		mRect = {mLeft,mTop,mRight,mBottom};
	}

	MyRect::MyRect(RECT desRect):mRect(desRect)
	{
	}

	MyRect::MyRect(POINT pointLeftTop, POINT pointRightBottom): MyRect(pointLeftTop.x,pointLeftTop.y,pointRightBottom.x,pointRightBottom.y)
	{

	}

	RECT MyRect::Rect() const
	{
		return mRect;
	}

	MyRect  MyRect::operator-(MyRect & rect)
	{
		return MyRect(mLeft-rect.mLeft,mTop-rect.mTop,
			mRight - rect.mRight, mBottom - rect.mBottom);
	}

	MyRect::operator RECT&()
	{
		return mRect;
	}

	MyRect::operator D2D1_RECT_F()
	{
		return RectToD2DRectF(*this);
	}


	Point Point::operator=(POINT & pt)
	{
		Point tempPoint = {pt.x,pt.y};
		return tempPoint;
	}

	bool Point::operator==(Point pt)
	{
		return pt.x == x && pt.y == y;
	}

	Point::operator POINT()
	{
		POINT pt = {x,y};
		return pt;
	}

	Point Point::operator-(POINT pt)
	{
		return Point(x-pt.x,y-pt.y);
	}

	Point Point::operator+(POINT pt)
	{
		return Point(x + pt.x, y + pt.y);
	}

	Point::Point(int _x, int _y):x(_x),y(_y)
	{

	}

	Point::Point(POINT pt):x(pt.x),y(pt.y)
	{
	}

	Point::Point() : Point(0,0)
	{

	}
}


