// zxhelper.cpp: 定义 DLL 的初始化例程。
//

#include "pch.h"
#include "framework.h"
#include "Viewer.h"
#include "zxhelper.h"
#include "Log.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


//
//TODO:  如果此 DLL 相对于 MFC DLL 是动态链接的，
//		则从此 DLL 导出的任何调入
//		MFC 的函数必须将 AFX_MANAGE_STATE 宏添加到
//		该函数的最前面。
//
//		例如: 
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// 此处为普通函数体
//		}
//
//		此宏先于任何 MFC 调用
//		出现在每个函数中十分重要。  这意味着
//		它必须作为以下项中的第一个语句:
//		出现，甚至先于所有对象变量声明，
//		这是因为它们的构造函数可能生成 MFC
//		DLL 调用。
//
//		有关其他详细信息，
//		请参阅 MFC 技术说明 33 和 58。
//

// CzxhelperApp

BEGIN_MESSAGE_MAP(CzxhelperApp, CWinApp)
END_MESSAGE_MAP()


// CzxhelperApp 构造

CzxhelperApp::CzxhelperApp()
{
	// TODO:  在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的 CzxhelperApp 对象

CzxhelperApp theApp;
Viewer* c_viewer;
DWORD WINAPI showDialog(LPARAM LPDATE) {
	c_viewer = new Viewer;
	c_viewer->DoModal();
	delete c_viewer;
	FreeLibraryAndExitThread(theApp.m_hInstance, 1);
	return TRUE;

}
// 全局日志对象
CLog g_Log;

// 初始化日志
BOOL InitLog() {
	CString strLogPath = _T("C:\\MyAppLog.txt"); // 日志文件路径
	return g_Log.Init(strLogPath);
}
// 关闭日志
void CloseLog() {
	g_Log.Close();
}
 //CzxhelperApp 初始化

BOOL CzxhelperApp::InitInstance()
{
	CWinApp::InitInstance();
	// 初始化日志
	if (!InitLog()) {
		AfxMessageBox(_T("Failed to initialize log!"));
	}
	g_Log.Write(_T("Application started."));
	// 其他初始化代码...
	::CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)showDialog, NULL, NULL, NULL);
	return TRUE;
}
int CzxhelperApp::ExitInstance() {
	g_Log.Write(_T("Application exited."));
	CloseLog(); // 关闭日志
	return CWinApp::ExitInstance();
}