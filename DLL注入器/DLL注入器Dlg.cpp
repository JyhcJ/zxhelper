// DLL注入器Dlg.cpp: 实现文件
//

#include "pch.h"
#include "afxdialogex.h"
#include "DLL注入器.h"
#include "DLL注入器Dlg.h"
#include "framework.h"
#include "mainController.h"
#include "PipeServer.h"
#include <iostream>
#include <windows.h>
#include "const.h"


extern const char* DLL_PATH;
extern const wchar_t* DLL_NAME;

HANDLE g_hPipeThread;

HANDLE g_hPipe;

std::vector<std::pair<DWORD, QWORD>>g_dllHwnds;

//std::map<DWORD, HWND> hwndMap;

PipeServer pipeServer;

std::vector<QWORD> arry_pipeServer;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

	// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()

// CDLL注入器Dlg 对话框

CDLL注入器Dlg::CDLL注入器Dlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DLL_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CDLL注入器Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, show_edit);
	DDX_Control(pDX, IDC_LIST1, m_ListBox);
}

BEGIN_MESSAGE_MAP(CDLL注入器Dlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CDLL注入器Dlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON1, &CDLL注入器Dlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CDLL注入器Dlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CDLL注入器Dlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &CDLL注入器Dlg::OnBnClickedButton4)
END_MESSAGE_MAP()

// CDLL注入器Dlg 消息处理程序
BOOL Call_提升权限(BOOL bEnable) //OpenProcess失败的情况调用
{
	BOOL fOK = FALSE;
	HANDLE hToken;
	if (OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES, &hToken)) //打开进程访问令牌
	{
		TOKEN_PRIVILEGES tp;
		tp.PrivilegeCount = 1;
		LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &tp.Privileges[0].Luid);
		tp.Privileges[0].Attributes = bEnable ? SE_PRIVILEGE_ENABLED : 0;
		AdjustTokenPrivileges(hToken, FALSE, &tp, sizeof(tp), NULL, NULL);
		fOK = (GetLastError() == ERROR_SUCCESS);
		CloseHandle(hToken);
	}
	return fOK;
}

BOOL CDLL注入器Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。
	myInit();
	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CDLL注入器Dlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CDLL注入器Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CDLL注入器Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

// 自定义初始化	
void CDLL注入器Dlg::myInit() {
	// 枚举所有窗口
	EnumWindows(EnumWindowsProc, reinterpret_cast<LPARAM>(TARGET_CLASSNAME));
}
// 注入并关闭
void CDLL注入器Dlg::OnBnClickedOk()
{
	CDialogEx::OnOK();
	OnBnClickedButton4();
}
// 注入按钮
void CDLL注入器Dlg::OnBnClickedButton4()
{
	
	// TODO: 在此添加控件通知处理程序代码
	int nSelectedCount = m_ListBox.GetSelCount();
	if (nSelectedCount == LB_ERR || nSelectedCount == 0)
	{
		AfxMessageBox(_T("没有选中注入目标！"));
		return;
	}

	// 分配缓冲区存储选中的项索引
	CArray<int, int> arySelectedIndices;
	arySelectedIndices.SetSize(nSelectedCount);
	m_ListBox.GetSelItems(nSelectedCount, arySelectedIndices.GetData());

	// 遍历选中的项，获取文本
	CString strSelectedItems;
	arry_pipeServer.clear();
	for (int i = 0; i < nSelectedCount; i++)
	{
		int nIndex = arySelectedIndices[i];
		HWND hwnd = (HWND)m_ListBox.GetItemData(nIndex);
		m_ListBox.GetText(nIndex, strSelectedItems);
		if (strSelectedItems.Find(L"[注入]") == -1)
		{

			cereatePipeAndGetProHandle(hwnd);
			//hwndMap.insert(std::make_pair(GetCurrentProcessId(), hwnd));
			//if (cereatePipeAndGetProHandle(hwnd))
			//{
			//	strSelectedItems += _T(" [注入]");
			//	m_ListBox.DeleteString(nIndex);            // 删除原项
			//	m_ListBox.InsertString(nIndex, strSelectedItems);    // 在原位置插入新文本
			//	m_ListBox.SetItemData(nIndex, (DWORD_PTR)hwnd);
			//}


		}
		else {
			DWORD pid;
			HWND dllHwnd = NULL;
			//Call_提升权限(TRUE);
			GetWindowThreadProcessId(hwnd, &pid);
			
			// first:PID   second:dllHwnd
			for(std::pair<DWORD,QWORD> onePair : g_dllHwnds)
			{	
				Call_输出调试信息("注入器onePair.first:%d", onePair.first);
				Call_输出调试信息("注入器onePair.second:%d", onePair.second);
				Call_输出调试信息("注入器pid.second:%d", pid);
				if (pid == onePair.first)
				{
					dllHwnd = reinterpret_cast<HWND>(onePair.second);
				}
			
			}
		
			if (dllHwnd != NULL)
			{
				Call_输出调试信息("注入器PostMessageW :%d", dllHwnd);
				::PostMessageW(dllHwnd, WM_MY_CROSS_PROCESS_MSG, (WPARAM)WM_MY_CROSS_PROCESS_MSG_QUIT, 1);

			}
			
			//PrintModules(pid);
		}

	}

	pipeServer.SetDatas(arry_pipeServer);
	OnBnClickedButton3();
}
// 注入记事本
void CDLL注入器Dlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	HWND hwndNotepad = ::FindWindow(L"Notepad", NULL);
	if (hwndNotepad == NULL)
	{
		AfxMessageBox(_T("未找到记事本窗口！"));
		return;
	}
	else {
		arry_pipeServer.clear();
		cereatePipeAndGetProHandle(hwndNotepad);
		//hwndMap.insert(std::make_pair(GetCurrentProcessId(), hwndNotepad));
		pipeServer.SetDatas(arry_pipeServer);

	}

}

