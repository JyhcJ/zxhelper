// C_viewer.cpp: 实现文件
//

#include "pch.h"
#include "zxhelper.h"
#include "afxdialogex.h"
#include "Viewer.h"
#include "controller.h"


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



END_MESSAGE_MAP()

BOOL Viewer::OnInitDialog() {
	CDialogEx::OnInitDialog();  // 调用基类的 OnInitDialog
	// 在控件初始化后调用 init
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
	float percentage;
	show_name.SetWindowTextW(getName());

	str.Format(_T("%d"), (QWORD)getAtt_qixue());
	show_qixue.SetWindowTextW(str);

	str.Format(_T("%d"), getAtt_zhenqi());
	show_zhenqi.SetWindowTextW(str);

	str.Format(_T("%d"), getAtt_yuanli());
	show_yuanli.SetWindowTextW(str);

	percentage = getAtt_wushi() / 10.0;
	str.Format(_T("%.1f"), percentage);
	str.Append(_T("%"));
	show_wushi.SetWindowTextW(str);

	percentage = getAtt_jianmian() / 10.0;
	str.Format(_T("%.1f"), percentage);
	str.Append(_T("%"));
	show_jianmian.SetWindowTextW(str);
}