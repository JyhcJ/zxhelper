
// DLL注入器Dlg.h: 头文件
//

#pragma once


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
	void injectP1(HWND handle);
	afx_msg void OnBnClickedButton2();
	CStringA LoadTextFromResource(int resourceID);
	CStringW UTF8ToUnicode(const char* utf8Str);
};
