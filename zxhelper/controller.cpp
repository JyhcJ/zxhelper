#include "pch.h"
#include "controller.h"
#include "Log.h"
#include "normalFun.h"
#include "T_obj.h"
#include "zxhelper.h"
extern "C" void call_relex();
std::vector<T包裹物品属性> OBJS_ATT;
QWORD TEMP_FIRSTOFFSET;
// 打坐
void relex() {
	call_relex();
}
// 使用回城卷
void goHome() {

	traversePackage();
	seekCityRoll();
}

void traversePackage() {
	T包裹物品属性 obj;
	T包裹偏移 objOffset;

	OBJS_ATT.clear();

	QWORD tfirst = BASE_ADDRESS_ATT;
	for (QWORD offset : objOffset.d一级偏移vector)
	{
		tfirst = SafeReadQWORD((QWORD*)(tfirst + offset));
	}

	if (objOffset.d一级偏移r == 0) {
		objOffset.d一级偏移r = tfirst;
	}

	QWORD tsecond;
	tsecond = SafeReadQWORD((QWORD*)(objOffset.d一级偏移r + objOffset.d二级偏移));

	QWORD tcount;
	tcount = SafeReadQWORD((QWORD*)(tsecond + objOffset.d包裹上限偏移));

	for (size_t i = 0; i < tcount; i++)
	{
		QWORD tobj;
		tobj = SafeReadQWORD((QWORD*)(tsecond + i * 0x8));
		if (tobj == 0)
		{
			continue;
		}
		obj.d物品ID = SafeReadQWORD((QWORD*)(tobj + 0xC));
		obj.d数量 = SafeReadQWORD((QWORD*)(tobj + 0x18));
		obj.d是否可使用 = SafeReadQWORD((QWORD*)(tobj + 0x20));

		for (std::vector<QWORD> packetOffsets : objOffset.d包裹物品名称偏移)
		{
			QWORD tname = tobj;
			for (QWORD offset : packetOffsets)
			{
				tname = SafeReadQWORD((QWORD*)(tname + offset));
			}
			if (isString((char*)tname, 25))
			{
				obj.d物品名称 = (wchar_t*)tname;
				break;
			}
		}

		OBJS_ATT.push_back(obj);
	}
}

void seekCityRoll() {
	const CString cityRollName = L"回城券";
}

wchar_t* getName() {
	QWORD value = *(QWORD*)BASE_ADDRESS_NAME;
	if (isString(reinterpret_cast<const char*>(value), 20)) {
		return  (wchar_t*)(value + 0);
	}
	else {
		const char* str = "error";
		return (wchar_t*)str;
	}
}

bool isString(const char* ptr, size_t maxLength = 20) {
	//CString str;
	//str.Format(_T("Pointer address: 0x%p"), BASE_ADDRESS_ATT); // %p 用于格式化指针
	//AfxMessageBox(str); // 弹出消息框显示指针地址

	SafeReadQWORD((QWORD*)ptr);

	if (ptr == nullptr || IsBadReadPtr(ptr, maxLength)) {
		AfxMessageBox(_T("Invalid pointer or memory range!"));
		return false;  // 空指针不是字符串
	}
	__try
	{
		for (size_t i = 0; i < maxLength; ++i) {
			if (ptr[i] == '\0') {
				return true;  // 遇到空字符，说明是字符串
			}
		}
		return false;
	}
	__except (GetExceptionCode() == EXCEPTION_ACCESS_VIOLATION ? EXCEPTION_EXECUTE_HANDLER : EXCEPTION_CONTINUE_SEARCH) {
		{
			AfxMessageBox(L"200");
			return false;  // 未找到空字符，可能不是字符串
		}
	}
}

QWORD getAtt_TBaseAddress() {
	__try {
		QWORD baseValue = SafeReadQWORD((QWORD*)(BASE_ADDRESS_ATT));
		baseValue = SafeReadQWORD((QWORD*)(baseValue + 0x38));
		baseValue = SafeReadQWORD((QWORD*)(baseValue + 0x10));
		baseValue = SafeReadQWORD((QWORD*)(baseValue + 0x48));
		baseValue = SafeReadQWORD((QWORD*)(baseValue + 0x10));
		baseValue = SafeReadQWORD((QWORD*)(baseValue + 0x60));
		TEMP_FIRSTOFFSET = baseValue;
		return baseValue;
	}
	__except (GetExceptionCode() == EXCEPTION_ACCESS_VIOLATION ? EXCEPTION_EXECUTE_HANDLER : EXCEPTION_CONTINUE_SEARCH) {
		AfxMessageBox(L"读取属性基地址异常");
		MessageBox(NULL, L"读取属性基地址异常", NULL, 0U);
		//exit(1);
		return -20; // 返回默认值或错误码
	}
}

QWORD getAtt_qixue() {
	QWORD ret = TEMP_FIRSTOFFSET;
	if (ret < 0)	return ret;
	return  SafeReadQWORD((QWORD*)(ret + OFFSET_QIXUE));
}

QWORD getAtt_zhenqi() {
	QWORD ret = TEMP_FIRSTOFFSET;
	if (ret < 0)
	{
		return ret;
	}
	return SafeReadQWORD((QWORD*)(ret + OFFSET_QIXUE + 0x8));
}

QWORD getAtt_yuanli() {
	QWORD ret = TEMP_FIRSTOFFSET;
	if (ret < 0)
	{
		return ret;
	}
	return SafeReadQWORD((QWORD*)(ret + OFFSET_QIXUE + 0xC));
}

QWORD getAtt_wushi() {
	QWORD ret = TEMP_FIRSTOFFSET;
	if (ret < 0)
	{
		return ret;
	}
	return SafeReadQWORD((QWORD*)(ret + OFFSET_QIXUE + 0x450C));
}

QWORD getAtt_jianmian() {
	QWORD ret = TEMP_FIRSTOFFSET;
	if (ret < 0)
	{
		return ret;
	}
	return SafeReadQWORD((QWORD*)(ret + OFFSET_QIXUE + 0x450C - 0x8));
}