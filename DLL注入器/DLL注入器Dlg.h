
// DLL注入器Dlg.h: 头文件
//

#pragma once


BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam);



// CDLL注入器Dlg 对话框
class CDLL注入器Dlg : public CDialogEx
{
// 构造
public:
	CDLL注入器Dlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLL_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedButton1();
	CEdit show_edit;
	afx_msg void OnBnClickedButton2();
	void addString(wchar_t* windowTitle, HWND hwnd, bool isInject);

	void myInit();
	CListBox m_ListBox;
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton4();
	virtual void PostNcDestroy();
};
