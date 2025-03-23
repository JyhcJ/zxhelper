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

float SafeReadFloat(DWORD* ptr) {
	if (ptr == nullptr || !IsValidPointer(ptr, sizeof(float))) {
		return -20; // ����Ĭ��ֵ�������
	}
	return *(float*)ptr;
}

QWORD SafeReadQWORD_Arry_NotLast(std::vector<QWORD> arry) {
	size_t i = arry.size();
	QWORD temp = 0;
	QWORD ret ;
	for (QWORD offset : arry)
	{
		i--;
		if (i != 0)
		{
			temp = SafeReadQWORD((QWORD*)(temp + offset));
		}
		else {
			ret = temp + offset;
		}
	}
	return ret;
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
	// ����̶�ǰ׺�ַ���
	const char* prefix = "[������Ϣ] ";
	char szbufFormat[0x1000];
	char szbufFormat_Game[0x1100] = "";
	// ���̶�ǰ׺ƴ�ӵ� szbufFormat_Game
	strcat_s(szbufFormat_Game, prefix);
	va_list argList;
	va_start(argList, pszFormat);//�����б��ʼ��
	vsprintf_s(szbufFormat, pszFormat, argList);
	strcat_s(szbufFormat_Game, szbufFormat);
	OutputDebugStringA(szbufFormat_Game);
	va_end(argList);

#endif
}
void Call_���������ϢW(const wchar_t* pszFormat, ...)
{
#ifdef _DEBUG
	// ����̶�ǰ׺�ַ���
	const wchar_t* prefix = L"[������Ϣ] ";
	wchar_t szbufFormat[0x1000];
	wchar_t szbufFormat_Game[0x1100] = L"";

	// ���̶�ǰ׺ƴ�ӵ� szbufFormat_Game
	wcscat_s(szbufFormat_Game, prefix);

	// ����ɱ����
	va_list argList;
	va_start(argList, pszFormat); // �����б��ʼ��
	vswprintf_s(szbufFormat, pszFormat, argList); // ��ʽ�����ַ��ַ���
	wcscat_s(szbufFormat_Game, szbufFormat); // ƴ�ӽ��
	OutputDebugStringW(szbufFormat_Game); // ��� UTF-16 ������Ϣ
	va_end(argList);
#endif
}
// ����һ��������������ά�ռ�������֮��ľ���
float calculateDistance(DWORD* myptr, DWORD* objptr) {
	Call_���������Ϣ("1:%p ,,,2:%p ", myptr, objptr);
	// ���������ֵ
	float dx = SafeReadFloat(myptr++) - SafeReadFloat(objptr++);
	float dz = SafeReadFloat(myptr++) - SafeReadFloat(objptr++);
	float dy = SafeReadFloat(myptr) - SafeReadFloat(objptr);
	// �������
	float distance = sqrt(pow(dx, 2) + pow(dy, 2) + pow(dz, 2));
	Call_���������Ϣ("%f", distance);
	return distance;
}

CStringA LoadTextFromResource(int resourceID)
{
	HRSRC hResource = FindResource(AfxGetResourceHandle(), MAKEINTRESOURCE(resourceID), _T("txt"));
	if (!hResource)
	{
		AfxMessageBox(_T("Failed to find resource!"));
		return ("");
	}

	HGLOBAL hMemory = LoadResource(AfxGetResourceHandle(), hResource);
	if (!hMemory)
	{
		AfxMessageBox(_T("Failed to load resource!"));
		return ("");
	}

	DWORD size = SizeofResource(AfxGetResourceHandle(), hResource);
	LPVOID data = LockResource(hMemory);
	if (!data)
	{
		AfxMessageBox(_T("Failed to lock resource!"));
		return ("");
	}

	// ����Դ����ת��ΪCString
	CStringA text((const char*)data, size);
	return text;
}

CStringW UTF8ToUnicode(const char* utf8Str)
{
	if (!utf8Str || *utf8Str == '\0')
	{
		return CStringW();
	}

	// ������Ҫ�Ŀ��ַ���
	int wideCharCount = MultiByteToWideChar(CP_UTF8, 0, utf8Str, -1, nullptr, 0);
	if (wideCharCount == 0)
	{
		return CStringW();
	}

	// ���仺����
	CStringW unicodeStr;
	wchar_t* buffer = unicodeStr.GetBuffer(wideCharCount);

	// ת�� UTF-8 �� UTF-16
	MultiByteToWideChar(CP_UTF8, 0, utf8Str, -1, buffer, wideCharCount);

	// �ͷŻ�����
	unicodeStr.ReleaseBuffer();

	return unicodeStr;
}
