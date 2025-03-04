#include "pch.h"
#include "controller.h"
#include "normalFun.h"
#include "Viewer.h"

extern "C" void call_relex();

//rdx ��ʱ�̶�Ϊ 0
extern "C" void call_useObj(QWORD rcx, QWORD rdx, QWORD r8, QWORD r9);

extern CLog g_Log;

std::vector<T������Ʒ����> OBJS_ATT;
T��������ƫ�� tPersonOffset;
T����ƫ�� objOffset;
QWORD TEMP_FIRSTOFFSET;
// ����
void relex() {
	call_relex();
}

// ʹ����Ʒ
void useObj(QWORD index, QWORD objId) {
	QWORD temp = 0;
	QWORD rcx;
	size_t i = USEOBJ_RCX_OFFSETS.size();
	for (QWORD offset : USEOBJ_RCX_OFFSETS)
	{
		i--;
		if (i != 0)
		{
			temp = SafeReadQWORD((QWORD*)(temp + offset));
		}
		else {
			rcx = temp + offset;
		}
	}
	call_useObj(rcx, 0x0, index, objId);
}
// ʹ�ûسǾ�
void goHome() {
	traversePackage();
	seekCityRoll();
}

std::vector<T������Ʒ����> traversePackage() {
	CString str;
	T������Ʒ���� obj;

	OBJS_ATT.clear();

	QWORD tfirst = SafeReadQWORD((QWORD*)(BASE_ADDRESS_ATT));

	for (QWORD offset : objOffset.dһ��ƫ��vector)
	{
		tfirst = SafeReadQWORD((QWORD*)(tfirst + offset));
	}

	if (objOffset.dһ��ƫ��r == 0) {
		objOffset.dһ��ƫ��r = tfirst;
	}

	QWORD tsecond;
	tsecond = SafeReadQWORD((QWORD*)(objOffset.dһ��ƫ��r + objOffset.d����ƫ��));

	DWORD tcount;
	tcount = SafeReadDWORD((DWORD*)(tsecond + objOffset.d��������ƫ��));

	if (tcount > 288)
	{
		return std::vector<T������Ʒ����>();
	}

	for (size_t i = 0; i < tcount; i++)
	{
		QWORD tobj;
		tobj = SafeReadQWORD((QWORD*)(tsecond + objOffset.d��������ƫ��));
		tobj = SafeReadQWORD((QWORD*)(tobj + i * 0x8));
		//CString str3;
		//str3.Format(_T("Pointer1 address: 0x%p"), tobj); // %p ���ڸ�ʽ��ָ��
		//AfxMessageBox(str3);
		if (tobj == 0)
		{
			continue;
		}
		obj.q��ƷID = SafeReadQWORD((QWORD*)(tobj + 0xC));
		obj.q���� = SafeReadQWORD((QWORD*)(tobj + 0x18));
		obj.d�Ƿ��ʹ�� = SafeReadQWORD((QWORD*)(tobj + 0x20));
		obj.q���ڸ��� = i;
		for (std::vector<QWORD> packetOffsets : objOffset.d������Ʒ����ƫ��)
		{
			QWORD tname = tobj;
			QWORD* tname_r = nullptr;
			size_t i = packetOffsets.size();
			for (QWORD offset : packetOffsets)
			{
				i--;
				//AfxMessageBox(obj.d��Ʒ����);

				if (i != 0)
				{
					tname = SafeReadQWORD((QWORD*)(tname + offset));
				}
				else {
					tname_r = (QWORD*)(tname + offset);
					//str.Format(_T("Pointer2 address: %X"), tname_r); // %p ���ڸ�ʽ��ָ��
					//g_Log.Write(str);
				}
			}

			if (isString(tname_r, 30)) {
				obj.p��Ʒ���� = (wchar_t*)tname_r;
				break;
			}
		}
		OBJS_ATT.push_back(obj);
	}
	return OBJS_ATT;
}

void seekCityRoll() {
	const CString cityRollName = L"�س�ȯ";
}

T��������  getPersonAtt() {
	T�������� tperson;
	QWORD userName = SafeReadQWORD((QWORD*)tPersonOffset.BASE_ADDRESS_NAME);
	if (isString((QWORD*)userName, 20)) {
		tperson.p���� = (wchar_t*)userName;
	}
	else {
		tperson.p���� = L"error";
	}

	QWORD tfirst = SafeReadQWORD((QWORD*)(BASE_ADDRESS_ATT));
	for (QWORD offset : tPersonOffset.dһ��ƫ��vector)
	{
		tfirst = SafeReadQWORD((QWORD*)(tfirst + offset));
	}

	if (tPersonOffset.dһ��ƫ��r == 0) {
		tPersonOffset.dһ��ƫ��r = tfirst;
	}

	tperson.dѪ�� = SafeReadDWORD((DWORD*)(tfirst + tPersonOffset.dѪ��ƫ��));
	tperson.d���� = SafeReadDWORD((DWORD*)(tfirst + tPersonOffset.d����ƫ��));
	tperson.dԪ�� = SafeReadDWORD((DWORD*)(tfirst + tPersonOffset.dԪ��ƫ��));
	tperson.d���� = SafeReadDWORD((DWORD*)(tfirst + tPersonOffset.d����ƫ��));
	tperson.d���� = SafeReadDWORD((DWORD*)(tfirst + tPersonOffset.d����ƫ��));

	return tperson;
}

bool isString(QWORD* ptr, size_t maxLength = 20) {
	//CString str;
	//str.Format(_T("Pointer address: 0x%p"), BASE_ADDRESS_ATT); // %p ���ڸ�ʽ��ָ��
	//AfxMessageBox(str); // ������Ϣ����ʾָ���ַ
	CString str;
	wchar_t* temp_ptr = (wchar_t*)(ptr);

	if (temp_ptr == nullptr) {
		//AfxMessageBox(_T("Invalid pointer or memory range!"));
		str.Format(_T("Pointer address: 0x%p is nullptr pointer !"), temp_ptr); // %p ���ڸ�ʽ��ָ��
		g_Log.Write(str);
		return false;  // ��ָ�벻���ַ���
	}

	wchar_t wchar;
	for (size_t i = 0; i < maxLength; ++i) {
		wchar = (wchar_t)SafeReadQWORD((QWORD*)(temp_ptr));
		if (i < 2)
		{
			if (wchar == 0x0001 || wchar == 0x0000)
			{
				return false;
			}
		}

		if (wchar == '\0' && i > 1) {
			return true;  // �������ַ���˵�����ַ���
		}
		temp_ptr++;
		//str.Format(_T("Pointer address: 0x%p!"), temp_ptr); // %p ���ڸ�ʽ��ָ��
		//g_Log.Write(str);
	}
	return false;
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