// 测试按钮
void CDLL注入器Dlg::OnBnClickedButton2()
{
	const wchar_t* utf16Str1 = L"// ";
	const char* utf16Str2 = "// ";
	const wchar_t* utf16Str3 = L"你";
	const char* utf16Str4 = "你";

}

// 添加字符串到列表框
void CDLL注入器Dlg::addString(wchar_t* windowTitle, HWND hwnd,bool isInject) {
	CString str;
	if (isInject)
	{
		str.Format(L"[注入]%s %X", windowTitle, hwnd);
	}
	else {
		str.Format(L"%s %X", windowTitle, hwnd);
	}

	m_ListBox.AddString(str.GetString());
	int index = m_ListBox.GetCount();
	m_ListBox.SetItemData(index - 1, (DWORD_PTR)hwnd);

}

// 刷新 枚举窗口按钮
void CDLL注入器Dlg::OnBnClickedButton3()
{
	// TODO: 在此添加控件通知处理程序代码
	m_ListBox.ResetContent();

	// 枚举所有窗口
	EnumWindows(EnumWindowsProc, reinterpret_cast<LPARAM>(TARGET_CLASSNAME));

	
}
// 回调函数：枚举窗口
BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam)
{
	
	const wchar_t* targetClassName = reinterpret_cast<const wchar_t*>(lParam); // 目标类名

	wchar_t className[256] = { 0 };
	GetClassNameW(hwnd, className, 256); // 获取窗口类名

	// 如果类名匹配
	if (wcscmp(className, targetClassName) == 0)
	{
		QWORD pid;
		GetWindowThreadProcessId(hwnd, (LPDWORD)&pid);
		HMODULE hMod = GetRemoteModuleHandle(static_cast<DWORD>(pid), DLL_NAME);
		bool isInject;
		if (hMod) {
			isInject = true;
		}
		else {
			isInject = false;
		}

		wchar_t windowTitle[256] = { 0 };
		GetWindowTextW(hwnd, windowTitle, 256); // 获取窗口标题
		if (wcslen(windowTitle) > 0)
		{
		
			//std::wcout << L"窗口标题: " << windowTitle << std::endl;
			CDLL注入器Dlg* pDlg = reinterpret_cast<CDLL注入器Dlg*>(AfxGetMainWnd());


			if (pDlg != nullptr)
			{
		
				pDlg->addString(windowTitle, hwnd, isInject);
				//hwndMap.insert(std::make_pair(GetWindowThreadProcessId(hwnd, NULL), hwnd));

			}

			//m_ListBox.setItemData();
		}
	}

	return TRUE; // 继续枚举
}

void CDLL注入器Dlg::PostNcDestroy()
{
	// TODO: 在此添加专用代码和/或调用基类
	g_exitFLag = TRUE;


	//if (!arry_pipeServer.empty())
	//{
	//	for (size_t i = 0; i < arry_pipeServer.size() / 2; )
	//	{
	//		UnloadInjectedDLL(static_cast<DWORD>(arry_pipeServer[i]), DLL_NAME);
	//		i = i + 2;
	//	}
	//}

	pipeServer.~PipeServer();

	if (g_hPipeThread != NULL) {
		WaitForSingleObject(g_hPipeThread, INFINITE); // 等待线程安全退出
		CloseHandle(g_hPipeThread);
		g_hPipeThread = NULL;
	}


	CDialogEx::PostNcDestroy();

	
}
