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