// Viewer_packet.cpp: 实现文件
//

#include "pch.h"
#include "afxdialogex.h"
#include "controller.h"
#include "Log.h"
#include "normalFun.h"
#include "Viewer_packet.h"
#include "zxhelper.h"

std::vector<CString> Viewer_packet::cSKILL_NAME_ARRY;
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
	DDX_Control(pDX, IDC_COMBO1, m_combox);
	DDX_Control(pDX, IDC_LIST5, m_CListCtrl);
	DDX_Control(pDX, IDC_LIST6, m_CListCtrlSkill);
}


BEGIN_MESSAGE_MAP(Viewer_packet, CDialogEx)
	ON_LBN_SELCHANGE(IDC_LIST1, &Viewer_packet::OnLbnSelchangeList1)
	ON_LBN_DBLCLK(IDC_LIST1, &Viewer_packet::OnDblclkList1)
	ON_CBN_SELCHANGE(IDC_COMBO1, &Viewer_packet::OnCbnSelchangeCombo1)
	ON_BN_CLICKED(IDC_BUTTON1, &Viewer_packet::OnBnClickedButton1)
	ON_NOTIFY(NM_RDBLCLK, IDC_LIST5, &Viewer_packet::OnRdblclkList5)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST5, &Viewer_packet::OnColumnclickList5)
	ON_BN_CLICKED(IDC_BUTTON4, &Viewer_packet::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON6, &Viewer_packet::OnBnClickedButton6)
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
	
	

	initObject();

	initPerson();

	iniSkillName();

	if (cSKILL_NAME_ARRY.empty())
	{
		AfxMessageBox(_T("初始化skillNames失败!"));

	}

	

	//std::qsort(sort_items, count, sizeof(CString), CompareFirstColumn);

	/*for (int j = 0; j < count; j++)
	{
		m_ListBox.AddString(sort_items[j]);

	}*/
	// 释放动态分配的内存
	//delete[] sort_items;

}

void Viewer_packet::initObject() {

	m_combox.AddString(_T("call调用"));
	//m_combox.SetItemData(0, 0);
	m_combox.AddString(_T("封包调用"));
	//m_combox.SetItemData(1, 1);
	m_combox.SetCurSel(0); //索引从0开始

	showObject();

}

void Viewer_packet::showObject() {

	std::vector<T包裹物品属性> objs;

	objs = traversePackage();
	
	size_t count = objs.size();
	if (count == 0)
	{
		AfxMessageBox(_T("读取包裹失败了!"));
		//EndDialog(IDOK);不关闭窗口
		return;
	}

	//CString* sort_items = new CString[count];
	int i = 0;
	for (T包裹物品属性& obj : objs)
	{
		CString str;
		str.AppendFormat(_T(" %d "), (UINT)obj.q所在格数);
		str.Append(obj.p物品名称);
		str.AppendFormat(_T(" %d %X "), (UINT)obj.q数量, (UINT)obj.q物品ID);
		//sort_items[i]=str;
		// 指针传参 会被销毁啊
		//DWORD useObj[2] = {(int)obj.q所在格数 ,(int)obj.d物品ID};
		//m_ListBox.SetItemData(i, (DWORD_PTR)&useObj);
		QWORD qwordData = ((obj.q所在格数 << 32) | (obj.q物品ID));
		m_ListBox.AddString(str);
		m_ListBox.SetItemData(i, qwordData);
		i++;
	}
}

void Viewer_packet::initPerson() {
	// 设置扩展样式
	m_CListCtrl.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
	// 设置 ListCtrl 风格
	m_CListCtrl.ModifyStyle(0, LVS_REPORT | LVS_SINGLESEL | LVS_SHOWSELALWAYS);
	// 启用列标题点击排序
	m_CListCtrl.SetExtendedStyle(m_CListCtrl.GetExtendedStyle() | LVS_EX_HEADERDRAGDROP);

	m_CListCtrl.InsertColumn(0, _T("等级"), LVCFMT_LEFT, 50);
	m_CListCtrl.InsertColumn(1, _T("血量"), LVCFMT_LEFT, 70);
	m_CListCtrl.InsertColumn(2, _T("距离"), LVCFMT_LEFT, 50);
	m_CListCtrl.InsertColumn(3, _T("角色ID"), LVCFMT_LEFT, 100);

	showPerson();
}

