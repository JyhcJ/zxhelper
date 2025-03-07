#include "pch.h"
#include "normalFun.h"
#include "zxhelper.h"
#include <iostream>
#include <vector>
//#include "afxdialogex.h"

//��ȫ���ز�����ַ��ֵ
QWORD SafeReadQWORD(QWORD* ptr) {
	if (ptr == nullptr || !IsValidPointer(ptr, sizeof(QWORD))) {
		return -10; // ����Ĭ��ֵ�������
	}
	return *(QWORD*)ptr;
}
DWORD SafeReadDWORD(DWORD* ptr) {
	if (ptr == nullptr || !IsValidPointer(ptr, sizeof(DWORD))) {
		return -30; // ����Ĭ��ֵ�������
	}
	return *(DWORD*)ptr;
}

// ͨ�õ� SafeRead ����
template <typename T>
T SafeRead(T* ptr) {
	if (ptr == nullptr || !IsValidPointer(ptr, sizeof(T))) {
		return static_cast<T>(-100); // ����Ĭ��ֵ�������
	}
	return *ptr;
}

//QWORD ReadChain(QWORD base,std::vector<QWORD> offsets) {
//    QWORD value = base;
//    for (QWORD offset : offsets) {
//        value = SafeReadQWORD((QWORD*)(value + offset));
//        if (value == -10) {
//            break; // ��ȡʧ��
//        }
//    }
//    return value;
//}

void ReadUtf8FileAndSplitByNewline(CStringArray& lines)
{
	// �����ļ�ѡ��Ի���
	CFileDialog fileDlg(TRUE);  // TRUE ��ʾ���ļ��Ի���
	fileDlg.m_ofn.lpstrTitle = _T("ѡ���ļ�");  // ���öԻ������
	fileDlg.m_ofn.lpstrFilter = _T("�ı��ļ� (*.txt)\0*.txt\0�����ļ� (*.*)\0*.*\0");  // �����ļ�������

	CString filePath;
	if (fileDlg.DoModal() == IDOK)
	{
		filePath = fileDlg.GetPathName();  // ��ȡ�û�ѡ����ļ�·��
		if (filePath.IsEmpty())
		{
			AfxMessageBox(_T("δѡ���ļ���"));
			return ;
		}
	}
	else
	{
		return; // �û�ȡ��ѡ��
	}

	// ���ļ�
	CStdioFile file;
	if (!file.Open(filePath, CFile::modeRead | CFile::typeBinary))
	{
		AfxMessageBox(_T("�޷����ļ���"));
		return ;
	}

	// ��ȡ�ļ���С
	ULONGLONG fileSize = file.GetLength();
	if (fileSize == 0)
	{
		AfxMessageBox(_T("�ļ�Ϊ�գ�"));
		file.Close();
		return ;
	}

	// ��ȡ�ļ�����
	CStringA contentA;
	char* buffer = contentA.GetBuffer(static_cast<int>(fileSize));
	file.Read(buffer, static_cast<UINT>(fileSize));
	contentA.ReleaseBuffer();
	file.Close();

	// ��鲢���� UTF-8 BOM��������ڣ�
	const char* pData = contentA.GetString();
	if (fileSize >= 3 && static_cast<unsigned char>(pData[0]) == 0xEF &&
		static_cast<unsigned char>(pData[1]) == 0xBB &&
		static_cast<unsigned char>(pData[2]) == 0xBF)
	{
		pData += 3; // ���� BOM
		contentA = pData; // ���¸�ֵ
	}

	// �� UTF-8 ת��Ϊ Unicode
	int length = MultiByteToWideChar(CP_UTF8, 0, contentA, -1, nullptr, 0);
	if (length <= 0)
	{
		AfxMessageBox(_T("�ļ�����ת��ʧ�ܣ�"));
		return ;
	}

	CString contentW;
	MultiByteToWideChar(CP_UTF8, 0, contentA, -1, contentW.GetBuffer(length), length);
	contentW.ReleaseBuffer();

	// �����з��ָ�����
	int start = 0;
	int end = contentW.Find(_T("\n")); // ���һ��з�

	while (end != -1)
	{
		CString line = contentW.Mid(start, end - start); // ��ȡһ��
		line.TrimRight(_T("\r")); // ȥ�����ܵĻس���
		lines.Add(line); // ��ӵ�����
		start = end + 1; // �ƶ�����һ��
		end = contentW.Find(_T("\n"), start); // �������һ��з�
	}

	// ������һ�У����û�л��з���β��
	if (start < contentW.GetLength())
	{
		CString line = contentW.Mid(start);
		line.TrimRight(_T("\r")); // ȥ�����ܵĻس���
		lines.Add(line);
	}

	// ����ָ�������
	for (int i = 0; i < lines.GetSize(); i++)
	{
		//AfxMessageBox(lines[i]); // ������Ϣ����ʾÿһ��
		//Call_���������Ϣ("������Ϣ:str = %s", lines[i]);
	}
	
}
// �ȽϺ���������CString��qsort
// �Զ���ȽϺ���
int CompareFirstColumn(const void* a, const void* b) {
	// �� void* ת��Ϊ CString*
	const CString* strA = static_cast<const CString*>(a);
	const CString* strB = static_cast<const CString*>(b);
	int curPos = 0;
	// ��ȡ��һ�е�����
	int numA = _ttoi(strA->Tokenize(_T(" "), curPos)); // ��ȡ��һ���ո�ǰ�Ĳ��ֲ�ת��Ϊ����
	int numB = _ttoi(strB->Tokenize(_T(" "), curPos)); // ��ȡ��һ���ո�ǰ�Ĳ��ֲ�ת��Ϊ����

	// ����һ�е�������������
	return numA - numB;
}
// ��ȡ�ַ�����ͷ������
int ExtractNumber(const CString& str) {
	// �� CString ת��Ϊ const char*
	CStringA strA(str); // ת��Ϊ���ֽ��ַ���
	const char* cstr = strA;

	// ʹ�� strtok_s �ָ��ַ���
	char* context = nullptr; // ���ڴ洢 strtok_s ��״̬
	char* token = strtok_s((char*)cstr, " ", &context); // ���ո�ָ�
	if (token != nullptr) {
		return atoi(token); // ����һ������ת��Ϊ����
	}
	return 0; // ���û�����֣����� 0
}

bool IsValidPointer(void* ptr, size_t size) {
	return !IsBadReadPtr(ptr, size);
}

void Call_���������Ϣ(char* pszFormat, ...)
{
#ifdef _DEBUG

	char szbufFormat[0x1000];
	char szbufFormat_Game[0x1100] = "";
	va_list argList;
	va_start(argList, pszFormat);//�����б��ʼ��
	vsprintf_s(szbufFormat, pszFormat, argList);
	strcat_s(szbufFormat_Game, szbufFormat);
	OutputDebugStringA(szbufFormat_Game);
	va_end(argList);

#endif
}