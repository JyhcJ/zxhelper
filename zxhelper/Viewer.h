#pragma once
#include "afxdialogex.h"
#include "Log.h"


// C_viewer 对话框

class Viewer : public CDialogEx
{
	DECLARE_DYNAMIC(Viewer)

public:
	Viewer(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~Viewer();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	
	afx_msg void OnBnClickedButton1();
	CStatic show_wushi;
	CStatic show_qixue;
	virtual BOOL OnInitDialog();// 重写 OnInitDialog
	void init();
	virtual void OnDestroy();
	CStatic show_name;
	CStatic show_jianmian;
	CStatic show_yuanli;
	CStatic show_zhenqi;
	afx_msg void OnBnClickedButton2();

};
