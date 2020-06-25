// pch.cpp: 与预编译标头对应的源文件

#include "预编译头.h"

#pragma comment(lib, "./Detours/lib.X64/detours.lib")

// 当使用预编译的头时，需要使用此源文件，编译才能成功。

void init();
void exit();

// DLL模块启动或退出时候调用
BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
) {
	switch (ul_reason_for_call) {
	case DLL_PROCESS_ATTACH:
		init();
		break;
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
		break;
	case DLL_PROCESS_DETACH:
		exit();
		break;
	}
	return TRUE;
}
