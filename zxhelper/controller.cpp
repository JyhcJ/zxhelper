#include "pch.h"
#include "controller.h"
#include "Log.h"
#include "normalFun.h"
#include "Viewer_packet.h"

extern "C" void call_relex(QWORD rcx, QWORD callAdd);

//rdx ��ʱ�̶�Ϊ 0
extern "C" void call_useObj(QWORD rcx, QWORD rdx, QWORD r8, QWORD r9, QWORD callAdd);

extern "C" void call_useObj_encap(QWORD rcx, byte* rdx, byte r8, byte r9, QWORD callAdd);

extern "C" void call_hanHua(QWORD rcx, QWORD rdx, LPCWSTR r8, QWORD callAddress);

extern "C" void call_Select(QWORD rcx, QWORD rdx, QWORD callAddress);

extern "C" void call_teleportation(QWORD rcx, QWORD rdx, QWORD r8, QWORD r9, QWORD obj_loc_ptr, QWORD rsp28, QWORD callAddress);

extern CLog g_Log;

std::vector<T������Ʒ����> OBJS_ATT;
T��������ƫ�� tPersonOffset;
T����ƫ�� objOffset;
T�������� tperson;
QWORD TEMP_FIRSTOFFSET;
// ����
void relex() {
	//send �µ�����
	//(48 8B CB C1 E8 05 F6 D0 24 01 0F B6 D0 ���Լ���С)+ 1�ֽ�(E8) ����4�ֽ�
	//����4�ֽ� + �ֵ�ַ +5 = call��ַ
	//D0 call ���ŵ�
	QWORD callAddress = 0x00000001403834C0;
	Call_���������Ϣ("������Ϣ:%p \t\t %p", BASE_ADDRESS_ATT, callAddress);
	call_relex(BASE_ADDRESS_ATT, callAddress);
}
// ʹ����Ʒ
void useObj(QWORD index, QWORD objId, bool isEncap) {
	QWORD rcx = 0;
	if (isEncap)
	{
		//send �� ���ĸ�
		//66 89 03 44 88 73 ?? 66 89 6B ?? 89 73 ?? 44 8D 40 ?? 40 88 7B ?? 48 8B 0D ?? ?? ?? ?? 48 8B 49 ??
		//call ??���ŵ�
		for (QWORD offset : USEOBJ_ENCAP_RCX_OFFSETS)
		{
			rcx = SafeReadQWORD((QWORD*)(rcx + offset));
		}

		byte bate_array[] = { 0x28,0x00,0x00,0x01,(byte)index,0x00,(byte)objId,(byte)(objId >> 8),(byte)(objId >> 16),0x00 };
		//Call_���������Ϣ()
		CString str;
		for (byte byte1 : bate_array) {
			str.AppendFormat(_T("%02X "), byte1);
		}

		Call_���������Ϣ("������Ϣ:ʹ����Ʒ���:%S", str);
		Call_���������Ϣ("������Ϣ:ʹ����Ʒ��� rcx = %p", rcx);
		//R8:A �Ƿ������

		//����������ʽ ���Ѳ���call_useObj_encap(rcx, bate_array, 0xA, 0x1);//r8=000000000000010A r9=000000001062AB01
		QWORD r8 = 0x000000000000000A;
		QWORD r9 = 0x1;
		QWORD callAddress = 0x00000001409DDA40;
		call_useObj_encap(rcx, bate_array, r8, r9, callAddress);
	}
	else {
		//send�µ����
		//88 44 24 20 45 8B C4 33 D2
		//call����D2

		/*i = USEOBJ_RCX_OFFSETS.size();
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
		}*/

		rcx = SafeReadQWORD_Arry_NotLast(USEOBJ_RCX_OFFSETS);
		QWORD callAddress = 0x00000001409AB5F0;
		Call_���������Ϣ("������Ϣ:%p	%p	%d	%p	%p", rcx, 0x0, index, objId, callAddress);
		call_useObj(rcx, 0x0, index, objId, callAddress);
	}
}
// ѡ�ж���
void selectObj(DWORD id) {
	//send�µ�����
	//48 8B 15 ?? ?? ?? ?? 48 8B 4A ?? 8B D0 48 81 C1 70 01 00 00 E8 ?? ?? ?? ?? 45 33 C0 8B D7
	//E8 �������call��ַ
	QWORD callAddress = 0x1409AB4F0;

	Call_���������Ϣ("����1:%p,����2:%p,����3:%p", BASE_ADDRESS_ATT, id, callAddress);
	call_Select(BASE_ADDRESS_ATT, id, callAddress);
}