struct PersonData {
	DWORD* p坐标ptr;  // 假设是坐标指针
	DWORD dID;      // 假设是ID
};
void Viewer_packet::showPerson() {
	std::vector<T人物属性> persons = traversePerson();
	m_CListCtrl.DeleteAllItems();

	for (T人物属性 person : persons)
	{
		
		int nIndex = m_CListCtrl.GetItemCount(); // 获取当前行数
		m_CListCtrl.InsertItem(nIndex, _T("")); // 插入新行
		CString str;
		str.Format(_T("%d"), (UINT)person.d等级);
		m_CListCtrl.SetItemText(nIndex, 0, str); // 设置第1列

		str.Format(_T("%d"), (UINT)person.d血量);
		m_CListCtrl.SetItemText(nIndex, 1, str); // 设置第2列

		str.Format(_T("%.1f"), person.f距离);
		m_CListCtrl.SetItemText(nIndex, 2, str); // 设置第3列

		str.Format(_T("%08X"), person.dID);

		m_CListCtrl.SetItemText(nIndex, 3, str); // 设置第4列
		// 定义结构体
		

		// 存储数据
		PersonData* pData = new PersonData{ person.p坐标ptr, person.dID };
		m_CListCtrl.SetItemData(nIndex, reinterpret_cast<DWORD_PTR>(pData));

	/*	DWORD_PTR data[] = { (DWORD_PTR)person.p坐标ptr ,person.dID };
		m_CListCtrl.SetItemData(nIndex, reinterpret_cast<DWORD_PTR>(data));*/



		//m_CListCtrl.UpdateData();
	}
}

void Viewer_packet::iniSkillName() {

	m_CListCtrlSkill.InsertColumn(0, _T("技能名称"), LVCFMT_LEFT, 120);
	m_CListCtrlSkill.InsertColumn(1, _T("技能ID"), LVCFMT_LEFT, 50);
	m_CListCtrlSkill.InsertColumn(2, _T("CD"), LVCFMT_LEFT, 55);
	m_CListCtrlSkill.InsertColumn(3, _T("最大CD"), LVCFMT_LEFT, 55);
	m_CListCtrlSkill.InsertColumn(4, _T("技能对象地址"), LVCFMT_LEFT, 100);

	CStringA str = LoadTextFromResource(IDR_TXT1);

	CString content = UTF8ToUnicode((const char*)str.GetString());

	if (!content.IsEmpty())
	{
		// 处理TXT文件内容
		std::vector<CString> filteredLines;

		// 按行分割内容
		int start = 0;
		CString line = content.Tokenize(_T("\n"), start);
		while (!line.IsEmpty())
		{
			// 去除行首尾的空白字符
			line.Trim();

			// 检查行是否为空
			if (line.IsEmpty())
			{
				line = content.Tokenize(_T("\n"), start);
				continue;
			}

			// 解析第一列的数字
			int number = 0;
			CString text;
			if (swscanf_s(line, _T("%d %[^\n]"), &number, text.GetBuffer(256), 256) == 2)
			{
				text.ReleaseBuffer();

				// 检查数字是否以0结尾
				if (number % 10 == 0)
				{
					// 符合条件，添加到过滤后的列表中

					//Call_输出调试信息W(_T("添加:%s"), line.GetString());
					filteredLines.push_back(line);
				}
			}

			// 继续处理下一行
			line = content.Tokenize(_T("\n"), start);
		}
		cSKILL_NAME_ARRY = filteredLines;
		showSkillName();
	}
	return;

}

void Viewer_packet::showSkillName() {

	std::vector<T技能> skills = traverseSkill();

	for (T技能& skill : skills) {

		int nIndex = m_CListCtrlSkill.GetItemCount(); // 获取当前行数
	
		m_CListCtrlSkill.InsertItem(nIndex, _T(""));

		CString str;
		//str.Format(_T("%s"), skill.p技能名称);
		m_CListCtrlSkill.SetItemText(nIndex, 0, skill.p技能名称.GetString()); // 设置第1列

		str.Format(_T("%X"), skill.d技能ID);
		m_CListCtrlSkill.SetItemText(nIndex, 1, str); // 设置第2列

		str.Format(_T("%d"), skill.d技能冷却时间);
		m_CListCtrlSkill.SetItemText(nIndex, 2, str); // 设置第3列

		str.Format(_T("%d"), skill.d技能最大冷却时间);
		m_CListCtrlSkill.SetItemText(nIndex, 3, str); // 设置第4列

		str.Format(_T("%X"), skill.d技能对象);
		m_CListCtrlSkill.SetItemText(nIndex, 4, str); // 设置第4列
	

	}


}

void Viewer_packet::OnLbnSelchangeList1()
{
	// TODO: 在此添加控件通知处理程序代码
}
// 使用物品
void Viewer_packet::OnDblclkList1()
{
	// TODO: 在此添加控件通知处理程序代码
	int selectedIndex = m_ListBox.GetCurSel();
	if (selectedIndex != LB_ERR) { // 确保有项被选中
		QWORD ret = m_ListBox.GetItemData(selectedIndex);
		
		useObj(ret >> 32, ret & 0xFFFFFFFF, m_combox.GetCurSel());
		/*DWORD* useObjArry;
		useObjArry = (DWORD*)m_ListBox.GetItemData(selectedIndex);
		useObj(useObjArry[0], useObjArry[1]);*/
	}
}

