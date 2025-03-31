// testDialog.cpp: 实现文件
//

#include "pch.h"
#include "DLL注入器.h"
#include "afxdialogex.h"
#include "testDialog.h"
#include <iostream>


// testDialog 对话框

IMPLEMENT_DYNAMIC(testDialog, CDialogEx)

testDialog::testDialog(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG1, pParent)
{

}

testDialog::~testDialog()
{
}

void testDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(testDialog, CDialogEx)
END_MESSAGE_MAP()
void ReadUtf8FileAndSplitByNewline()
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
			return;
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
		return;
	}

	// 获取文件大小
	ULONGLONG fileSize = file.GetLength();
	if (fileSize == 0)
	{
		AfxMessageBox(_T("文件为空！"));
		file.Close();
		return;
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
		return;
	}

	CString contentW;
	MultiByteToWideChar(CP_UTF8, 0, contentA, -1, contentW.GetBuffer(length), length);
	contentW.ReleaseBuffer();

	// 按换行符分割内容
	CStringArray lines; // 存储分割后的行
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
		AfxMessageBox(lines[i]); // 弹出消息框显示每一行
	}
}

CStringW UTF8ToUnicode(const char* utf8Str)
{
	if (!utf8Str || *utf8Str == '\0')
	{
		return CStringW();
	}
	std::cout << utf8Str << std::endl;
	// 计算需要的宽字符数
	int wideCharCount = MultiByteToWideChar(CP_UTF8, 0, utf8Str, -1, nullptr, 0);
	if (wideCharCount == 0)
	{
		return CStringW();
	}

	// 分配缓冲区
	CStringW unicodeStr;
	wchar_t* buffer = unicodeStr.GetBuffer(wideCharCount);

	// 转换 UTF-8 到 UTF-16
	MultiByteToWideChar(CP_UTF8, 0, utf8Str, -1, buffer, wideCharCount);

	// 释放缓冲区
	unicodeStr.ReleaseBuffer();

	return unicodeStr;
}
// testDialog 消息处理程序
