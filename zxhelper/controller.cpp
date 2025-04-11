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

extern "C" void call_follow(QWORD rcx, QWORD rdx, QWORD r8,QWORD r9, QWORD objID, QWORD callAddress);

extern CLog g_Log;

std::vector<T������Ʒ����> OBJS_ATT;
T��������ƫ�� tPersonOffset;
T����ƫ�� objOffset;
T�������� tperson;
QWORD TEMP_FIRSTOFFSET;
// ����

static QWORD callAddress_relex = 0x140387880;
void relex() {
	//send �µ�����
	//(48 8B CB C1 E8 05 F6 D0 24 01 0F B6 D0 ���Լ���С)+ 1�ֽ�(E8) ����4�ֽ�
	//����4�ֽ� + �ֵ�ַ +5 = call��ַ
	//D0 call ���ŵ�
	//QWORD callAddress = 0x00000001403834C0;
	//QWORD callAddress = 0x00000001403884C0;
	
	//Call_���������Ϣ("������Ϣ:%p \t\t %p", BASE_ADDRESS_ATT, callAddress);

	QWORD args[] = { GetBaseAddress(), callAddress_relex };

	call_asm([=]()->void
		{
			call_relex(GetBaseAddress(), callAddress_relex);

		});

}
// ʹ����Ʒ
static QWORD callAddress_useObj = 0x00000001409AFD10;
static QWORD callAddress_EncapuseObj = 0x00000001409E20B0;
void useObj(QWORD index, QWORD objId, bool isEncap) {
	QWORD rcx = 0;
	if (isEncap)
	{
		//send �� ���ĸ�
		//66 89 03 44 88 73 ?? 66 89 6B ?? 89 73 ?? 44 8D 40 ?? 40 88 7B ?? 48 8B 0D ?? ?? ?? ?? 48 8B 49 ??
		//call ??���ŵ�
		for (QWORD offset : Get_USEOBJ_ENCAP_RCX_OFFSETS())
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
		//QWORD callAddress = 0x00000001409DDA40;
		//QWORD callAddress = 0x00000001409E20B0;

		/*call_asm([=]()->void
			{*/
		call_useObj_encap(rcx, const_cast<byte*>(bate_array), static_cast<byte>(r8), static_cast<byte>(r9), callAddress_EncapuseObj);
		//});


	}
	else {
		//send�µ����
		//88 44 24 20 45 8B C4 33 D2
		//call����D2
		rcx = SafeReadQWORD_Arry_NotLast(Get_USEOBJ_RCX_OFFSETS());
		//QWORD callAddress = 0x00000001409AB5F0;
		//QWORD callAddress = 0x00000001409AFD10;
		Call_���������Ϣ("������Ϣ:%p	%p	%d	%p	%p", rcx, 0x0, index, objId, callAddress_useObj);

		QWORD rdx = 0x0;

		QWORD params[] = { rcx, rdx, index, objId, callAddress_useObj };
		/*call_asm([=]()->void
			{*/
		call_useObj(rcx, 0x0, index, objId, callAddress_useObj);
		//});
	}
}
// ѡ�ж���
static QWORD callAddress_selectObj = 0x00000001409AFC10;
void selectObj(DWORD id) {
	//send�µ�����
	//48 8B 15 ?? ?? ?? ?? 48 8B 4A ?? 8B D0 48 81 C1 70 01 00 00 E8 ?? ?? ?? ?? 45 33 C0 8B D7    E8 �������call��ַ
	//3B C3 0F 84 9A 00 00 00 89 86 ?? ?? ?? ?? 48 8B 15 ?? ?? ?? ?? 48 8B 4A ?? 8B D0         D0��������и�ADD�������

	//QWORD callAddress = 0x1409AB4F0;
	//QWORD callAddress = 0x00000001409AFC10;

	Call_���������Ϣ("��ַ:%p,ѡ������ID:%p,call��ַ:%p", GetBaseAddress(), id, callAddress_selectObj);
	//call_Select(BASE_ADDRESS_ATT, id, callAddress);
	call_asm([=]()->void
		{
			call_Select(GetBaseAddress(), id, callAddress_selectObj);
		});

}

