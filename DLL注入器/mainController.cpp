
#include "pch.h"
#include "DLL注入器Dlg.h"
#include "mainController.h"
#include <tlhelp32.h>
#include <vector>
#include "test.h"

//注入DLL
const char* DLL_PATH = "C:/Users/54469/Desktop/zxhelper/zxhelper/x64/Debug/zxhelper.dll";
const wchar_t* DLL_NAME = L"zxhelper.dll";
extern std::vector<QWORD> arry_pipeServer;
void Call_输出调试信息(char* pszFormat, ...)
{
#ifdef _DEBUG
	// 定义固定前缀字符串
	const char* prefix = "[调试信息] ";
	char szbufFormat[0x1000];
	char szbufFormat_Game[0x1100] = "";
	// 将固定前缀拼接到 szbufFormat_Game
	strcat_s(szbufFormat_Game, prefix);
	va_list argList;
	va_start(argList, pszFormat);//参数列表初始化
	vsprintf_s(szbufFormat, pszFormat, argList);
	strcat_s(szbufFormat_Game, szbufFormat);
	OutputDebugStringA(szbufFormat_Game);
	va_end(argList);

#endif
}

void Call_输出调试信息W(const wchar_t* pszFormat, ...)
{
#ifdef _DEBUG
	// 定义固定前缀字符串
	const wchar_t* prefix = L"[调试信息] ";
	wchar_t* szbufFormat = new wchar_t[0x1000];
	wchar_t* szbufFormat_Game = new wchar_t[0x1100];

	// 初始化 szbufFormat_Game
	szbufFormat_Game[0] = L'\0';

	// 将固定前缀拼接到 szbufFormat_Game
	wcscat_s(szbufFormat_Game, 0x1100, prefix);

	// 处理可变参数
	va_list argList;
	va_start(argList, pszFormat); // 参数列表初始化
	vswprintf_s(szbufFormat, 0x1000, pszFormat, argList); // 格式化宽字符字符串
	wcscat_s(szbufFormat_Game, 0x1100, szbufFormat); // 拼接结果
	OutputDebugStringW(szbufFormat_Game); // 输出 UTF-16 调试信息
	va_end(argList);

	// 释放堆内存
	delete[] szbufFormat;
	delete[] szbufFormat_Game;
#endif
}


// 创建管道并获取进程句柄
bool cereatePipeAndGetProHandle(HWND hwnd) {
	//DWORD tid = GetWindowThreadProcessId(hWnd, &pid); // tid 是线程ID，pid 是进程ID

	QWORD pid;
	GetWindowThreadProcessId(hwnd, (LPDWORD) &pid);
	
	HMODULE hMod = GetRemoteModuleHandle(static_cast<DWORD>(pid), DLL_NAME);
	if (hMod) {
		Call_输出调试信息W(L"模块:%s  未关闭", DLL_NAME);
		AfxMessageBox(L"稍等一会,模块还未退出(重启被注入程序也可以)");
		return false;
	}

	arry_pipeServer.push_back(pid);
	arry_pipeServer.push_back((QWORD)hwnd);

	if (pid == NULL)
	{
		//MessageBoxW(L"获取 ""pid 失败", NULL, MB_OK);
		return false;
	}
	HANDLE process = OpenProcess(PROCESS_ALL_ACCESS, FALSE,static_cast<DWORD>(pid));
	if (process == NULL)
	{
		//MessageBoxW(L"打开 process 失败", NULL, MB_OK);
		return false;
	}


	CString rString;

	CDLL注入器Dlg* pDlg = reinterpret_cast<CDLL注入器Dlg*>(AfxGetMainWnd());
	if (pDlg != nullptr)
	{
		pDlg->show_edit.GetWindowTextW(rString);

	}
	if (rString.IsEmpty())
	{
		return injectDLL(process, DLL_PATH);
		//dll_inject(process, "C:/Users/54469/Desktop/zxhelper/zxhelper/x64/Debug/定位工具/定位.dll");
	}
	else {
		return injectDLL(process, (const char*)rString.GetString());
	}

}
// 注入DLL
BOOL injectDLL(HANDLE processHandle, const char* dll) {
	// 在目标进程中分配内存
	LPVOID address = VirtualAllocEx(processHandle, NULL, 256, MEM_COMMIT, PAGE_READWRITE);
	if (address == NULL)
	{
		MessageBoxW(NULL, L"分配内存失败2!", NULL, MB_OK);
		return FALSE;
	}

	BOOL ret = WriteProcessMemory(processHandle, address, dll, strlen(dll) + 1, NULL);
	if (ret == NULL)
	{
		MessageBoxW(NULL, L"写内存失败2!", NULL, MB_OK);
		return FALSE;
	}
	HANDLE threadHandle = CreateRemoteThread(processHandle, NULL, NULL, (LPTHREAD_START_ROUTINE)LoadLibraryA, address, NULL, NULL);
	if (threadHandle == NULL)
	{
		MessageBoxW(NULL, L"创建远程线程失败!", NULL, MB_OK);
		VirtualFreeEx(processHandle, address, 0, MEM_RELEASE);
		return FALSE;
	}
	WaitForSingleObject(threadHandle, INFINITE);
	CloseHandle(threadHandle);
	VirtualFreeEx(processHandle, address, 0, MEM_RELEASE);
	CloseHandle(processHandle);
	return TRUE;
}
// 卸载DLL
bool UnloadInjectedDLL(DWORD pid, const wchar_t* dllName) {
	// 1. 打开目标进程
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
	if (!hProcess) return false;

	// 2. 获取DLL模块句柄（需枚举模块）
	HMODULE hMod = GetRemoteModuleHandle(pid, dllName);
	if (!hMod) {
		CloseHandle(hProcess);
		return false;
	}
	Call_输出调试信息("[客服端] 需要执行卸载模块");
	// 3. 创建卸载线程
	HANDLE hThread = CreateRemoteThread(hProcess, NULL, 0,
		(LPTHREAD_START_ROUTINE)FreeLibrary, hMod, 0, NULL);

	if (!hThread) {
		CloseHandle(hProcess);
		//return false;
	}

	// 4. 等待完成
	WaitForSingleObject(hThread, INFINITE);

	// 5. 清理
	CloseHandle(hThread);
	CloseHandle(hProcess);

	// 检查DLL是否已被卸载,,这实在当前进程中
	/*if (GetModuleHandleW(DLL_NAME) == NULL) {
		Call_输出调试信息("[客服端] PID:%d 执行卸载zxhelper模块(但可能卸载不成功,因为有未知阻塞)", pid);
	}*/

	return true;
}

