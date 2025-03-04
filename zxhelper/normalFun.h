#pragma once
#include <Windows.h>

QWORD SafeReadQWORD(QWORD* ptr);

DWORD SafeReadDWORD(DWORD* ptr);

//QWORD ReadChain(QWORD base, const std::vector<QWORD> offsets);

int CompareFirstColumn(const void* a, const void* b);

int ExtractNumber(const CString& str);



bool IsValidPointer(void* ptr, size_t size);

