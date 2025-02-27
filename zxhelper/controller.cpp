#include "pch.h"
#include "controller.h"

extern "C" void call_relex();

//���Ի�ַ
const QWORD BASE_ADDRESS_ATT = 0x14171CFD8;
//const QWORD BASE_ADDRESS_NAME = 0x7FFC440F4978;
const QWORD BASE_ADDRESS_NAME = 0x1417243B0;
const int  OFFSET_QIXUE = 0x558;

void relex() {
	call_relex();
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

QWORD getAtt_TBaseAddress() {
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
		MessageBox(NULL, L"��ȡ���Ի���ַ�쳣", NULL, 0U);
		exit(1);
	}


}

QWORD getAtt_qixue() {
	return  *(QWORD*)(getAtt_TBaseAddress() + OFFSET_QIXUE);
}

QWORD getAtt_zhenqi() {
	return *(QWORD*)(getAtt_TBaseAddress() + OFFSET_QIXUE + 0x8);
}

QWORD getAtt_yuanli() {
	return *(QWORD*)(getAtt_TBaseAddress() + OFFSET_QIXUE + 0xC);
}

QWORD getAtt_wushi() {
	return *(QWORD*)(getAtt_TBaseAddress() + OFFSET_QIXUE + 0x450C);
}

QWORD getAtt_jianmian() {
	return *(QWORD*)(getAtt_TBaseAddress() + OFFSET_QIXUE + 0x450C - 0x8);
}

bool isString(const char* ptr, size_t maxLength = 20) {
	//CString str;
	//str.Format(_T("Pointer address: 0x%p"), BASE_ADDRESS_ATT); // %p ���ڸ�ʽ��ָ��
	//AfxMessageBox(str); // ������Ϣ����ʾָ���ַ

	if (ptr == nullptr) {

		return false;  // ��ָ�벻���ַ���
	}
	try
	{
		for (size_t i = 0; i < maxLength; ++i) {


			if (ptr[i] == '\0') {
				return true;  // �������ַ���˵�����ַ���
			}
		}
	}
	catch (...)
	{
		AfxMessageBox(L"200");
	}
	



	return false;  // δ�ҵ����ַ������ܲ����ַ���
}