// ��˪����Ϯ�ŵ�˲��
static QWORD callAddress_teleportation = 0x00000001409AF8D0;
void teleportation(DWORD obj_location_ptr) {
	//send��
	//45 33 C9 45 0F B6 C4 8B 52 ? ? 48 81 C1 70 01 00 00
	//��β00 ���ž���call

	//QWORD callAddress = 0x1409AB1B0;
	//QWORD callAddress = 0x00000001409AF8D0;
	QWORD rdx = 0x1ECF;  //��˪ ����Ϯ
	QWORD r8 = 0x0;
	QWORD r9 = 0x0;
	QWORD rsp20 = obj_location_ptr;
	QWORD rsp28 = 0x0;
	//call_teleportation(BASE_ADDRESS_ATT, rdx, r8, r9, rsp20, rsp28, callAddress);
	call_asm([=]()->void
		{
			call_teleportation(GetBaseAddress(), rdx, r8, r9, rsp20, rsp28, callAddress_teleportation);
		});


}

// һ������Ŀ��(��һ�㺯��������call ����ʵ�ֽ�������в���)
void follow(QWORD objID) {

	//48 8B CF 48 63 D8 E8 ?? ?? ?? ?? 48 8B C8 41 B0 01 8B D3 E8 ?? ?? ?? ?? 48 8B CF 

	char* featureCode = "48 8B CF 48 63 D8 E8 ?? ?? ?? ?? 48 8B C8 41 B0 01 8B D3 E8 ?? ?? ?? ?? 48 8B CF";
	char* cruxCode = "E8";

	QWORD rdx = 0xEE;
	QWORD r8 = 0x0;
	QWORD r9 = 0x0;
	//Call_���������Ϣ("0x%p;;;call_����rcx", calAddress(featureCode, cruxCode, true, 0, false));
	//Call_���������Ϣ("0x%p;;;call_����call", calAddress(featureCode, cruxCode, true, 1, false));
	call_follow(
		calAddress(featureCode, cruxCode, true, 0, false),
		rdx,
		r8,
		r9,
		objID,
		calAddress(featureCode, cruxCode, true, 1, false)
	);
}

