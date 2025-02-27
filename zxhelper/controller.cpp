#include "pch.h"
#include "controller.h"

extern "C" void call_relex();

//属性基址
const QWORD BASE_ADDRESS_ATT = 0x14171CFD8;
const QWORD BASE_ADDRESS_NAME = 0x7FFC440F4978;
const int  OFFSET_QIXUE = 0x558;

void relex() {
	call_relex();
}

wchar_t* getName() {
	if (isString(reinterpret_cast<const char*>(BASE_ADDRESS_NAME), 20)){
		return  (wchar_t*)(BASE_ADDRESS_NAME + 0);
	}
	else {
		const char* str = "error";
		return (wchar_t*) str;
	}

}

QWORD getAtt_TBaseAddress() {
	MessageBox(NULL, L"123", NULL, 0U);
	try
	{
		QWORD value = *(QWORD*)BASE_ADDRESS_ATT;
		value = *(QWORD*)(value + 0x38);
		value = *(QWORD*)(value + 0x10);
		value = *(QWORD*)(value + 0x48);
		value = *(QWORD*)(value + 0x10);
		value = *(QWORD*)(value + 0x60);
		return value;
	}
	catch (...)
	{
		MessageBox(NULL, L"读取属性基地址异常", NULL, 0U);
		exit(1);
	}


}

QWORD getAtt_qixue() {
	return  *(QWORD*)(getAtt_TBaseAddress() + OFFSET_QIXUE);
}

QWORD getAtt_zhenqi() {
	return *(QWORD*)(getAtt_TBaseAddress() + OFFSET_QIXUE+ 0x8);
}

QWORD getAtt_yuanli() {
	return *(QWORD*)(getAtt_TBaseAddress() + OFFSET_QIXUE + 0xC);
}

QWORD getAtt_wushi() {
	return *(QWORD*)(getAtt_TBaseAddress() + OFFSET_QIXUE + 0x450C);
}

QWORD getAtt_jianmian() {
	return *(QWORD*)(getAtt_TBaseAddress() + OFFSET_QIXUE + 0x450C-0x8);
}
	
bool isString(const char* ptr, size_t maxLength = 20) {
	MessageBox(NULL, L"5", NULL, 0U);
	if (ptr == nullptr) {
		MessageBox(NULL, L"8", NULL, 0U);
		return false;  // 空指针不是字符串
	}

	for (size_t i = 0; i < maxLength; ++i) {
		MessageBox(NULL, L"6", NULL, 0U);
		if (ptr[i] == '\0') {
			MessageBox(NULL, L"11", NULL, 0U);
			return true;  // 遇到空字符，说明是字符串
		}
	}
	MessageBox(NULL, L"7", NULL, 0U);
	return false;  // 未找到空字符，可能不是字符串
}

