
#include "pch.h"
#include <iostream>

#include "../UIAutomation/UIA.h"
#pragma comment(lib,"../debug/UIAutomation.lib")

int main()
{
	system("Appwiz.cpl");
	auto testWindow = FindWindowA("CabinetWClass","程序和功能");
	UIA.SetRoot(UIA.ElementFromHwnd(testWindow));

	UIA.GetElementByAidPath("");
}


