#include "pch.h"
#include "controller.h"
#include "Log.h"
#include "normalFun.h"
#include "T_obj.h"
#include "zxhelper.h"
extern "C" void call_relex();
std::vector<T������Ʒ����> OBJS_ATT;
QWORD TEMP_FIRSTOFFSET;
// ����
void relex() {
	call_relex();
}
// ʹ�ûسǾ�
void goHome() {

	traversePackage();
	seekCityRoll();
}

void traversePackage() {
	T������Ʒ���� obj;
	T����ƫ�� objOffset;

	OBJS_ATT.clear();

	QWORD tfirst = BASE_ADDRESS_ATT;
	for (QWORD offset : objOffset.dһ��ƫ��vector)
	{
		tfirst = SafeReadQWORD((QWORD*)(tfirst + offset));
	}

	if (objOffset.dһ��ƫ��r == 0) {
		objOffset.dһ��ƫ��r = tfirst;
	}

	QWORD tsecond;
	tsecond = SafeReadQWORD((QWORD*)(objOffset.dһ��ƫ��r + objOffset.d����ƫ��));

	QWORD tcount;
	tcount = SafeReadQWORD((QWORD*)(tsecond + objOffset.d��������ƫ��));

	for (size_t i = 0; i < tcount; i++)
	{
		QWORD tobj;
		tobj = SafeReadQWORD((QWORD*)(tsecond + i * 0x8));
		if (tobj == 0)
		{
			continue;
		}
		obj.d��ƷID = SafeReadQWORD((QWORD*)(tobj + 0xC));
		obj.d���� = SafeReadQWORD((QWORD*)(tobj + 0x18));
		obj.d�Ƿ��ʹ�� = SafeReadQWORD((QWORD*)(tobj + 0x20));

		for (std::vector<QWORD> packetOffsets : objOffset.d������Ʒ����ƫ��)
		{
			QWORD tname = tobj;
			for (QWORD offset : packetOffsets)
			{
				tname = SafeReadQWORD((QWORD*)(tname + offset));
			}
			if (isString((char*)tname, 25))
			{
				obj.d��Ʒ���� = (wchar_t*)tname;
				break;
			}
		}

		OBJS_ATT.push_back(obj);
	}
}

void seekCityRoll() {
	const CString cityRollName = L"�س�ȯ";
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
	//str.Format(_T("Pointer address: 0x%p"), BASE_ADDRESS_ATT); // %p ���ڸ�ʽ��ָ��
	//AfxMessageBox(str); // ������Ϣ����ʾָ���ַ

	SafeReadQWORD((QWORD*)ptr);

	if (ptr == nullptr || IsBadReadPtr(ptr, maxLength)) {
		AfxMessageBox(_T("Invalid pointer or memory range!"));
		return false;  // ��ָ�벻���ַ���
	}
	__try
	{
		for (size_t i = 0; i < maxLength; ++i) {
			if (ptr[i] == '\0') {
				return true;  // �������ַ���˵�����ַ���
			}
		}
		return false;
	}
	__except (GetExceptionCode() == EXCEPTION_ACCESS_VIOLATION ? EXCEPTION_EXECUTE_HANDLER : EXCEPTION_CONTINUE_SEARCH) {
		{
			AfxMessageBox(L"200");
			return false;  // δ�ҵ����ַ������ܲ����ַ���
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
		AfxMessageBox(L"��ȡ���Ի���ַ�쳣");
		MessageBox(NULL, L"��ȡ���Ի���ַ�쳣", NULL, 0U);
		//exit(1);
		return -20; // ����Ĭ��ֵ�������
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