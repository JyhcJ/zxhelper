#include "pch.h"
#include "afxdialogex.h"
#include "controller.h"
#include "Log.h"
#include "normalFun.h"
#include "Viewer.h"
#include "Viewer_packet.h"
#include "zxhelper.h"
#include "counst.h"

#include <nlohmann/json.hpp>
//#include "T_obj.h"

// C_viewer 对话框
extern INT_PTR nRet;
Viewer* g_pDlg;

CLog g_Log;

UINT AutoHanHua(LPVOID pParam);

void UpdateWindowPosition(HWND targetHwnd, HWND Hwnd);

bool speakFlag = false;
CWinThread* pThread_speak = nullptr; // 线程句柄
CStringArray strArry;

HANDLE hThread_att;
HWND g_hTargetWnd; // 吸附目标窗口句柄
HWND g_dllHWnd;
//HWND g_Hwnd; // dialog窗口句柄
bool g_bAttached = true; // 是否已吸附
HANDLE pipeThread;
IMPLEMENT_DYNAMIC(Viewer, CDialogEx)

DWORD __stdcall PipeThread(LPVOID lpParam);





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
	ON_WM_MOVE()
	ON_WM_TIMER()
//	ON_WM_CREATE()
ON_BN_CLICKED(IDC_BUTTON5, &Viewer::OnBnClickedButton5)
END_MESSAGE_MAP()

// C_viewer 消息处理程序
// 初始化
BOOL Viewer::OnInitDialog() {
	CDialogEx::OnInitDialog();  // 调用基类的 OnInitDialog
	// 在控件初始化后调用 init

	init();
	return TRUE;  // 返回 TRUE，除非你将焦点设置到某个控件
}

// 自定义初始化
void Viewer::init() {
	// 初始化日志
	if (!g_Log.Init(LOG_PATH)) {
		AfxMessageBox(_T("Failed to initialize log!"));
	}
	g_Log.Write(_T("Application started."));

	//_set_se_translator(seh_translator); // __try -> try 注册转换器

	//虽然这样也可以,但是这里还没有初始化完成...
	//pDlg = reinterpret_cast<Viewer*>(AfxGetMainWnd());
	g_dllHWnd = this->m_hWnd;
	g_pDlg = this;

	//获取属性
	thread_getAtt();
	
	//向dll传递数据
	pipeThread = CreateThread(NULL, 0, PipeThread, NULL, 0, NULL);

	//调整窗口位置
	SetTimer(1, 250, NULL);



	//hThread_att = ::CreateThread(NULL, NULL, thread_getAtt, NULL, NULL, NULL);



}




// 打坐按钮
void Viewer::OnBnClickedButton1()
{// 窗口初始化(已有窗口句柄) 并没有
//int Viewer::OnCreate(LPCREATESTRUCT lpCreateStruct)
//{
//	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
//		return -1;
//
	// TODO:  在此添加您专用的创建代码
//	pDlg = reinterpret_cast<Viewer*>(AfxGetMainWnd());
//	return 0;
//}
	// TODO: 在此添加控件通知处理程序代码
	relex();

}

//#pragma pack(push, 1)
//struct MapEntry {
//	DWORD pid;
//	HWND  hwnd;
//};
//#pragma pack(pop)