// ����
static QWORD callAddress_hanHua = 0x00000001409E1B90;
void hanHua(CString str) {
	//Call_���������Ϣ("������Ϣ:!!!");
	// send �µ��ĸ�
	//8B 45 9C 89 44 24 20 44 8B CB
	//call CB����
	if (isString((QWORD*)str.GetString(), 60))
	{
		//Call_���������Ϣ("������Ϣ:!!!%p", str);
		QWORD rcx = SafeReadQWORD((QWORD*)(GetBaseAddress()));
		rcx = SafeReadQWORD((QWORD*)(rcx + 0x40));
		//QWORD callAddress = 0x00000001409DD520;
		//QWORD callAddress = 0x00000001409E1B90;
		call_asm([=]()->void
			{
				call_hanHua(rcx, NULL, str, callAddress_hanHua);
			});


	}
}
// ��������
std::vector<T������Ʒ����> traversePackage() {
	CString str;
	T������Ʒ���� obj;

	OBJS_ATT.clear();

	QWORD tfirst = SafeReadQWORD((QWORD*)(GetBaseAddress()));
	if (tfirst > 0XFFFFFFFFFFFF) {
		obj.b��Ч�� = false;
		Call_���������Ϣ("��ȡ�����׵�ַ����tfirst : %p  ...", tfirst);
		return std::vector<T������Ʒ����>();
	}


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

	QWORD startAdd = SafeReadQWORD((QWORD*)(SafeReadQWORD_Arry_NotLast(Get_PERSON_AROUD_OFFSETS()) + 0x10));
	Call_���������Ϣ("��Χ�������startAdd = %p", startAdd);
	if (startAdd > 0XFFFFFFFFFFFF) {
		Call_���������Ϣ("��ȡ��Χ����������startAdd : %p  ...", startAdd);
		return std::vector<T��������>();
	}

	QWORD size = SafeReadQWORD((QWORD*)(SafeReadQWORD_Arry_NotLast(Get_PERSON_AROUD_OFFSETS()) + 0x28));
	Call_���������Ϣ("��Χ�������size = %d", size);
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
			Call_���������Ϣ("������Ϣ:ѡ�ж����ַ = %p", obj);
			obj_att.dID = SafeReadDWORD((DWORD*)(obj + obj_att.offset.dIDƫ��));
			selectObj(obj_att.dID);
			DWORD temp = SafeReadDWORD((DWORD*)(obj + 8));
			obj_att.d�ȼ� = SafeReadDWORD((DWORD*)(temp + obj_att.offset.d�ȼ�ƫ��));
			obj_att.dѪ�� = SafeReadDWORD((DWORD*)(temp + obj_att.offset.dѪ��ƫ��));
			obj_att.p����ptr = (DWORD*)((QWORD)temp + obj_att.offset.fXƫ��);
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
	for (QWORD offset : TSkill.skillOffsets.Get_offsets_һ��()) {
		address_һ�� = SafeReadQWORD((QWORD*)(address_һ�� + offset));
		if (address_һ�� > 0XFFFFFFFFFFFF) {
			return std::vector<T����>();
		}
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
		if (g_exitFLag) {
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
// ��ȡ������������
T��������  getPersonAtt() {
	QWORD userName = SafeReadQWORD((QWORD*)tPersonOffset.BASE_ADDRESS_NAME);


	if (isString((QWORD*)userName, 20)) {
		tperson.p���� = (wchar_t*)userName;
	}
	else {
		if (isString((QWORD*)tPersonOffset.BASE_ADDRESS_NAME, 20)) {
			tperson.p���� = (wchar_t*)tPersonOffset.BASE_ADDRESS_NAME;
		}
		else {
			tperson.p���� = L"error";
		}
	}

	QWORD tfirst = SafeReadQWORD((QWORD*)(GetBaseAddress()));
	if (tfirst > 0XFFFFFFFFFFFF) {
		tperson.��Ч�� = false;
		return T��������();
	}

	for (QWORD offset : tPersonOffset.dһ��ƫ��vector)
	{
		tfirst = SafeReadQWORD((QWORD*)(tfirst + offset));

	}


	//ʵʱ����,��Ҫ���ػ��������
	//if (tPersonOffset.dһ��ƫ��r == 0) {
	//	tPersonOffset.dһ��ƫ��r = tfirst;
	//	tperson.p����ptr = (DWORD*)(tfirst + tPersonOffset.fXƫ��);
	//}

	tperson.p����ptr = (DWORD*)(tfirst + tPersonOffset.fXƫ��);
	tperson.dѪ�� = SafeReadDWORD((DWORD*)(tfirst + tPersonOffset.dѪ��ƫ��));
	tperson.d���� = SafeReadDWORD((DWORD*)(tfirst + tPersonOffset.d����ƫ��));
	tperson.dԪ�� = SafeReadDWORD((DWORD*)(tfirst + tPersonOffset.dԪ��ƫ��));
	tperson.d���� = SafeReadDWORD((DWORD*)(tfirst + tPersonOffset.d����ƫ��));
	tperson.d���� = SafeReadDWORD((DWORD*)(tfirst + tPersonOffset.d����ƫ��));
	tperson.��Ч�� = true;

	return tperson;
}
// ���»�ַ
void updateBaseAddress() {
	//featureCode = 66 89 03 44 88 73 ? ? 66 89 6B ? ? 89 73 ? ? 44 8D 40 ? ? 40 88 7B ? ? 48 8B 0D ? ? ? ? ? ? ? ? 48 8B 49 ? ?
	//cruxCode = 48 8B 0D;
	//isAfter = true
	//repeats = 0;
	char* featureCode = "66 89 03 44 88 73 ??";
	char* cruxCode = "48 8B 0D";
	QWORD newAddress = calAddress(featureCode, cruxCode, true, 0, false);
	if (newAddress)
	{
		if (GetBaseAddress() != newAddress)

		{
			GetBaseAddress() = newAddress;
			Call_���������Ϣ("0x%p;;;��ַ_�µĻ�ַ", newAddress);
		}
	}



	//g_Log.Write(_T("��ַ:0x%p"), newAddress);
}
// ����call��ַ
void updateCallAddress() {
	QWORD newAddress;
	char* featureCode;
	char* cruxCode;


	featureCode = "48 8B CB C1 E8 05 F6 D0 24 01 0F B6 D0";
	cruxCode = "E8";
	newAddress = calAddress(featureCode, cruxCode, true, 0, false);
	if (newAddress)
	{
		if (callAddress_relex != newAddress) {
			callAddress_relex = newAddress;
			//g_Log.Write(("CALL����:0x%p"), newAddress);
			Call_���������Ϣ("0x%p;;;call_����", newAddress);

		}
	}


	featureCode = "88 44 24 20 45 8B C4 33 D2";
	cruxCode = "E8";
	newAddress = calAddress(featureCode, cruxCode, true, 0, false);
	if (newAddress)
	{
		if (callAddress_useObj != newAddress)
		{
			callAddress_useObj = newAddress;
			//g_Log.Write(L"CALLʹ����Ʒ:0x%p", newAddress);
			Call_���������Ϣ("0x%p;;;call_ʹ����Ʒ", newAddress);
		}
	}



	featureCode = "66 89 03 44 88 73 ?? 66 89 6B ?? 89 73 ?? 44 8D 40 ?? 40 88 7B ?? 48 8B 0D ?? ?? ?? ?? 48 8B 49 ??";
	cruxCode = "E8";
	newAddress = calAddress(featureCode, cruxCode, true, 0, false);
	if (newAddress)
	{
		if (callAddress_EncapuseObj != newAddress) {
			callAddress_EncapuseObj = newAddress;
			//g_Log.Write(_T("���ʹ����Ʒ:0x%p"), newAddress);
			Call_���������Ϣ("0x%p;;;call_���ʹ����Ʒ", newAddress);

		}
	}


	featureCode = "3B C3 0F 84 9A 00 00 00 89 86 ?? ?? ?? ?? 48 8B 15 ?? ?? ?? ?? 48 8B 4A ?? 8B D0";
	cruxCode = "E8";
	newAddress = calAddress(featureCode, cruxCode, true, 0, false);
	if (newAddress)
	{
		if (callAddress_selectObj != newAddress) {
			callAddress_selectObj = newAddress;
			//g_Log.Write(_T("���ʹ����Ʒ:0x%p"), newAddress);
			Call_���������Ϣ("0x%p;;;call_ѡ�ж���", newAddress);

		}
	}
	

	featureCode = "45 33 C9 45 0F B6 C4 8B 52 ?? 48 81 C1 70 01 00 00";
	cruxCode = "E8";
	newAddress = calAddress(featureCode, cruxCode, true, 0, false);
	if (newAddress)
	{
		if (callAddress_teleportation != newAddress) {
			callAddress_teleportation = newAddress;
			//g_Log.Write(_T("���ʹ����Ʒ:0x%p"), newAddress);
			Call_���������Ϣ("0x%p;;;call_����Ϯ", newAddress);
		}
	}


	featureCode = "8B 45 9C 89 44 24 20 44 8B CB";
	cruxCode = "E8";
	newAddress = calAddress(featureCode, cruxCode, true, 0 , false);
	if (newAddress)
	{
		if (callAddress_hanHua != newAddress) {
			callAddress_hanHua = newAddress;
			//g_Log.Write(_T("���ʹ����Ʒ:0x%p"), newAddress);
			Call_���������Ϣ("0x%p;;;call_����", newAddress);
		}
	}



	featureCode = "CC 4C 8B C1 81 FA E8 03 00 00";
	cruxCode = "49 8B 80";
	newAddress = calAddress(featureCode, cruxCode, true, 0,true);
	if (newAddress)
	{
		if (T����ƫ��::d����ƫ�� != newAddress) {
			T����ƫ��::d����ƫ�� = newAddress;
			//g_Log.Write(_T("���ʹ����Ʒ:0x%p"), newAddress);
			Call_���������Ϣ("0x%p;;;ƫ��_������ƫ��", newAddress);
		}
	}

	

	featureCode = "C3 CC CC 48 89 5C 24 08 44 8B 99 ?? ?? ?? ?? 33 C0 44 8B D0 45 85 DB 7E";
	cruxCode = "24 48 8B 99";
	newAddress = calAddress(featureCode, cruxCode, true, 0, true);
	if (newAddress)
	{
		if (T����ƫ��::offset_���������׵�ַ != newAddress) {
			T����ƫ��::offset_���������׵�ַ = newAddress;
			T����ƫ��::offset_���ܱ���������� = newAddress + 0x8;
			//g_Log.Write(_T("���ʹ����Ʒ:0x%p"), newAddress);
			Call_���������Ϣ("0x%p;;;ƫ��_�����׵�ַ��ƫ��", newAddress);
			Call_���������Ϣ("0x%p;;;ƫ��_���ܱ����������", T����ƫ��::offset_���ܱ����������);
		}
	}

	
}




void updateAddress() {
	updateBaseAddress();
	updateCallAddress();
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
	//__try {
	QWORD baseValue = SafeReadQWORD((QWORD*)(GetBaseAddress()));
	baseValue = SafeReadQWORD((QWORD*)(baseValue + 0x38));
	baseValue = SafeReadQWORD((QWORD*)(baseValue + 0x10));
	baseValue = SafeReadQWORD((QWORD*)(baseValue + 0x48));
	baseValue = SafeReadQWORD((QWORD*)(baseValue + 0x10));
	baseValue = SafeReadQWORD((QWORD*)(baseValue + 0x60));
	TEMP_FIRSTOFFSET = baseValue;
	return baseValue;
	//}
	//__except (GetExceptionCode() == EXCEPTION_ACCESS_VIOLATION ? EXCEPTION_EXECUTE_HANDLER : EXCEPTION_CONTINUE_SEARCH) {
	//	AfxMessageBox(L"��ȡ���Ի���ַ�쳣");
	//	MessageBox(NULL, L"��ȡ���Ի���ַ�쳣", NULL, 0U);
	//	//exit(1);
	//	return -20; // ����Ĭ��ֵ�������
	//}
}