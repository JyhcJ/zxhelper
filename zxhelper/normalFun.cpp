#include "pch.h"
#include "normalFun.h"
#include "zxhelper.h"
#include <iostream>
#include <vector>
//#include "afxdialogex.h"

//安全返回参数地址的值
QWORD SafeReadQWORD(QWORD* ptr) {
	if (ptr == nullptr || !IsValidPointer(ptr, sizeof(QWORD))) {
		return -10; // 返回默认值或错误码
	}
	return *(QWORD*)ptr;
}
DWORD SafeReadDWORD(DWORD* ptr) {
	if (ptr == nullptr || !IsValidPointer(ptr, sizeof(DWORD))) {
		return -30; // 返回默认值或错误码
	}
	return *(DWORD*)ptr;
}

// 通用的 SafeRead 函数
template <typename T>
T SafeRead(T* ptr) {
	if (ptr == nullptr || !IsValidPointer(ptr, sizeof(T))) {
		return static_cast<T>(-100); // 返回默认值或错误码
	}
	return *ptr;
}

//QWORD ReadChain(QWORD base,std::vector<QWORD> offsets) {
//    QWORD value = base;
//    for (QWORD offset : offsets) {
//        value = SafeReadQWORD((QWORD*)(value + offset));
//        if (value == -10) {
//            break; // 读取失败
//        }
//    }
//    return value;
//}

void ReadUtf8FileAndSplitByNewline(CStringArray& lines)
{
	// 创建文件选择对话框
	CFileDialog fileDlg(TRUE);  // TRUE 表示打开文件对话框
	fileDlg.m_ofn.lpstrTitle = _T("选择文件");  // 设置对话框标题
	fileDlg.m_ofn.lpstrFilter = _T("文本文件 (*.txt)\0*.txt\0所有文件 (*.*)\0*.*\0");  // 设置文件过滤器

	CString filePath;
	if (fileDlg.DoModal() == IDOK)
	{
		filePath = fileDlg.GetPathName();  // 获取用户选择的文件路径
		if (filePath.IsEmpty())
		{
			AfxMessageBox(_T("未选择文件！"));
			return ;
		}
	}
	else
	{
		return; // 用户取消选择
	}

	// 打开文件
	CStdioFile file;
	if (!file.Open(filePath, CFile::modeRead | CFile::typeBinary))
	{
		AfxMessageBox(_T("无法打开文件！"));
		return ;
	}

	// 获取文件大小
	ULONGLONG fileSize = file.GetLength();
	if (fileSize == 0)
	{
		AfxMessageBox(_T("文件为空！"));
		file.Close();
		return ;
	}

	// 读取文件内容
	CStringA contentA;
	char* buffer = contentA.GetBuffer(static_cast<int>(fileSize));
	file.Read(buffer, static_cast<UINT>(fileSize));
	contentA.ReleaseBuffer();
	file.Close();

	// 检查并跳过 UTF-8 BOM（如果存在）
	const char* pData = contentA.GetString();
	if (fileSize >= 3 && static_cast<unsigned char>(pData[0]) == 0xEF &&
		static_cast<unsigned char>(pData[1]) == 0xBB &&
		static_cast<unsigned char>(pData[2]) == 0xBF)
	{
		pData += 3; // 跳过 BOM
		contentA = pData; // 重新赋值
	}

	// 将 UTF-8 转换为 Unicode
	int length = MultiByteToWideChar(CP_UTF8, 0, contentA, -1, nullptr, 0);
	if (length <= 0)
	{
		AfxMessageBox(_T("文件内容转换失败！"));
		return ;
	}

	CString contentW;
	MultiByteToWideChar(CP_UTF8, 0, contentA, -1, contentW.GetBuffer(length), length);
	contentW.ReleaseBuffer();

	// 按换行符分割内容
	int start = 0;
	int end = contentW.Find(_T("\n")); // 查找换行符

	while (end != -1)
	{
		CString line = contentW.Mid(start, end - start); // 提取一行
		line.TrimRight(_T("\r")); // 去除可能的回车符
		lines.Add(line); // 添加到数组
		start = end + 1; // 移动到下一行
		end = contentW.Find(_T("\n"), start); // 继续查找换行符
	}

	// 添加最后一行（如果没有换行符结尾）
	if (start < contentW.GetLength())
	{
		CString line = contentW.Mid(start);
		line.TrimRight(_T("\r")); // 去除可能的回车符
		lines.Add(line);
	}

	// 输出分割后的内容
	for (int i = 0; i < lines.GetSize(); i++)
	{
		//AfxMessageBox(lines[i]); // 弹出消息框显示每一行
		//Call_输出调试信息("调试信息:str = %s", lines[i]);
	}
	
}
// 比较函数，用于CString的qsort
// 自定义比较函数
int CompareFirstColumn(const void* a, const void* b) {
	// 将 void* 转换为 CString*
	const CString* strA = static_cast<const CString*>(a);
	const CString* strB = static_cast<const CString*>(b);
	int curPos = 0;
	// 提取第一列的整数
	int numA = _ttoi(strA->Tokenize(_T(" "), curPos)); // 提取第一个空格前的部分并转换为整数
	int numB = _ttoi(strB->Tokenize(_T(" "), curPos)); // 提取第一个空格前的部分并转换为整数

	// 按第一列的整数升序排序
	return numA - numB;
}
// 提取字符串开头的数字
int ExtractNumber(const CString& str) {
	// 将 CString 转换为 const char*
	CStringA strA(str); // 转换为多字节字符串
	const char* cstr = strA;

	// 使用 strtok_s 分割字符串
	char* context = nullptr; // 用于存储 strtok_s 的状态
	char* token = strtok_s((char*)cstr, " ", &context); // 按空格分割
	if (token != nullptr) {
		return atoi(token); // 将第一个部分转换为整数
	}
	return 0; // 如果没有数字，返回 0
}

bool IsValidPointer(void* ptr, size_t size) {
	return !IsBadReadPtr(ptr, size);
}

void Call_输出调试信息(char* pszFormat, ...)
{
#ifdef _DEBUG

	char szbufFormat[0x1000];
	char szbufFormat_Game[0x1100] = "";
	va_list argList;
	va_start(argList, pszFormat);//参数列表初始化
	vsprintf_s(szbufFormat, pszFormat, argList);
	strcat_s(szbufFormat_Game, szbufFormat);
	OutputDebugStringA(szbufFormat_Game);
	va_end(argList);

#endif
}