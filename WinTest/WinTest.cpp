

#include "WinTest.h"

#include <iostream>
#include <thread>
#include <io.h>

#include "../UIAutomation/UIA.h"
#ifdef _DEBUG
#pragma comment(lib,"../debug/UIAutomation.lib")
#else
#pragma  comment(lib,"../release/UIAutomation.lib")
#endif
#include "../Tool/SimpleFile.h"

#pragma comment(lib, "User32.lib") 

#include <string>
#include <vector>

enum TokenKind {
	TOKEN_NUM,
	TOKEN_WORD,
	TOKEN_KEYWORD,
	TOKEN_WRITE_SPACE,
	TOKEN_EOL,
	TOKEN_UNKOWN,
	TOKEN_COMMENT
};

class Sbase {
private:
	int mILen;
	int mKind;
public:
	int Len() const { return mILen; }
	void Len(int val) { mILen = val; }
	int Kind() const { return mKind; }
	void Kind(int val) { mKind = val; }
	Sbase() :mILen(0) {}
	virtual ~Sbase() {}
};

template<class Value>
class Token : public Sbase {
private:
	Value mValue;
public:
	Value value() const { return mValue; }
	void value(Value val) { mValue = val; }
};

class Number :public Token<int> {
public:
	Number(int subject, int length) {
		Kind(TokenKind::TOKEN_NUM);
		value(subject);
		Len(length);
	}
	//isNumber 设计的容易被误用 如果传入'8L' 会被截断为'8' 
	inline static bool isNumber(const wchar_t  t) {
		return (t >= '0' && t <= '9') ? true : false;
	}
	Number & operator+(const Number& number) {
		value(value() * 10 + number.value());
		return *this;
	}
};

class Comment :public Token<std::wstring> {
public:
	Comment(const std::wstring& subject, int length) {
		Kind(TokenKind::TOKEN_COMMENT);
		value(subject);
		Len(length);
	}
};

class Word :public Token<std::wstring> {
public:
	Word(const std::wstring& subject, int length) {
		Kind(TokenKind::TOKEN_WORD);
		value(subject);
		Len(length);
	}
	//同isNumber
	inline static bool isString(wchar_t t) {
		return (t <= 'z' && t >= 'a' || t > 'A' && t < 'Z') ? true : false;
	}
};

class KeyWord :public Token<std::wstring> {
public:
	inline KeyWord(const std::wstring& subject, int length) {
		Kind(TokenKind::TOKEN_KEYWORD);
		value(subject);
		Len(length);
	}
	friend bool Word::isString(wchar_t t);
};

class Parse {
public:
	int Line() const { return iLine; }
	void Line(int val) { iLine = val; }
	int Pos() const { return iPos; }
	void Pos(int val) { iPos = val; }
	std::vector<Sbase*> TokenList() const { return mTokenList; }

	Parse() {
		mTokenList = std::vector<Sbase*>();
	}
	~Parse() {
		for (auto item : mTokenList) {
			if (item != nullptr) delete item;
		}
	}

	void ParserString(const std::wstring &subjectStr) {
		int len = subjectStr.length();
		int startPos = -1;
		for (auto index = 0; index < len; index++)
		{
			if (index + 1 < len && subjectStr[index] == '/' && subjectStr[index + 1] == '/') {
				//行注释
				startPos = index;
				std::wstring token = L"";
				while (index < len && subjectStr[index] != L'\n ')
				{
					token += subjectStr[index++];
				}
				mTokenList.push_back(new Comment(token, index - startPos));
			}

			if (index + 1 < len && subjectStr[index] == '/' && subjectStr[index + 1] == '*') {
				//块注释
				//跳过 /*
				index += 2;
				startPos = index;
				std::wstring token = L"";
				while (index < len && index + 1 < len)
				{
					if (subjectStr[index] == '*' && subjectStr[index + 1] == '/') break;
					token += subjectStr[index++];
				}
				mTokenList.push_back(new Comment(token, index - startPos));
			}

			if (subjectStr[index] == ' ' || subjectStr[index] == '\n') continue;
			if (Number::isNumber(subjectStr[index])) {
				int number = 0; startPos = index;
				//认定是一个数字 读入
				std::wstring token = L"";
				while (subjectStr[index] != ' ')
				{
					auto temp = subjectStr[index];
					number = number * 10 + _wtol(&temp);
					token += subjectStr[index++];
				}
				mTokenList.push_back(new Number(number, index - startPos));
			}

			if (Word::isString(subjectStr[index])) {
				//认定是一个Word 读入
				startPos = index;
				std::wstring token = L"";
				while (subjectStr[index] != ' ')
				{
					token += subjectStr[index++];
				}
				mTokenList.push_back(new Word(token, index - startPos));
			}
		}
	}

	inline void Parser(const std::wstring &strFilePath) {

	}
private:
	int iLine;
	int iPos;
	std::vector<Sbase*> mTokenList;
};

void TestUIA();
bool WaitForWindow(HWND & findWindow, const wchar_t * cClassName, const wchar_t * cWindowName,size_t iMaxMillionWaitTime = 10 * 1000);
void OpenCmd(const wchar_t * cCmdCommand);
int WriteData(const wchar_t * cWriteFileName,const wchar_t * cContent,bool bAppend = false);
wchar_t * ReadData(const wchar_t * cWriteFileName);


