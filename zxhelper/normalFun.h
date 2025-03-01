#pragma once
#include <Windows.h>

QWORD SafeReadQWORD(QWORD* ptr);

DWORD SafeReadDWORD(DWORD* ptr);

//QWORD ReadChain(QWORD base, const std::vector<QWORD> offsets);

bool IsValidPointer(void* ptr, size_t size);
