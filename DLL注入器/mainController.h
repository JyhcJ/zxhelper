#pragma once
#include <windows.h>



// 自定义中%s 和 %S一样的
void Call_输出调试信息(char* pszFormat, ...);

void Call_输出调试信息W(const wchar_t* pszFormat, ...);

DWORD __stdcall PipeThread(LPVOID lpParam);

bool cereatePipeAndGetProHandle(HWND handle);

BOOL injectDLL(HANDLE processHandle, const char* dll);

bool UnloadInjectedDLL(DWORD pid, const wchar_t* dllName);

HMODULE GetRemoteModuleHandle(DWORD pid, const wchar_t* moduleName);

void PrintModules(DWORD pid);
