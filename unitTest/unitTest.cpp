#include "pch.h"
#include <iostream>
#include <thread>
#include "../UIAutomation/UIA.h"
#pragma comment(lib,"../debug/UIAutomation.lib")

int main()
{
	system("Appwiz.cpl");
	HWND findWindow = nullptr;
	auto WaitWindow = [&findWindow]() {
		while (1) {
			if(NULL!=(findWindow = FindWindowA("CabinetWClass", "程序和功能")))
				break;
			Sleep(200);
		}
	};

	std::thread WaitThread(WaitWindow);
	WaitThread.join();

	UIA.SetRoot(findWindow);

	auto find = UIA.GetWindowByClassPath("ShellTabWindowClass/DUIViewWndClassName");
	auto pAE = UIA.GetFirstChildElement(UIA.ElementFromHwnd(find));
	auto strname = UIA.GetElementName(pAE);
	UIA.SetRoot(pAE);
	pAE = UIA.GetLastChildElement(pAE);
	pAE = UIA.GetFirstChildElement(pAE);
	pAE = UIA.GetFirstChildElement(pAE);
	pAE = UIA.GetLastChildElement(pAE);

	strname = UIA.GetElementName(pAE);
	UIA.Invoke(pAE);
}


