
#include "pch.h"
#include "DLLע����Dlg.h"
#include "mainController.h"
#include <tlhelp32.h>
#include <vector>
#include "test.h"

//ע��DLL
const char* DLL_PATH = "C:/Users/54469/Desktop/zxhelper/zxhelper/x64/Debug/zxhelper.dll";
const wchar_t* DLL_NAME = L"zxhelper.dll";
extern std::vector<QWORD> arry_pipeServer;
void Call_���������Ϣ(char* pszFormat, ...)
{
#ifdef _DEBUG
	// ����̶�ǰ׺�ַ���
	const char* prefix = "[������Ϣ] ";
	char szbufFormat[0x1000];
	char szbufFormat_Game[0x1100] = "";
	// ���̶�ǰ׺ƴ�ӵ� szbufFormat_Game
	strcat_s(szbufFormat_Game, prefix);
	va_list argList;
	va_start(argList, pszFormat);//�����б��ʼ��
	vsprintf_s(szbufFormat, pszFormat, argList);
	strcat_s(szbufFormat_Game, szbufFormat);
	OutputDebugStringA(szbufFormat_Game);
	va_end(argList);

#endif
}

void Call_���������ϢW(const wchar_t* pszFormat, ...)
{
#ifdef _DEBUG
	// ����̶�ǰ׺�ַ���
	const wchar_t* prefix = L"[������Ϣ] ";
	wchar_t* szbufFormat = new wchar_t[0x1000];
	wchar_t* szbufFormat_Game = new wchar_t[0x1100];

	// ��ʼ�� szbufFormat_Game
	szbufFormat_Game[0] = L'\0';

	// ���̶�ǰ׺ƴ�ӵ� szbufFormat_Game
	wcscat_s(szbufFormat_Game, 0x1100, prefix);

	// ����ɱ����
	va_list argList;
	va_start(argList, pszFormat); // �����б��ʼ��
	vswprintf_s(szbufFormat, 0x1000, pszFormat, argList); // ��ʽ�����ַ��ַ���
	wcscat_s(szbufFormat_Game, 0x1100, szbufFormat); // ƴ�ӽ��
	OutputDebugStringW(szbufFormat_Game); // ��� UTF-16 ������Ϣ
	va_end(argList);

	// �ͷŶ��ڴ�
	delete[] szbufFormat;
	delete[] szbufFormat_Game;
#endif
}


// �����ܵ�����ȡ���̾��
bool cereatePipeAndGetProHandle(HWND hwnd) {
	//DWORD tid = GetWindowThreadProcessId(hWnd, &pid); // tid ���߳�ID��pid �ǽ���ID

	QWORD pid;
	GetWindowThreadProcessId(hwnd, (LPDWORD) &pid);
	
	HMODULE hMod = GetRemoteModuleHandle(static_cast<DWORD>(pid), DLL_NAME);
	if (hMod) {
		Call_���������ϢW(L"ģ��:%s  δ�ر�", DLL_NAME);
		AfxMessageBox(L"�Ե�һ��,ģ�黹δ�˳�(������ע�����Ҳ����)");
		return false;
	}

	arry_pipeServer.push_back(pid);
	arry_pipeServer.push_back((QWORD)hwnd);

	if (pid == NULL)
	{
		//MessageBoxW(L"��ȡ ""pid ʧ��", NULL, MB_OK);
		return false;
	}
	HANDLE process = OpenProcess(PROCESS_ALL_ACCESS, FALSE,static_cast<DWORD>(pid));
	if (process == NULL)
	{
		//MessageBoxW(L"�� process ʧ��", NULL, MB_OK);
		return false;
	}


	CString rString;

	CDLLע����Dlg* pDlg = reinterpret_cast<CDLLע����Dlg*>(AfxGetMainWnd());
	if (pDlg != nullptr)
	{
		pDlg->show_edit.GetWindowTextW(rString);

	}
	if (rString.IsEmpty())
	{
		return injectDLL(process, DLL_PATH);
		//dll_inject(process, "C:/Users/54469/Desktop/zxhelper/zxhelper/x64/Debug/��λ����/��λ.dll");
	}
	else {
		return injectDLL(process, (const char*)rString.GetString());
	}

}
// ע��DLL
BOOL injectDLL(HANDLE processHandle, const char* dll) {
	// ��Ŀ������з����ڴ�
	LPVOID address = VirtualAllocEx(processHandle, NULL, 256, MEM_COMMIT, PAGE_READWRITE);
	if (address == NULL)
	{
		MessageBoxW(NULL, L"�����ڴ�ʧ��2!", NULL, MB_OK);
		return FALSE;
	}

	BOOL ret = WriteProcessMemory(processHandle, address, dll, strlen(dll) + 1, NULL);
	if (ret == NULL)
	{
		MessageBoxW(NULL, L"д�ڴ�ʧ��2!", NULL, MB_OK);
		return FALSE;
	}
	HANDLE threadHandle = CreateRemoteThread(processHandle, NULL, NULL, (LPTHREAD_START_ROUTINE)LoadLibraryA, address, NULL, NULL);
	if (threadHandle == NULL)
	{
		MessageBoxW(NULL, L"����Զ���߳�ʧ��!", NULL, MB_OK);
		VirtualFreeEx(processHandle, address, 0, MEM_RELEASE);
		return FALSE;
	}
	WaitForSingleObject(threadHandle, INFINITE);
	CloseHandle(threadHandle);
	VirtualFreeEx(processHandle, address, 0, MEM_RELEASE);
	CloseHandle(processHandle);
	return TRUE;
}
// ж��DLL
bool UnloadInjectedDLL(DWORD pid, const wchar_t* dllName) {
	// 1. ��Ŀ�����
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
	if (!hProcess) return false;

	// 2. ��ȡDLLģ��������ö��ģ�飩
	HMODULE hMod = GetRemoteModuleHandle(pid, dllName);
	if (!hMod) {
		CloseHandle(hProcess);
		return false;
	}
	Call_���������Ϣ("[�ͷ���] ��Ҫִ��ж��ģ��");
	// 3. ����ж���߳�
	HANDLE hThread = CreateRemoteThread(hProcess, NULL, 0,
		(LPTHREAD_START_ROUTINE)FreeLibrary, hMod, 0, NULL);

	if (!hThread) {
		CloseHandle(hProcess);
		//return false;
	}

	// 4. �ȴ����
	WaitForSingleObject(hThread, INFINITE);

	// 5. ����
	CloseHandle(hThread);
	CloseHandle(hProcess);

	// ���DLL�Ƿ��ѱ�ж��,,��ʵ�ڵ�ǰ������
	/*if (GetModuleHandleW(DLL_NAME) == NULL) {
		Call_���������Ϣ("[�ͷ���] PID:%d ִ��ж��zxhelperģ��(������ж�ز��ɹ�,��Ϊ��δ֪����)", pid);
	}*/

	return true;
}

