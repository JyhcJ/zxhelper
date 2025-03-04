#include "pch.h"
#include "afxdialogex.h"
#include "controller.h"
#include "Viewer.h"
#include "zxhelper.h"
#include "Viewer_packet.h"
#include "Log.h"
//#include "T_obj.h"


// C_viewer 对话框

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
}


BEGIN_MESSAGE_MAP(Viewer, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &Viewer::OnBnClickedButton1)



	ON_BN_CLICKED(IDC_BUTTON2, &Viewer::OnBnClickedButton2)
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

void Viewer::OnDestroy() {
	g_Log.Write(_T("Application exited."));
	CloseLog(); // 关闭日志
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

