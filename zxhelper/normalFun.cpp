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