// ��ȡԶ�̽��̵�ָ��ģ����
HMODULE GetRemoteModuleHandle(DWORD pid, const wchar_t* moduleName) {
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, pid);
	if (hSnapshot == INVALID_HANDLE_VALUE)
		return NULL;

	MODULEENTRY32 me = { sizeof(me) };
	HMODULE hMod = NULL;
	if (Module32First(hSnapshot, &me)) {
		do {
			if (_wcsicmp(me.szModule, moduleName) == 0) {
				//Call_���������ϢW(L"%s", moduleName);
				hMod = me.hModule;
				break;
			}
		} while (Module32Next(hSnapshot, &me));
	}

	CloseHandle(hSnapshot);
	return hMod;
}

// ʹ��ʾ��
void PrintModules(DWORD pid) {
	//auto modules = GetRemoteModuleHandle(pid);
	//for (HMODULE hMod : modules) {
	//	MODULEENTRY32 me = { sizeof(me) };
	//	if (GetModuleInformation(GetCurrentProcess(), hMod, &me, sizeof(me))) {
	//		wprintf(L"Module: %s (0x%p)\n", me.szModule, me.hModule);
	//		
	//}
	HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, pid);
	// ����1��ʹ��ToolHelp32ֱ�ӻ�ȡģ����Ϣ
	MODULEENTRY32 me = { sizeof(me) };
	HMODULE hMod = GetRemoteModuleHandle(pid, DLL_NAME);
	if (hMod) {
		//Call_���������ϢW(L"[ToolHelp32]ж��ģ����: %s, ��ַ: 0x%p\n", me.szModule, me.modBaseAddr);
		// 3. ����ж���߳�
		HANDLE hThread = CreateRemoteThread(hProcess, NULL, 0,
			(LPTHREAD_START_ROUTINE)FreeLibrary, hMod, 0, NULL);

		if (!hThread) {
			CloseHandle(hProcess);
			return;
		}

		// 4. �ȴ����
		WaitForSingleObject(hThread, INFINITE);

		// 5. ����
		CloseHandle(hThread);
		CloseHandle(hProcess);
	}

	// ����2��ʹ��PSAPI��ȡģ���ڴ���Ϣ

	//if (hProcess) {
	//	MODULEINFO mi = { 0 };
	//	if (GetModuleInformation(hProcess, hMod, &mi, sizeof(mi))) {
	//		//Call_���������Ϣ("[PSAPI] ��ַ: 0x%p, ��С: %u\n", mi.lpBaseOfDll, mi.SizeOfImage);
	//		wprintf(L"[PSAPI] ��ַ: 0x%p, ��С: %u\n", mi.lpBaseOfDll, mi.SizeOfImage);
	//	}
	//	CloseHandle(hProcess);
	//}

}