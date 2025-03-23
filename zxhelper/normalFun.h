#pragma once
#include <vector>
#include <Windows.h>
QWORD SafeReadQWORD(QWORD* ptr);

float SafeReadFloat(float* ptr);

QWORD SafeReadQWORD_Arry_NotLast(std::vector<QWORD> arry);

DWORD SafeReadDWORD(DWORD* ptr);

//QWORD ReadChain(QWORD base, const std::vector<QWORD> offsets);

void ReadUtf8FileAndSplitByNewline(CStringArray& lines);

int CompareFirstColumn(const void* a, const void* b);

int ExtractNumber(const CString& str);

bool IsValidPointer(void* ptr, size_t size);


// 自定义中%s 和 %S一样的
void Call_输出调试信息(char* pszFormat, ...);

void Call_输出调试信息W(const wchar_t* pszFormat, ...);

float calculateDistance(DWORD* myptr, DWORD* objptr);

CStringA LoadTextFromResource(int resourceID);

CStringW UTF8ToUnicode(const char* utf8Str);



