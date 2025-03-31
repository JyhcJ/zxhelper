#pragma once
#include "afxdialogex.h"

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

	void thread_getAtt();

	void init();

	void  getAtt();

	CStatic show_name;
	CStatic show_jianmian;
	CStatic show_yuanli;
	CStatic show_zhenqi;
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();

	void OnStartThread();

	CButton m_btn3;
	CEdit m_edit1;
	virtual void PostNcDestroy();


	afx_msg void OnMove(int x, int y);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
//	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