void Viewer_packet::OnCbnSelchangeCombo1()
{
	// TODO: 在此添加控件通知处理程序代码
}
// 刷新按钮被单击
void Viewer_packet::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	showPerson();

	

}
// 右键双击选中
void Viewer_packet::OnRdblclkList5(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	  // 获取双击的项索引
	int nItem = pNMItemActivate->iItem;

	if (nItem >= 0)
	{
		// 获取该项的文本
		CString strItemText = m_CListCtrl.GetItemText(nItem, 3);
		Call_输出调试信息("%S", strItemText);
		TCHAR* pEnd;
		DWORD dwValue = _tcstoul(strItemText, &pEnd, 16);
		if (*pEnd != _T('\0')) {
			// 转换失败
			Call_输出调试信息("错误：CString-->DWORD转化失败");
			return;
		}
	
		selectObj(dwValue);

		//AfxMessageBox(_T("你双击了: ") + strItemText);
	}



	*pResult = 0;





}
// 单击列排序
void Viewer_packet::OnColumnclickList5(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	// 获取点击的列索引
	int nColumn = pNMLV->iSubItem;
	Call_输出调试信息("nColumn :  %d", nColumn);
	// 如果点击的是当前排序列，切换排序顺序
	if (nColumn == m_nSortColumn) {
		m_bAscending = !m_bAscending;
	}
	else {
		// 否则，设置新的排序列，并默认升序
		m_nSortColumn = nColumn;
		m_bAscending = true;
	}
	Call_输出调试信息("调用SortListCtrl");
	// 调用排序函数
	SortListCtrl(nColumn, m_bAscending);




	*pResult = 0;
}
// 排序函数
void Viewer_packet::SortListCtrl(int nColumn, bool bAscending)
{
	// 设置排序回调函数
	m_CListCtrl.SortItemsEx(CompareFunc, (LPARAM)this);
}
// 排序回调函数
int CALLBACK Viewer_packet::CompareFunc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{

	Viewer_packet* pDialog = (Viewer_packet*)lParamSort;
	CListCtrl* pListCtrl = &(pDialog->m_CListCtrl);

	// 获取项文本
	CString strItem1 = pListCtrl->GetItemText((int)lParam1, pDialog->m_nSortColumn);
	CString strItem2 = pListCtrl->GetItemText((int)lParam2, pDialog->m_nSortColumn);



	////这是首数字开始排序
	// 比较文本
	//int nCompare = strItem1.CompareNoCase(strItem2);

	
	

	int x1 = _tstoi(strItem1.GetBuffer());
	int x2 = _tstoi(strItem2.GetBuffer());
	int result = 0;
	if ((x1 - x2) < 0)
		result = -1;
	else if ((x1 - x2) == 0)
		result = 0;
	else
		result = 1;
	// 根据排序顺序调整比较结果
	if (!pDialog->m_bAscending) {
		result = -result;
	}
	return result;


}
// 脚底瞬移
void Viewer_packet::OnBnClickedButton4()
{
	// TODO: 在此添加控件通知处理程序代码
	int nSelectedIndex = m_CListCtrl.GetSelectionMark();
	if (nSelectedIndex != -1) {
		//CString strItemText = m_CListCtrl.GetItemText(nSelectedIndex, 0);  // 获取选中项的文本

		// 读取数据
		PersonData* pRetrieved = reinterpret_cast<PersonData*>(m_CListCtrl.GetItemData(nSelectedIndex));
		//TRACE("ID: %d, 坐标指针: %p\n", pRetrieved->dID, pRetrieved->p坐标ptr);

		//DWORD_PTR ptr = m_CListCtrl.GetItemData(nSelectedIndex);
		Call_输出调试信息("脚底瞬移对象坐标指针:%p", pRetrieved->p坐标ptr);
		teleportation((DWORD)(pRetrieved->p坐标ptr));
	}
	else {
		AfxMessageBox(_T("没有选中人物对象"));
	}

}
// 一键跟随目标
void Viewer_packet::OnBnClickedButton6()
{
	// TODO: 在此添加控件通知处理程序代码
	int nSelectedIndex = m_CListCtrl.GetSelectionMark();
	if (nSelectedIndex != -1) {
		//CString strItemText = m_CListCtrl.GetItemText(nSelectedIndex, 0);  // 获取选中项的文本
		PersonData* pRetrieved = reinterpret_cast<PersonData*>(m_CListCtrl.GetItemData(nSelectedIndex));

		//DWORD_PTR ptr = m_CListCtrl.GetItemData(nSelectedIndex);
		Call_输出调试信息("一键跟随目标ID:%p", pRetrieved->dID);
		follow(pRetrieved->dID);
	}
	else {
		AfxMessageBox(_T("没有选中人物对象"));
	}
}
