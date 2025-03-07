
// DLL注入器Dlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "DLL注入器.h"
#include "DLL注入器Dlg.h"
#include "afxdialogex.h"

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
}

BEGIN_MESSAGE_MAP(CDLL注入器Dlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CDLL注入器Dlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON1, &CDLL注入器Dlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CDLL注入器Dlg::OnBnClickedButton2)
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

BOOL dll_inject(HANDLE processHandle, const char* dll) {
	LPVOID address = VirtualAllocEx(processHandle, NULL, 256, MEM_COMMIT, PAGE_READWRITE);
	if (address == NULL)
	{
		MessageBoxW(NULL, L"分配内存失败!", NULL, MB_OK);
		return FALSE;
	}

	BOOL ret = WriteProcessMemory(processHandle, address, dll, strlen(dll) + 1, NULL);
	if (ret == NULL)
	{
		MessageBoxW(NULL, L"写内存失败!", NULL, MB_OK);
		return FALSE;
	}
	HANDLE	threadHandle = CreateRemoteThread(processHandle, NULL, NULL, (LPTHREAD_START_ROUTINE)LoadLibraryA, address, NULL, NULL);
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

void CDLL注入器Dlg::injectP1(HWND handle) {
	DWORD pid;
	//Call_提升权限(TRUE);
	GetWindowThreadProcessId(handle, &pid);

	if (pid == NULL)
	{
		MessageBoxW(L"获取 ""pid 失败", NULL, MB_OK);
		return;
	}
	HANDLE process = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
	if (process == NULL)
	{
		MessageBoxW(L"打开 process 失败", NULL, MB_OK);
		return;
	}
	CString rString;
	show_edit.GetWindowTextW(rString);
	if (rString.IsEmpty())
	{
		dll_inject(process, "C:/Users/54469/Desktop/zxhelper/zxhelper/x64/Debug/zxhelper.dll");
		//dll_inject(process, "C:/Users/54469/Desktop/zxhelper/zxhelper/x64/Debug/定位工具/定位.dll");
	}
	else {
		dll_inject(process, (const char*)rString.GetString());
	}

}

void CDLL注入器Dlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnOK();
	HWND handle = ::FindWindow(NULL, L"诛仙");
	if (handle == NULL)
	{
		MessageBoxW(L"没有找到 诛仙 窗口", NULL, MB_OK);
		return;
	}
	injectP1(handle);
}

void CDLL注入器Dlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	HWND hwndNotepad = ::FindWindow(L"Notepad", NULL);
	injectP1(hwndNotepad);
}

void ReadUtf8FileAndSplitByNewline()
{
	// 创建文件选择对话框
	CFileDialog fileDlg(TRUE);  // TRUE 表示打开文件对话框
	fileDlg.m_ofn.lpstrTitle = _T("选择文件");  // 设置对话框标题
	fileDlg.m_ofn.lpstrFilter = _T("文本文件 (*.txt)\0*.txt\0所有文件 (*.*)\0*.*\0");  // 设置文件过滤器

	CString filePath;
	if (fileDlg.DoModal() == IDOK)
	{
		filePath = fileDlg.GetPathName();  // 获取用户选择的文件路径
		if (filePath.IsEmpty())
		{
			AfxMessageBox(_T("未选择文件！"));
			return;
		}
	}
	else
	{
		return; // 用户取消选择
	}

	// 打开文件
	CStdioFile file;
	if (!file.Open(filePath, CFile::modeRead | CFile::typeBinary))
	{
		AfxMessageBox(_T("无法打开文件！"));
		return;
	}

	// 获取文件大小
	ULONGLONG fileSize = file.GetLength();
	if (fileSize == 0)
	{
		AfxMessageBox(_T("文件为空！"));
		file.Close();
		return;
	}

	// 读取文件内容
	CStringA contentA;
	char* buffer = contentA.GetBuffer(static_cast<int>(fileSize));
	file.Read(buffer, static_cast<UINT>(fileSize));
	contentA.ReleaseBuffer();
	file.Close();

	// 检查并跳过 UTF-8 BOM（如果存在）
	const char* pData = contentA.GetString();
	if (fileSize >= 3 && static_cast<unsigned char>(pData[0]) == 0xEF &&
		static_cast<unsigned char>(pData[1]) == 0xBB &&
		static_cast<unsigned char>(pData[2]) == 0xBF)
	{
		pData += 3; // 跳过 BOM
		contentA = pData; // 重新赋值
	}

	// 将 UTF-8 转换为 Unicode
	int length = MultiByteToWideChar(CP_UTF8, 0, contentA, -1, nullptr, 0);
	if (length <= 0)
	{
		AfxMessageBox(_T("文件内容转换失败！"));
		return;
	}

	CString contentW;
	MultiByteToWideChar(CP_UTF8, 0, contentA, -1, contentW.GetBuffer(length), length);
	contentW.ReleaseBuffer();

	// 按换行符分割内容
	CStringArray lines; // 存储分割后的行
	int start = 0;
	int end = contentW.Find(_T("\n")); // 查找换行符

	while (end != -1)
	{
		CString line = contentW.Mid(start, end - start); // 提取一行
		line.TrimRight(_T("\r")); // 去除可能的回车符
		lines.Add(line); // 添加到数组
		start = end + 1; // 移动到下一行
		end = contentW.Find(_T("\n"), start); // 继续查找换行符
	}

	// 添加最后一行（如果没有换行符结尾）
	if (start < contentW.GetLength())
	{
		CString line = contentW.Mid(start);
		line.TrimRight(_T("\r")); // 去除可能的回车符
		lines.Add(line);
	}

	// 输出分割后的内容
	for (int i = 0; i < lines.GetSize(); i++)
	{
		AfxMessageBox(lines[i]); // 弹出消息框显示每一行
	}
}

void fun1(CString* ptr) {
	ptr->SetString(L"5");
}

void CDLL注入器Dlg::OnBnClickedButton2()
{

	//ReadUtf8FileAndSplitByNewline();
	CString* strNumber = new CString();
	fun1(strNumber);
	//AfxMessageBox(strNumber->GetString());

	int interval = _ttoi(strNumber->GetString()) * 1000;

	strNumber->Format(_T("%d"), interval);

	AfxMessageBox(*strNumber);

	delete strNumber;
}