DWORD WINAPI PipeThread(LPVOID lpParam) {
	Sleep(2000);
	int 循环次数 = 0;
	Call_输出调试信息("[客服端]PipeThread线程启动");
	HANDLE hPipe = NULL;
	while (!g_exitFLag) {
		// 尝试连接管道
		hPipe = CreateFileW(
			L"\\\\.\\pipe\\MyPipe",             // 管道名称
			GENERIC_READ | GENERIC_WRITE ,      // 读写权限
			0,                                  // 不共享
			NULL,                               // 默认安全属性
			OPEN_EXISTING,                      // 打开已存在的管道
			FILE_ATTRIBUTE_NORMAL,				// 可选：异步模式FILE_FLAG_OVERLAPPED,  
			NULL                                // 无模板文件
		);

		Call_输出调试信息("[客服端]hPipe = %d", hPipe);
		if (hPipe != INVALID_HANDLE_VALUE) {
			if (循环次数 >= 3)
			{
				Call_输出调试信息("[客服端]管道连接超时");
				break;
			}
			Call_输出调试信息("[客服端]管道创建成功");
	
			DWORD bytesRead;
			//std::map<DWORD, HWND> hwndMap ;
			// 读取数据
			size_t count;
			// 先读数据量
			if (ReadFile(hPipe, &count, sizeof(count), &bytesRead, NULL))
			{
				Call_输出调试信息("[客服端]接收到 count:%d", count);
				std::vector<QWORD> arry_pipeServer(count);
				if (ReadFile(hPipe, arry_pipeServer.data(), count * sizeof(QWORD), &bytesRead, NULL)) {
					for (size_t i = 0; i < arry_pipeServer.size(); i++) {
						Call_输出调试信息("  arry[%zu] = 0x%llX", i, arry_pipeServer[i]);


						if (static_cast<DWORD>(arry_pipeServer[i]) == static_cast<DWORD>(GetCurrentProcessId())) {
							g_hTargetWnd = (HWND)arry_pipeServer[i + 1];
							Call_输出调试信息("[客服端]接收到 m_hTargetWnd:%d", g_hTargetWnd);
							//回响应（如果需要）

							DWORD bytesWritten;
							HWND dllHwnd = g_dllHWnd;
							auto my_pair = std::make_pair((DWORD)arry_pipeServer[i],  (QWORD)dllHwnd);
							nlohmann::json j = { {"dllPid", my_pair.first}, {"dllHwnd", my_pair.second} };
							std::string serialized = j.dump();
							uint32_t len = serialized.size();

							if (WriteFile(hPipe, &len, sizeof(len), &bytesWritten, NULL)) {
								if (WriteFile(hPipe, serialized.data(), len, &bytesWritten, NULL))
								{
									Call_输出调试信息("[客服端]string成功发送");
								}
								else {
									Call_输出调试信息("[客服端]string写入管道失败，错误代码：%d", GetLastError());
								}
								Call_输出调试信息("[客服端]len成功发送");
							}
							else {
								Call_输出调试信息("[客服端]len写入管道失败，错误代码：%d", GetLastError());
							}
							break;
						}
					}

					break;//遍历完没有就没有了
				}
				else {
					Call_输出调试信息("[客服端]读取 arry_pipeServer 失败 错误码:%d", GetLastError());
					Sleep(2000);
					循环次数++;
					break;
				}
			}
			else {
				Call_输出调试信息("[客服端]读取 count 失败 错误码:%d", GetLastError());
				Sleep(2000);
				循环次数++;
				continue;
			}
		}
		else {
			if (循环次数 >= 3) {
				Call_输出调试信息("[客户端]连接失败，错误码: %d", GetLastError());
				break;
			}
			if (hPipe) CloseHandle(hPipe);
			hPipe = NULL;
			循环次数++;
			Sleep(1000);
		}
	
		Sleep(1000);
	}

	if (hPipe != NULL && hPipe != INVALID_HANDLE_VALUE) {
		CloseHandle(hPipe);
		hPipe = NULL;        // 重置为 NULL，避免重复关闭
	}
	return 0;
}

// 关闭日志
void CloseLog() {
	g_Log.Close();
}

void Viewer::thread_getAtt()
{
	// 使用 Lambda 表达式包装成员函数
	auto threadFunc = [](LPVOID lpParameter) -> DWORD {
		Viewer* pThis = (Viewer*)lpParameter;
		pThis->getAtt();
		return 0;
		}; // 创建线程，传递 this 指针

	hThread_att = CreateThread(
		NULL,                   // 安全属性
		0,                      // 堆栈大小（默认）
		threadFunc,             // Lambda 表达式
		this,                   // 传递 this 指针
		0,                      // 创建标志（立即执行）
		NULL                    // 线程 ID
	);

	if (hThread_att == NULL) {
		//std::cerr << "Failed to create thread!" << std::endl;
		return;
	}
}