// ��˪����Ϯ�ŵ�˲��
void teleportation(DWORD obj_location_ptr) {
	//send��
	//45 33 C9 45 0F B6 C4 8B 52 ? ? 48 81 C1 70 01 00 00
	//��β00 ���ž���call

	QWORD callAddress = 0x1409AB1B0;
	QWORD rdx = 0x1ECF;  //��˪ ����Ϯ
	QWORD r8 = 0x0;
	QWORD r9 = 0x0;
	QWORD rsp20 = obj_location_ptr;
	QWORD rsp28 = 0x0;
	call_teleportation(BASE_ADDRESS_ATT, rdx, r8, r9, rsp20, rsp28, callAddress);
}

// ʹ�ûسǾ�
void goHome() {
	traversePackage();
	seekCityRoll();
}
// ����
void hanHua(CString str) {
	//Call_���������Ϣ("������Ϣ:!!!");
	// send �µ��ĸ�
	//8B 45 9C 89 44 24 20 44 8B CB
	//call CB����
	if (isString((QWORD*)str.GetString(), 60))
	{
		//Call_���������Ϣ("������Ϣ:!!!%p", str);
		QWORD rcx = SafeReadQWORD((QWORD*)(BASE_ADDRESS_ATT));
		rcx = SafeReadQWORD((QWORD*)(rcx + 0x40));
		QWORD callAddress = 0x00000001409DD520;
		call_hanHua(rcx, NULL, str, callAddress);
	}
}
// ��������
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
		obj.d�Ƿ��ʹ�� = (char)SafeReadQWORD((QWORD*)(tobj + 0x20));
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
// ������Χ�������
std::vector<T��������> traversePerson() {
	//4C 8D 45 B7 48 8D 55 D7 48 8D 4D FF

	QWORD startAdd = SafeReadQWORD((QWORD*)(SafeReadQWORD_Arry_NotLast(PERSON_AROUD_OFFSETS) + 0x10));
	Call_���������Ϣ("������Ϣ:startAdd = %p", startAdd);
	QWORD size = SafeReadQWORD((QWORD*)(SafeReadQWORD_Arry_NotLast(PERSON_AROUD_OFFSETS) + 0x28));
	Call_���������Ϣ("������Ϣ:size = %d", size);
	QWORD objCount = 0;
	QWORD obj = 0;
	T�������� obj_att;
	std::vector<T��������> obj_att_arry;
	for (size_t i = 0; i < size; i++)
	{
		obj = SafeReadQWORD((QWORD*)(startAdd + i * 8));
		if (obj == 0)
		{
			continue;
		}
		else {
			Call_���������Ϣ("������Ϣ:obj = %p", obj);
			obj_att.dID = SafeReadDWORD((DWORD*)(obj + obj_att.offset.dIDƫ��));
			selectObj(obj_att.dID);
			DWORD temp = SafeReadDWORD((DWORD*)(obj + 8));
			obj_att.d�ȼ� = SafeReadDWORD((DWORD*)(temp + obj_att.offset.d�ȼ�ƫ��));
			obj_att.dѪ�� = SafeReadDWORD((DWORD*)(temp + obj_att.offset.dѪ��ƫ��));
			obj_att.p����ptr = (DWORD*)(temp + obj_att.offset.fXƫ��);
			obj_att.f���� = calculateDistance(tperson.p����ptr, obj_att.p����ptr);
			obj_att_arry.push_back(obj_att);
			objCount++;
		}
	}
	Call_���������Ϣ("������Ϣ:objCount = %d", objCount);
	return obj_att_arry;
}
// ��������
std::vector<T����> traverseSkill() {
	std::vector<T����> TSkills;
	T���� TSkill;
	QWORD address_һ�� = 0;
	for (QWORD offset : TSkill.skillOffsets.offsets_һ��) {
		address_һ�� = SafeReadQWORD((QWORD*)(address_һ�� + offset));
		//Call_���������Ϣ("1:%p ", address_һ��);
		//Call_���������Ϣ("2:%p ", offset);

	}
	size_t skillMax = SafeReadDWORD((DWORD*)(address_һ�� + TSkill.skillOffsets.offset_���ܱ����������));
	//Call_���������Ϣ("һ����ַ:%p ", address_һ��);
	Call_���������Ϣ("skillMax:%d ", skillMax);
	QWORD address_�׵�ַ = SafeReadQWORD((QWORD*)(address_һ�� + TSkill.skillOffsets.offset_���������׵�ַ));
	//for (size_t i = 0; i < skillMax; i++)
	for (size_t i = 0; i < skillMax; i++)
	{
		if (EXITFLAG) {
			break;
		}
		//Call_���������Ϣ("i: %llu, skillMax: %llu", i, skillMax);

		DWORD address_���ܶ��� = SafeReadDWORD((DWORD*)(address_�׵�ַ + 8 * i));

		TSkill.d����ID = SafeReadDWORD((DWORD*)(address_���ܶ��� + TSkill.skillOffsets.offset_����ID));
		TSkill.d������ȴʱ�� = SafeReadDWORD((DWORD*)(address_���ܶ��� + TSkill.skillOffsets.offset_������ȴʱ��));
		TSkill.d���������ȴʱ�� = SafeReadDWORD((DWORD*)(address_���ܶ��� + TSkill.skillOffsets.offset_���������ȴʱ��));
		TSkill.d���ܶ��� = address_���ܶ���;


		Call_���������Ϣ("address_���ܶ���:%p ", address_���ܶ���);
		Call_���������Ϣ("qq����ID:%d ", TSkill.d����ID);
		//Call_���������Ϣ("q������ȴʱ��:%d ", TSkill.q������ȴʱ��);
		//Call_���������Ϣ("qq���������ȴʱ��:%d ", TSkill.q���������ȴʱ��);

		if (Viewer_packet::cSKILL_NAME_ARRY.empty())
		{
			break;
		}


		for (const CString& index : Viewer_packet::cSKILL_NAME_ARRY)
		{
			//Call_���������ϢW(_T("index:%s"), index.GetString());
			CString strSkillID;

			size_t temp10 = TSkill.d����ID;
			temp10 = temp10 * 10;

			strSkillID.Format(_T("%llu"), temp10); // �� QWORD ת��Ϊ�����ַ���
			//Call_���������Ϣ("strSkillID:%S", strSkillID.GetString());
			if (index.Find(strSkillID) == 0)
			{
				// ���ҵ�һ��˫���ŵ�λ��
				int start = index.Find(_T('"'));
				if (start == -1)
				{
					Call_���������Ϣ("����δ�ҵ���ʼ˫����");
					continue; // ������ǰѭ��
				}

				// ���ҵڶ���˫���ŵ�λ��
				int end = index.Find(_T('"'), start + 1);
				if (end == -1)
				{
					Call_���������Ϣ("����δ�ҵ�����˫����");
					continue; // ������ǰѭ��
				}
				// ��ȡ˫����֮�������
				TSkill.p�������� = index.Mid(start + 1, end - start - 1);

				Call_���������ϢW(_T("�ҵ��ļ�������:%s "), TSkill.p��������);

				break;
			}
			
		}
		TSkills.push_back(TSkill);



		/*QWORD ��������temp = address_���ܶ���;
		for (QWORD offset : TSkill.skillOffsets.offsets_��������)
		{
			��������temp = SafeReadQWORD((QWORD*)(��������temp + offset));
		}
		Call_���������Ϣ("qq��������temp:%p ", ��������temp);*/

		//wchar_t constrcName[15];
		//try
		//{
		//	
		//	wchar_t* wptr = (wchar_t*)��������temp;
		//	wptr++; //���Ե�һ��f
		//	wchar_t terminator = 0x3000;


		//	for (size_t i = 0; i < 15; i++)
		//	{
		//		if ((wchar_t)wptr++ == terminator)
		//		{
		//			constrcName[i] = (DWORD)0x0000;
		//			break;
		//		}
		//		constrcName[i] = (wchar_t)wptr;
		//	}
		//}
		//catch (const std::exception&)
		//{
		//	Call_���������Ϣ("���������ַ���ת��ʧ��");
		//}




		//Call_���������Ϣ("constrcName:%p ", constrcName);
		/*if (isString((QWORD*)constrcName, 15))
		{
			TSkill.p�������� = constrcName;
		}*/
	}

	return	TSkills;
}


void seekCityRoll() {
	const CString cityRollName = L"�س�ȯ";
}
// ��ȡ������������
T��������  getPersonAtt() {
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
		tperson.p����ptr = (DWORD*)(tfirst + tPersonOffset.fXƫ��);
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