#include "pch.h"
#include "afxdialogex.h"
#include "controller.h"
#include "Viewer.h"
#include "zxhelper.h"
#include "Viewer_packet.h"
#include "Log.h"
#include "normalFun.h"
//#include "T_obj.h"

// C_viewer 对话框

UINT AutoHanHua(LPVOID pParam);
bool speakFlag = false;
CWinThread* pThread = nullptr; // 线程句柄
CStringArray strArry;
IMPLEMENT_DYNAMIC(Viewer, CDialogEx)

Viewer::Viewer(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG1, pParent)
{
}

Viewer::~Viewer()
{
}

void Viewer::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_name, show_name);
	DDX_Control(pDX, IDC_STATIC_qixue, show_qixue);
	DDX_Control(pDX, IDC_STATIC9_zhenqi, show_zhenqi);
	DDX_Control(pDX, IDC_STATIC10_yuanli, show_yuanli);
	DDX_Control(pDX, IDC_STATIC_wushi, show_wushi);
	DDX_Control(pDX, IDC_STATIC_jianmian, show_jianmian);
	DDX_Control(pDX, IDC_BUTTON3, m_btn3);
	DDX_Control(pDX, IDC_EDIT1, m_edit1);
}

BEGIN_MESSAGE_MAP(Viewer, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &Viewer::OnBnClickedButton1)

	ON_BN_CLICKED(IDC_BUTTON2, &Viewer::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &Viewer::OnBnClickedButton3)
END_MESSAGE_MAP()
CLog g_Log;
// 初始化日志
BOOL InitLog() {
	CString strLogPath = _T("C:\\Users\\54469\\Desktop\\zxhelper\\MyAppLog.txt"); // 日志文件路径
	return g_Log.Init(strLogPath);
}
// 关闭日志
void CloseLog() {
	g_Log.Close();
}

BOOL Viewer::OnInitDialog() {
	CDialogEx::OnInitDialog();  // 调用基类的 OnInitDialog
	// 在控件初始化后调用 init
	//
	// 初始化日志
	if (!InitLog()) {
		AfxMessageBox(_T("Failed to initialize log!"));
	}
	g_Log.Write(_T("Application started."));

	init();
	return TRUE;  // 返回 TRUE，除非你将焦点设置到某个控件
}
// C_viewer 消息处理程序

void Viewer::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	relex();
}

void Viewer::init() {
	CString str;

	double percentage;

	T人物属性 tPersonAtt = getPersonAtt();

	show_name.SetWindowTextW(tPersonAtt.p名称);

	str.Format(_T("%d"), tPersonAtt.d血量);
	show_qixue.SetWindowTextW(str);

	str.Format(_T("%d"), tPersonAtt.d真气);
	show_zhenqi.SetWindowTextW(str);

	str.Format(_T("%d"), tPersonAtt.d元力);
	show_yuanli.SetWindowTextW(str);

	percentage = tPersonAtt.d无视 / 10.0;
	str.Format(_T("%.1f"), percentage);
	str.Append(_T("%"));
	show_wushi.SetWindowTextW(str);

	percentage = tPersonAtt.d减免 / 10.0;
	str.Format(_T("%.1f"), percentage);
	str.Append(_T("%"));
	show_jianmian.SetWindowTextW(str);
}

Viewer_packet* c_viewer_packet;
DWORD WINAPI showDialog_p(LPARAM LPDATE) {
	c_viewer_packet = new Viewer_packet;
	c_viewer_packet->DoModal();
	delete c_viewer_packet;
	//FreeLibraryAndExitThread(theApp.m_hInstance, 1);
	return TRUE;
}

void Viewer::OnBnClickedButton2()
{
	::CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)showDialog_p, NULL, NULL, NULL);

	// TODO: 在此添加控件通知处理程序代码
}

void Viewer::OnBnClickedButton3()
{
	if (!speakFlag)
	{
		if (!pThread) // 如果线程未启动
		{
			speakFlag = true; // 设置标志为 true，启动线程
			CString* strNumber = new CString();
			m_edit1.GetWindowTextW(*strNumber);
			m_edit1.EnableWindow(FALSE); // 禁用编辑框
			pThread = AfxBeginThread(AutoHanHua, strNumber); // 创建线程
			pThread->m_bAutoDelete = false;
			if (pThread)
			{
				Call_输出调试信息("调试信息:喊话线程启动...\n");
			}
			else {
				Call_输出调试信息("调试信息:喊话线程启动失败...\n");
			}
		}
		else {
			speakFlag = !speakFlag;
		}
		
		m_btn3.SetWindowText(L"喊话运行中");
	}
	else {
		m_btn3.SetWindowText(L"自动喊话");	
		speakFlag = !speakFlag;
	}
	
	Call_输出调试信息("调试信息:speakFlag = %d\n", speakFlag);
}

UINT __stdcall AutoHanHua(LPVOID pParam)
{
	
	ReadUtf8FileAndSplitByNewline(strArry);
	CString* str = (CString*)pParam;
	int interval = _ttoi(str->GetString()) * 1000;

	Call_输出调试信息("调试信息:interval = %d\n", interval);

	for (int i = 1; ;i++) {
		if (speakFlag)
		{
			// 在这里实现需要循环运行的逻辑

			//TODO 将CString中的字符串加载到内存中,并得到地址.
			Call_输出调试信息("调试信息:interval = %d", interval);

			for (int i = 0; i < strArry.GetSize(); i++)
			{
				if (speakFlag)
				{
					CString str = strArry.GetAt(i);
					hanHua(str);
					Sleep(interval); // 休眠 1 秒
				}
				else {
					Sleep(1000);
					i--;
				}
			
			}
		}
		else
		{
			// 如果 speakFlag 为 false，暂停线程
			///TRACE(_T("线程暂停...\n"));
			Call_输出调试信息("调试信息:喊话线程暂停..");
			Sleep(1000); // 休眠 500 毫秒，避免忙等待
		}

		// 检查线程是否被要求退出
		if (pThread->m_bAutoDelete)
		{
			break; // 退出循环
		}
	}
	//TRACE(_T("线程退出...\n"));
	Call_输出调试信息("调试信息:喊话线程退出..");
	return 0; // 线程结束
}

void Viewer::OnStartThread()
{
}

void Viewer::PostNcDestroy()
{
	// TODO: 在此添加专用代码和/或调用基类

	CDialogEx::PostNcDestroy();
	if (pThread)
	{
		HANDLE hThread = pThread->m_hThread;

		pThread->m_bAutoDelete = true; // 请求线程退出

		Call_输出调试信息("调试信息:退出喊话线程中..");

		WaitForSingleObject(hThread, INFINITE);
		// 关闭线程句柄
		CloseHandle(hThread);

		pThread = nullptr; // 清空线程句柄
	}
	// 等待线程结束
	g_Log.Write(_T("Application exited."));
	CloseLog(); // 关闭日志
	Call_输出调试信息("调试信息:程序退出(销毁完毕)..");
}