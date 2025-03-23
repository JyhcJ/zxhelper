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
	static std::vector<CString> cSKILL_NAME_ARRY;

	void iniSkillName();

	void showSkillName();

	void init();

	void initObject();

	void showObject();

	void initPerson();

	void showPerson();

	int CloseWindow();
	void OnLbnSelchangeList1();
	CListBox m_ListBox;
	afx_msg void OnDblclkList1();
	CComboBox m_combox;
	afx_msg void OnCbnSelchangeCombo1();
	afx_msg void OnBnClickedButton1();
	CListCtrl m_CListCtrl;

	afx_msg void OnRdblclkList5(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnColumnclickList5(NMHDR* pNMHDR, LRESULT* pResult);
	void SortListCtrl(int nColumn, bool bAscending);
	static int CompareFunc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort);
	int m_nSortColumn;     // 当前排序列
	bool m_bAscending;     // 排序顺序（升序或降序）
	afx_msg void OnBnClickedButton4();
	CListCtrl m_CListCtrlSkill;
};