void Viewer::getAtt() {
	CString str;
	double percentage;
	while (true) {
		T人物属性 tPersonAtt = getPersonAtt();
		
		if (tPersonAtt.有效性) {
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
	
		Sleep(1000);
		if (g_exitFLag) {
			Call_输出调试信息("调试信息:属性线程内部退出...");
			break;
		}
	}
	return;
}

// 打开遍历窗口
Viewer_packet* c_viewer_packet;
DWORD WINAPI showDialog_p(LPARAM LPDATE) {
	c_viewer_packet = new Viewer_packet;
	c_viewer_packet->DoModal();
	delete c_viewer_packet;
	//FreeLibraryAndExitThread(theApp.m_hInstance, 1);
	return TRUE;
}

// 打开遍历窗口(创建线程)
void Viewer::OnBnClickedButton2()
{
	::CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)showDialog_p, NULL, NULL, NULL);

	// TODO: 在此添加控件通知处理程序代码
}
// 喊话按钮被单击
void Viewer::OnBnClickedButton3()
{
	if (!speakFlag)
	{
		if (!pThread_speak) // 如果线程未启动
		{
			speakFlag = true; // 设置标志为 true，启动线程
			CString* strNumber = new CString();
			m_edit1.GetWindowTextW(*strNumber);
			m_edit1.EnableWindow(FALSE); // 禁用编辑框
			pThread_speak = AfxBeginThread(AutoHanHua, strNumber); // 创建线程
			pThread_speak->m_bAutoDelete = false;
			if (pThread_speak)
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
// 喊话线程
UINT __stdcall AutoHanHua(LPVOID pParam)
{
	ReadUtf8FileAndSplitByNewline(strArry);
	CString* str = (CString*)pParam;
	int interval = _ttoi(str->GetString()) * 1000;
	if (interval < 3000)
	{
		interval = 6000;
	}

	for (int i = 1; ;i++) {
		if (speakFlag)
		{
			for (int j = 0; j < strArry.GetSize(); j++)
			{
				if (speakFlag)
				{
					CString str = strArry.GetAt(j);
					hanHua(str);
					Sleep(interval); // 休眠 1 秒
				}
				else if (pThread_speak->m_bAutoDelete) {
					break;
				}
				else {
					Sleep(1000);
					j--;
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
		if (pThread_speak->m_bAutoDelete)
		{
			break; // 退出循环
		}
	}
	//TRACE(_T("线程退出...\n"));
	Call_输出调试信息("调试信息:喊话线程退出..");
	return 0; // 线程结束
}


RECT g_rcLastTargetPos = { 0 };
// 更新窗口位置
void UpdateWindowPosition(HWND targetHwnd, HWND Hwnd)
{
	RECT rcTarget;
	::GetWindowRect(targetHwnd, &rcTarget);

	if (g_bAttached)
	{
		RECT rcDialog;
		::GetWindowRect(Hwnd, &rcDialog);
		// 右侧吸附逻辑
		int width = rcDialog.right - rcDialog.left;
		//int higth = rcDialog.top - rcDialog.bottom;
		rcDialog.left = rcTarget.right;
		rcDialog.top = rcTarget.top;

		//rcDialog.right = rcDialog.left + width;
		//rcDialog.top = rcDialog.bottom + higth;

		::SetWindowPos(
			Hwnd,
			NULL,
			rcDialog.left,
			rcDialog.top,
			0, 0,
			SWP_NOZORDER | SWP_NOSIZE
		);
	}
}
// 吸附操作, 使窗口吸附到目标窗口的边缘(默认吸附)...暂用不上
void Viewer::OnMove(int x, int y)
{
	CDialogEx::OnMove(x, y);

	// TODO: 在此处添加消息处理程序代码
	if (g_hTargetWnd == NULL)
		return;

	RECT rcTarget, rcDialog;
	::GetWindowRect(g_hTargetWnd, &rcTarget);  // 获取目标窗口的屏幕坐标
	GetWindowRect(&rcDialog);                  // 获取 Dialog 的屏幕坐标

	int nThreshold = 10;  // 吸附阈值（像素）

	// 检测是否需要吸附
	if (abs(rcDialog.left - rcTarget.right) <= nThreshold)
	{
		// 吸附到目标窗口的右侧
		rcDialog.left = rcTarget.right;
		rcDialog.right = rcDialog.left + (rcDialog.right - rcDialog.left);
		g_bAttached = true;
	}
	else if (abs(rcDialog.right - rcTarget.left) <= nThreshold)
	{
		// 吸附到目标窗口的左侧
		rcDialog.right = rcTarget.left;
		rcDialog.left = rcDialog.right - (rcDialog.right - rcDialog.left);
		g_bAttached = true;
	}
	else if (abs(rcDialog.top - rcTarget.bottom) <= nThreshold)
	{
		// 吸附到目标窗口的底部
		rcDialog.top = rcTarget.bottom;
		rcDialog.bottom = rcDialog.top + (rcDialog.bottom - rcDialog.top);
		g_bAttached = true;
	}
	else if (abs(rcDialog.bottom - rcTarget.top) <= nThreshold)
	{
		// 吸附到目标窗口的顶部
		rcDialog.bottom = rcTarget.top;
		rcDialog.top = rcDialog.bottom - (rcDialog.bottom - rcDialog.top);
		g_bAttached = true;
	}
	else
	{
		//m_bAttached = false;
	}

	// 如果需要吸附，则更新 Dialog 的位置
	if (g_bAttached)
	{
		SetWindowPos(NULL, rcDialog.left, rcDialog.top, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
	}
}

// 定时器消息处理程序(检查吸附目标窗口位置)
void Viewer::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CDialogEx::OnTimer(nIDEvent);
	if (nIDEvent == 1)
	{
		if (g_hTargetWnd == NULL)
		{
			return;
		}

		RECT rcCurrent;
		::GetWindowRect(g_hTargetWnd, &rcCurrent);

		// 检查位置是否变化
		if (memcmp(&rcCurrent, &g_rcLastTargetPos, sizeof(RECT)) != 0)
		{
			UpdateWindowPosition(g_hTargetWnd, m_hWnd);
			g_rcLastTargetPos = rcCurrent;
		}
	}
}

// windows消息回调函数
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	Call_输出调试信息("看我看我看我1");
	switch (msg) {

	case WM_MY_CROSS_PROCESS_MSG:
		Call_输出调试信息("看我看我看我1");
		switch (wParam)
		{
		case WM_MY_CROSS_PROCESS_MSG_QUIT:
			g_pDlg->DestroyWindow();
			Call_输出调试信息("看我看我看我g_pDlg->DestroyWindow();");
			break;
		default:
			break;
		}

		// 在这里调用你的目标函数
	
		return 0;  // 表示处理成功
	default:
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}
}

// 窗口销毁
void Viewer::PostNcDestroy()
{
	CDialogEx::PostNcDestroy();
	g_exitFLag = true;  //控制CreateThread创建的线程

	//喊话线程
	if (pThread_speak)
	{
		HANDLE hThread = pThread_speak->m_hThread;

		pThread_speak->m_bAutoDelete = true; // 请求线程退出

		WaitForSingleObject(hThread, INFINITE);

		// 关闭线程句柄 AfxBeginThread 会自动管理线程资源，无需手动调用 CloseHandle。
		CloseHandle(hThread);

		pThread_speak = nullptr; // 清空线程句柄
		Call_输出调试信息("调试信息:已退出喊话线程");
	}

	//管道线程
	WaitForSingleObject(pipeThread, INFINITE);
	CloseHandle(pipeThread);

	//属性线程
	WaitForSingleObject(hThread_att, INFINITE);
	CloseHandle(hThread_att);
	Call_输出调试信息("调试信息:已退出获取属性线程");

	g_Log.Write(_T("Application exited."));
	CloseLog(); // 关闭日志
	Call_输出调试信息("调试信息:程序退出(销毁完毕)..");

}



BOOL Viewer::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	//Call_输出调试信息("看我看我看我1");
	switch (pMsg->message) {

	case WM_MY_CROSS_PROCESS_MSG:
		
		switch (pMsg->wParam)
		{
		case WM_MY_CROSS_PROCESS_MSG_QUIT:
			g_pDlg->EndDialog(nRet);  // 关闭模态对话框
			Call_输出调试信息("调用消息关闭窗口");
			break;
		default:
			break;
		}
	default:
		break;
		//return DefWindowProcW( pMsg->message, pMsg->wParam, pMsg->lParam);
	}

	return CDialogEx::PreTranslateMessage(pMsg);

}

void Viewer::OnBnClickedButton5()
{
	// TODO: 在此添加控件通知处理程序代码
	updateAddress();
}
