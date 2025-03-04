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