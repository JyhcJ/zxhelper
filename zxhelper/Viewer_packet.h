#pragma once
#include "afxdialogex.h"


// Viewer_packet 对话框

class Viewer_packet : public CDialogEx
{
	DECLARE_DYNAMIC(Viewer_packet)

public:
	Viewer_packet(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~Viewer_packet();

	// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG2 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg
		BOOL OnInitDialog();
	void init();

	int CloseWindow();
	void OnLbnSelchangeList1();
	CListBox m_ListBox;
	afx_msg void OnDblclkList1();
};
