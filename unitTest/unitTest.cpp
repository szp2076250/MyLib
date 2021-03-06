#include "pch.h"
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



int main(int argc,char* argv[])
{
	if (argc <= 2)
	{
		printf("error usage!");
		return -1;
	}
	
	if (strcmp(argv[1], "0") == 0) TestUIA();
	else if (strcmp(argv[1], "1")==0) OpenCmd("umount X:");
	else if (strcmp(argv[1], "2")==0) OpenCmd(argv[2]);
	else if (strcmp(argv[1], "3")==0) printf("ErrorCode: %d\n",WriteData(argv[2],argv[3],argc>4?atoi(argv[4]):false));
	else if (strcmp(argv[1], "4") == 0) { 
		auto pRet = ReadData(argv[2]); if (!pRet) printf(""); else printf(pRet);
	}
	else
		printf("error usage!");

	return 0;
}

char * SimpleCrypt(char * cCryptString) {
	auto size = strlen(cCryptString);
	for (int index = 0; index < size; index++) {
		cCryptString[index] = ((int)cCryptString[index] - 1314520);
	}
	return cCryptString;
}

char * SimpleEncrypt(char * cCryptString) {
	auto size = strlen(cCryptString);
	for (int index = 0; index < size; index++) {
		cCryptString[index] = ((int)cCryptString[index] + 1314520);
	}
	return cCryptString;
}

char * ReadData(const char * cWriteFileName) {
	if (_access(cWriteFileName, 0) == -1)
		return nullptr;
	FILE * pFile = nullptr;
	fopen_s(&pFile, cWriteFileName, "r+t");
	IS_RETURN(!pFile, nullptr);

	fseek(pFile, 0, SEEK_END);
	auto size = ftell(pFile);
	rewind(pFile);

	char * writestr = (char *)malloc(size+1);
	ZeroMemory(writestr, size+1);

	int readCount = fread(writestr, sizeof(char), size, pFile);
	
	writestr = SimpleEncrypt(writestr);
	fclose(pFile);
	return writestr;
}

int WriteData(const char * cWriteFileName, const char * cContent,bool bAppend)
{
	if (_access(cWriteFileName, 0) != -1 && !bAppend)
		remove(cWriteFileName);
	FILE * pFile = nullptr;
	if (!bAppend)
		fopen_s(&pFile, cWriteFileName, "w");
	else 
		fopen_s(&pFile,cWriteFileName,"a");
	IS_RETURN(!pFile,-9999);
	std::string strTest = cContent;
	char * writeStr = const_cast<char *>(strTest.c_str());

	writeStr = SimpleCrypt(writeStr);
	fwrite(writeStr,sizeof(char), strlen(writeStr), pFile);
	fclose(pFile);
	return GetLastError();
}

void OpenCmd(const char * cCmdCommand) {
	keybd_event(VK_LWIN,0,0,0);
	keybd_event(82, 0, 0, 0);
	keybd_event(VK_LWIN, 0, KEYEVENTF_KEYUP, 0);
	keybd_event(82, 0, KEYEVENTF_KEYUP, 0);

	HWND hFindWindow = nullptr;
	WaitForWindow(hFindWindow,"#32770","运行");
	IS_RETURN_ERROR(!hFindWindow,,"没有发现对应窗口");

	//open cmd
	UIA.SetRoot(hFindWindow);
	auto editBox = UIA.ElementFromHwnd(UIA.GetWindowByClassPath("ComboBox/Edit"));
	UIA.SetForce(editBox);
	UIA.SetValue(editBox, cCmdCommand);

	auto btnOk = UIA.GetElementByAID("1");
	UIA.Invoke(btnOk);
}

bool WaitForWindow(HWND & findWindow,const char * cClassName,const char * cWindowName,size_t iMaxMillionWaitTime ) {
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