// 获取远程进程的指定模块句柄
HMODULE GetRemoteModuleHandle(DWORD pid, const wchar_t* moduleName) {
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, pid);
	if (hSnapshot == INVALID_HANDLE_VALUE)
		return NULL;

	MODULEENTRY32 me = { sizeof(me) };
	HMODULE hMod = NULL;
	if (Module32First(hSnapshot, &me)) {
		do {
			if (_wcsicmp(me.szModule, moduleName) == 0) {
				//Call_输出调试信息W(L"%s", moduleName);
				hMod = me.hModule;
				break;
			}
		} while (Module32Next(hSnapshot, &me));
	}

	CloseHandle(hSnapshot);
	return hMod;
}

// 使用示例
void PrintModules(DWORD pid) {
	//auto modules = GetRemoteModuleHandle(pid);
	//for (HMODULE hMod : modules) {
	//	MODULEENTRY32 me = { sizeof(me) };
	//	if (GetModuleInformation(GetCurrentProcess(), hMod, &me, sizeof(me))) {
	//		wprintf(L"Module: %s (0x%p)\n", me.szModule, me.hModule);
	//		
	//}
	HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, pid);
	// 方法1：使用ToolHelp32直接获取模块信息
	MODULEENTRY32 me = { sizeof(me) };
	HMODULE hMod = GetRemoteModuleHandle(pid, DLL_NAME);
	if (hMod) {
		//Call_输出调试信息W(L"[ToolHelp32]卸载模块名: %s, 基址: 0x%p\n", me.szModule, me.modBaseAddr);
		// 3. 创建卸载线程
		HANDLE hThread = CreateRemoteThread(hProcess, NULL, 0,
			(LPTHREAD_START_ROUTINE)FreeLibrary, hMod, 0, NULL);

		if (!hThread) {
			CloseHandle(hProcess);
			return;
		}

		// 4. 等待完成
		WaitForSingleObject(hThread, INFINITE);

		// 5. 清理
		CloseHandle(hThread);
		CloseHandle(hProcess);
	}

	// 方法2：使用PSAPI获取模块内存信息

	//if (hProcess) {
	//	MODULEINFO mi = { 0 };
	//	if (GetModuleInformation(hProcess, hMod, &mi, sizeof(mi))) {
	//		//Call_输出调试信息("[PSAPI] 基址: 0x%p, 大小: %u\n", mi.lpBaseOfDll, mi.SizeOfImage);
	//		wprintf(L"[PSAPI] 基址: 0x%p, 大小: %u\n", mi.lpBaseOfDll, mi.SizeOfImage);
	//	}
	//	CloseHandle(hProcess);
	//}

}