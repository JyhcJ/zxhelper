#pragma once
#include <windows.h>



// �Զ�����%s �� %Sһ����
void Call_���������Ϣ(char* pszFormat, ...);

void Call_���������ϢW(const wchar_t* pszFormat, ...);

DWORD __stdcall PipeThread(LPVOID lpParam);

bool cereatePipeAndGetProHandle(HWND handle);

BOOL injectDLL(HANDLE processHandle, const char* dll);

bool UnloadInjectedDLL(DWORD pid, const wchar_t* dllName);

HMODULE GetRemoteModuleHandle(DWORD pid, const wchar_t* moduleName);

void PrintModules(DWORD pid);
