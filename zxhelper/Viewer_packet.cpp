// Viewer_packet.cpp: 实现文件
//

#include "pch.h"
#include "afxdialogex.h"
#include "controller.h"
#include "Log.h"
#include "normalFun.h"
#include "Viewer_packet.h"
#include "zxhelper.h"


// Viewer_packet 对话框

IMPLEMENT_DYNAMIC(Viewer_packet, CDialogEx)

Viewer_packet::Viewer_packet(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG2, pParent)
{

}

Viewer_packet::~Viewer_packet()
{
}

void Viewer_packet::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_ListBox);
}


BEGIN_MESSAGE_MAP(Viewer_packet, CDialogEx)
	ON_LBN_SELCHANGE(IDC_LIST1, &Viewer_packet::OnLbnSelchangeList1)
	ON_LBN_DBLCLK(IDC_LIST1, &Viewer_packet::OnDblclkList1)
END_MESSAGE_MAP()


// Viewer_packet 消息处理程序
BOOL Viewer_packet::OnInitDialog() {
	CDialogEx::OnInitDialog();  // 调用基类的 OnInitDialog
	// 在控件初始化后调用 init
	init();
	return TRUE;  // 返回 TRUE，除非你将焦点设置到某个控件
}
extern CLog g_Log;
void Viewer_packet::init() {
	std::vector<T包裹物品属性> objs;
	objs = traversePackage();
	size_t count = objs.size();
	if (count == 0)
	{
		AfxMessageBox(_T("读取包裹失败了!"));
		EndDialog(IDOK);
		return;
	}

	//CString* sort_items = new CString[count];
	int i = 0;
	for (T包裹物品属性& obj : objs)
	{
		CString str;
		str.AppendFormat(_T(" %d "), (int)obj.q所在格数);
		str.Append(obj.p物品名称);
		str.AppendFormat(_T(" %d %X "), (int)obj.q数量, (int)obj.q物品ID);
		//sort_items[i]=str;
		// 指针传参 会被销毁啊
		//DWORD useObj[2] = {(int)obj.q所在格数 ,(int)obj.d物品ID};
		//m_ListBox.SetItemData(i, (DWORD_PTR)&useObj);
		QWORD qwordData = ((obj.q所在格数 << 32) | (obj.q物品ID));
		m_ListBox.AddString(str);
		m_ListBox.SetItemData(i, qwordData);
		i++;

		
	}
	//std::qsort(sort_items, count, sizeof(CString), CompareFirstColumn);

	/*for (int j = 0; j < count; j++)
	{
		m_ListBox.AddString(sort_items[j]);

	}*/
	// 释放动态分配的内存
	//delete[] sort_items;

}



void Viewer_packet::OnLbnSelchangeList1()
{
	// TODO: 在此添加控件通知处理程序代码
}

void Viewer_packet::OnDblclkList1()
{
	// TODO: 在此添加控件通知处理程序代码
	int selectedIndex = m_ListBox.GetCurSel();
	if (selectedIndex != LB_ERR) { // 确保有项被选中
		QWORD ret = m_ListBox.GetItemData(selectedIndex);
		useObj(ret >> 32, ret & 0xFFFFFFFF);
		/*DWORD* useObjArry;
		useObjArry = (DWORD*)m_ListBox.GetItemData(selectedIndex);
		useObj(useObjArry[0], useObjArry[1]);*/
	}
}
