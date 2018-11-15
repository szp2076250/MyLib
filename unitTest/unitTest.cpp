#include "pch.h"
#include <iostream>
#include <thread>
#include "../UIAutomation/UIA.h"
#pragma comment(lib,"../debug/UIAutomation.lib")
#include "../Tool/SimpleFile.h"

void TestUIA();
void TestMount();
bool WaitForWindow(HWND & findWindow, const char * cClassName, const char * cWindowName);
void OpenCmd(const char * cCmdCommand);

int main(int argc,char* argv[])
{
	if (argc <= 2)
	{
		printf("error usage!");
		return -1;
	}

	for (int index = 0; index < argc; index++) {
		printf(argv[index]);
	}

	if (strcmp(argv[1],"0")==0) TestUIA();
	else if (strcmp(argv[1],"1")) OpenCmd("umount X:");
	else if (strcmp(argv[1],"2")) OpenCmd(argv[2]);
	else
		printf("error usage!");

	return 0;
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

bool WaitForWindow(HWND & findWindow,const char * cClassName,const char * cWindowName) {
	auto WaitWindow = [&findWindow,&cClassName,&cWindowName]() {
		while (1) {
			if (NULL != (findWindow = FindWindowA(cClassName, cWindowName)))
				break;
			Sleep(200);
		}
	};
	std::thread waitThread(WaitWindow);

	waitThread.join();
	return true;
}

void TestMount() {
	WinExec("cmd",SW_NORMAL);
	WinExec("unitTest.bat", SW_NORMAL);
	system("C:");
	system("cd C:\\Windows\\System32");
	system("umount X:");
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