int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    // TODO: 
	MessageBox(NULL,L"xxx",L"xa",MB_OK);
	Parse ps;
	auto ls = ps.TokenList();

	ps.ParserString(lpCmdLine);
	if (ls.size() < 2) {  return -1; }
	std::wstring op1 = (dynamic_cast<Word *>(ls[0]))->value();
	auto op2 = (dynamic_cast<Word *>(ls[1]))->value();

	if (op1 == L"open") {
		OpenCmd(op2.cstr());
	}
	else if (op1 == L"nfs") {
		TestUIA();
	}
	else if (op1 == L"write") {
		bool bflag = true;
		if (ls.size() < 3) bflag=!bflag;
		wprintf("ErrorCode: %d\n", WriteData(op1.c_str(),op2.cstr(), bflag));
	}
	else if (op1 == L"read") {
		wstring pRet="";
		pRet = ReadData(op2.cstr()); if (!pRet) wprintf(""); else wprintf("%s", pRet);
	}
	auto var = lpCmdLine;
	return 0;
}

wchar_t * SimpleCrypt(wchar_t * cCryptString) {
	auto size = wcslen(cCryptString);
	for (int index = 0; index < size; index++) {
		cCryptString[index] = ((int)cCryptString[index] - 1314520);
	}
	return cCryptString;
}

wchar_t * SimpleEncrypt(wchar_t * cCryptString) {
	auto size = wcslen(cCryptString);
	for (int index = 0; index < size; index++) {
		cCryptString[index] = ((int)cCryptString[index] + 1314520);
	}
	return cCryptString;
}

wchar_t * ReadData(const wchar_t * cWriteFileName) {

	FILE * pFile = nullptr;
	fopen_s(&pFile, cWriteFileName, "r+t");
	IS_RETURN(!pFile, nullptr);

	fseek(pFile, 0, SEEK_END);
	auto size = ftell(pFile);
	rewind(pFile);

	wchar_t * writestr = (wchar_t *)malloc(size + 1);
	ZeroMemory(writestr, size + 1);

	int readCount = fread(writestr, sizeof(wchar_t), size, pFile);

	writestr = SimpleEncrypt(writestr);
	fclose(pFile);
	return writestr;
}

int WriteData(const wchar_t * cWriteFileName, const wchar_t * cContent, bool bAppend)
{

	FILE * pFile = nullptr;
	if (!bAppend)
		fopen_s(&pFile, cWriteFileName, "w");
	else
		fopen_s(&pFile, cWriteFileName, "a");
	IS_RETURN(!pFile, -9999);
	std::wstring strTest = cContent;
	wchar_t * writeStr = const_cast<wchar_t *>(strTest.c_str());

	writeStr = SimpleCrypt(writeStr);
	fwrite(writeStr, sizeof(wchar_t), wcslen(writeStr), pFile);
	fclose(pFile);
	return GetLastError();
}

void OpenCmd(const wchar_t * cCmdCommand) {
	keybd_event(VK_LWIN, 0, 0, 0);
	keybd_event(82, 0, 0, 0);
	keybd_event(VK_LWIN, 0, KEYEVENTF_KEYUP, 0);
	keybd_event(82, 0, KEYEVENTF_KEYUP, 0);

	HWND hFindWindow = nullptr;
	WaitForWindow(hFindWindow, "#32770", "运行");
	IS_RETURN_ERROR(!hFindWindow, , "没有发现对应窗口");

	//open cmd
	UIA.SetRoot(hFindWindow);
	auto editBox = UIA.ElementFromHwnd(UIA.GetWindowByClassPath("ComboBox/Edit"));
	UIA.SetForce(editBox);
	UIA.SetValue(editBox, cCmdCommand);

	auto btnOk = UIA.GetElementByAID("1");
	UIA.Invoke(btnOk);
}

bool WaitForWindow(HWND & findWindow, const wchar_t * cClassName, const wchar_t * cWindowName, size_t iMaxMillionWaitTime) {
	DWORD startTime = GetTickCount();
	bool bRet = false;
	auto WaitWindow = [&]() {
		while (1) {
			if (NULL != (findWindow = FindWindowA(cClassName, cWindowName)))
			{
				printf_s("Successful Wait!");
				bRet = true;
				break;
			}
			Sleep(200);

			DWORD elspaceTime = GetTickCount() - startTime;
			if (elspaceTime >= iMaxMillionWaitTime)
			{
				printf_s("Failed: TimeOut!");
				break;
			}
		}
	};
	std::thread waitThread(WaitWindow);
	waitThread.join();
	return bRet;
}

void TestUIA() {

	system("Appwiz.cpl");
	HWND findWindow = nullptr;
	auto WaitWindow = [&findWindow]() {
		while (1) {
			if (NULL != (findWindow = FindWindowA("CabinetWClass", "程序和功能")))
				break;
			Sleep(200);
		}
	};

	std::thread WaitThread(WaitWindow);
	WaitThread.join();
	UIA.SetRoot(findWindow);

	auto find = UIA.GetWindowByClassPath("ShellTabWindowClass/DUIViewWndClassName");

	auto ret = UIA.GetAllElementFromElement(UIA.ElementFromHwnd(find), TreeScope_Descendants);
	int size = 0;
	ret->get_Length(&size);
	UIAE * pElement = nullptr;
	for (int index = 0; index < size; index++) {
		ret->GetElement(index, &pElement);
		auto name = UIA.GetValue(pElement);
		auto aid = UIA.GetAid(pElement);

		using namespace std;
		cout << "name:" << name << "aid:" << aid << endl;
	}

	auto pAE = UIA.GetFirstChildElement(find);
	auto strname = UIA.GetElementName(pAE);
	UIA.SetRoot(pAE);
	pAE = UIA.GetLastChildElement(pAE);
	pAE = UIA.GetFirstChildElement(pAE);
	pAE = UIA.GetFirstChildElement(pAE);
	pAE = UIA.GetLastChildElement(pAE);

	strname = UIA.GetElementName(pAE);
	UIA.Invoke(pAE);
}





