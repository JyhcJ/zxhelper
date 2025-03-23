#include "pch.h"
#include "controller.h"
#include "Log.h"
#include "normalFun.h"
#include "Viewer_packet.h"

extern "C" void call_relex(QWORD rcx, QWORD callAdd);

//rdx 暂时固定为 0
extern "C" void call_useObj(QWORD rcx, QWORD rdx, QWORD r8, QWORD r9, QWORD callAdd);

extern "C" void call_useObj_encap(QWORD rcx, byte* rdx, byte r8, byte r9, QWORD callAdd);

extern "C" void call_hanHua(QWORD rcx, QWORD rdx, LPCWSTR r8, QWORD callAddress);

extern "C" void call_Select(QWORD rcx, QWORD rdx, QWORD callAddress);

extern "C" void call_teleportation(QWORD rcx, QWORD rdx, QWORD r8, QWORD r9, QWORD obj_loc_ptr, QWORD rsp28, QWORD callAddress);

extern CLog g_Log;

std::vector<T包裹物品属性> OBJS_ATT;
T人物属性偏移 tPersonOffset;
T包裹偏移 objOffset;
T人物属性 tperson;
QWORD TEMP_FIRSTOFFSET;
// 打坐
void relex() {
	//send 下第六个
	//(48 8B CB C1 E8 05 F6 D0 24 01 0F B6 D0 加自己大小)+ 1字节(E8) 读这4字节
	//读的4字节 + 现地址 +5 = call地址
	//D0 call 挨着的
	QWORD callAddress = 0x00000001403834C0;
	Call_输出调试信息("调试信息:%p \t\t %p", BASE_ADDRESS_ATT, callAddress);
	call_relex(BASE_ADDRESS_ATT, callAddress);
}
// 使用物品
void useObj(QWORD index, QWORD objId, bool isEncap) {
	QWORD rcx = 0;
	if (isEncap)
	{
		//send 下 第四个
		//66 89 03 44 88 73 ?? 66 89 6B ?? 89 73 ?? 44 8D 40 ?? 40 88 7B ?? 48 8B 0D ?? ?? ?? ?? 48 8B 49 ??
		//call ??挨着的
		for (QWORD offset : USEOBJ_ENCAP_RCX_OFFSETS)
		{
			rcx = SafeReadQWORD((QWORD*)(rcx + offset));
		}

		byte bate_array[] = { 0x28,0x00,0x00,0x01,(byte)index,0x00,(byte)objId,(byte)(objId >> 8),(byte)(objId >> 16),0x00 };
		//Call_输出调试信息()
		CString str;
		for (byte byte1 : bate_array) {
			str.AppendFormat(_T("%02X "), byte1);
		}

		Call_输出调试信息("调试信息:使用物品封包:%S", str);
		Call_输出调试信息("调试信息:使用物品封包 rcx = %p", rcx);
		//R8:A 是封包长度

		//保留这种形式 提醒不能call_useObj_encap(rcx, bate_array, 0xA, 0x1);//r8=000000000000010A r9=000000001062AB01
		QWORD r8 = 0x000000000000000A;
		QWORD r9 = 0x1;
		QWORD callAddress = 0x00000001409DDA40;
		call_useObj_encap(rcx, bate_array, r8, r9, callAddress);
	}
	else {
		//send下第五个
		//88 44 24 20 45 8B C4 33 D2
		//call挨着D2

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
		Call_输出调试信息("调试信息:%p	%p	%d	%p	%p", rcx, 0x0, index, objId, callAddress);
		call_useObj(rcx, 0x0, index, objId, callAddress);
	}
}
// 选中对象
void selectObj(DWORD id) {
	//send下第六个
	//48 8B 15 ?? ?? ?? ?? 48 8B 4A ?? 8B D0 48 81 C1 70 01 00 00 E8 ?? ?? ?? ?? 45 33 C0 8B D7
	//E8 后面就是call地址
	QWORD callAddress = 0x1409AB4F0;

	Call_输出调试信息("参数1:%p,参数2:%p,参数3:%p", BASE_ADDRESS_ATT, id, callAddress);
	call_Select(BASE_ADDRESS_ATT, id, callAddress);
}

// 逐霜黑龙袭脚底瞬移
void teleportation(DWORD obj_location_ptr) {
	//send下
	//45 33 C9 45 0F B6 C4 8B 52 ? ? 48 81 C1 70 01 00 00
	//结尾00 挨着就是call

	QWORD callAddress = 0x1409AB1B0;
	QWORD rdx = 0x1ECF;  //逐霜 黑龙袭
	QWORD r8 = 0x0;
	QWORD r9 = 0x0;
	QWORD rsp20 = obj_location_ptr;
	QWORD rsp28 = 0x0;
	call_teleportation(BASE_ADDRESS_ATT, rdx, r8, r9, rsp20, rsp28, callAddress);
}

// 使用回城卷
void goHome() {
	traversePackage();
	seekCityRoll();
}
// 喊话
void hanHua(CString str) {
	//Call_输出调试信息("调试信息:!!!");
	// send 下第四个
	//8B 45 9C 89 44 24 20 44 8B CB
	//call CB挨到
	if (isString((QWORD*)str.GetString(), 60))
	{
		//Call_输出调试信息("调试信息:!!!%p", str);
		QWORD rcx = SafeReadQWORD((QWORD*)(BASE_ADDRESS_ATT));
		rcx = SafeReadQWORD((QWORD*)(rcx + 0x40));
		QWORD callAddress = 0x00000001409DD520;
		call_hanHua(rcx, NULL, str, callAddress);
	}
}
// 遍历背包
std::vector<T包裹物品属性> traversePackage() {
	CString str;
	T包裹物品属性 obj;

	OBJS_ATT.clear();

	QWORD tfirst = SafeReadQWORD((QWORD*)(BASE_ADDRESS_ATT));

	for (QWORD offset : objOffset.d一级偏移vector)
	{
		tfirst = SafeReadQWORD((QWORD*)(tfirst + offset));
	}

	if (objOffset.d一级偏移r == 0) {
		objOffset.d一级偏移r = tfirst;
	}

	QWORD tsecond;
	tsecond = SafeReadQWORD((QWORD*)(objOffset.d一级偏移r + objOffset.d二级偏移));

	DWORD tcount;
	tcount = SafeReadDWORD((DWORD*)(tsecond + objOffset.d包裹上限偏移));

	if (tcount > 288)
	{
		return std::vector<T包裹物品属性>();
	}

	for (size_t i = 0; i < tcount; i++)
	{
		QWORD tobj;
		tobj = SafeReadQWORD((QWORD*)(tsecond + objOffset.d包裹对象偏移));
		tobj = SafeReadQWORD((QWORD*)(tobj + i * 0x8));
		//CString str3;
		//str3.Format(_T("Pointer1 address: 0x%p"), tobj); // %p 用于格式化指针
		//AfxMessageBox(str3);
		if (tobj == 0)
		{
			continue;
		}
		obj.q物品ID = SafeReadQWORD((QWORD*)(tobj + 0xC));
		obj.q数量 = SafeReadQWORD((QWORD*)(tobj + 0x18));
		obj.d是否可使用 = (char)SafeReadQWORD((QWORD*)(tobj + 0x20));
		obj.q所在格数 = i;
		for (std::vector<QWORD> packetOffsets : objOffset.d包裹物品名称偏移)
		{
			QWORD tname = tobj;
			QWORD* tname_r = nullptr;
			size_t i = packetOffsets.size();
			for (QWORD offset : packetOffsets)
			{
				i--;
				//AfxMessageBox(obj.d物品名称);

				if (i != 0)
				{
					tname = SafeReadQWORD((QWORD*)(tname + offset));
				}
				else {
					tname_r = (QWORD*)(tname + offset);
					//str.Format(_T("Pointer2 address: %X"), tname_r); // %p 用于格式化指针
					//g_Log.Write(str);
				}
			}

			if (isString(tname_r, 30)) {
				obj.p物品名称 = (wchar_t*)tname_r;
				break;
			}
		}
		OBJS_ATT.push_back(obj);
	}
	return OBJS_ATT;
}
// 遍历周围玩家人物
std::vector<T人物属性> traversePerson() {
	//4C 8D 45 B7 48 8D 55 D7 48 8D 4D FF

	QWORD startAdd = SafeReadQWORD((QWORD*)(SafeReadQWORD_Arry_NotLast(PERSON_AROUD_OFFSETS) + 0x10));
	Call_输出调试信息("调试信息:startAdd = %p", startAdd);
	QWORD size = SafeReadQWORD((QWORD*)(SafeReadQWORD_Arry_NotLast(PERSON_AROUD_OFFSETS) + 0x28));
	Call_输出调试信息("调试信息:size = %d", size);
	QWORD objCount = 0;
	QWORD obj = 0;
	T人物属性 obj_att;
	std::vector<T人物属性> obj_att_arry;
	for (size_t i = 0; i < size; i++)
	{
		obj = SafeReadQWORD((QWORD*)(startAdd + i * 8));
		if (obj == 0)
		{
			continue;
		}
		else {
			Call_输出调试信息("调试信息:obj = %p", obj);
			obj_att.dID = SafeReadDWORD((DWORD*)(obj + obj_att.offset.dID偏移));
			selectObj(obj_att.dID);
			DWORD temp = SafeReadDWORD((DWORD*)(obj + 8));
			obj_att.d等级 = SafeReadDWORD((DWORD*)(temp + obj_att.offset.d等级偏移));
			obj_att.d血量 = SafeReadDWORD((DWORD*)(temp + obj_att.offset.d血量偏移));
			obj_att.p坐标ptr = (DWORD*)(temp + obj_att.offset.fX偏移);
			obj_att.f距离 = calculateDistance(tperson.p坐标ptr, obj_att.p坐标ptr);
			obj_att_arry.push_back(obj_att);
			objCount++;
		}
	}
	Call_输出调试信息("调试信息:objCount = %d", objCount);
	return obj_att_arry;
}
// 遍历技能
std::vector<T技能> traverseSkill() {
	std::vector<T技能> TSkills;
	T技能 TSkill;
	QWORD address_一级 = 0;
	for (QWORD offset : TSkill.skillOffsets.offsets_一级) {
		address_一级 = SafeReadQWORD((QWORD*)(address_一级 + offset));
		//Call_输出调试信息("1:%p ", address_一级);
		//Call_输出调试信息("2:%p ", offset);

	}
	size_t skillMax = SafeReadDWORD((DWORD*)(address_一级 + TSkill.skillOffsets.offset_技能遍历最大数量));
	//Call_输出调试信息("一级地址:%p ", address_一级);
	Call_输出调试信息("skillMax:%d ", skillMax);
	QWORD address_首地址 = SafeReadQWORD((QWORD*)(address_一级 + TSkill.skillOffsets.offset_技能数组首地址));
	//for (size_t i = 0; i < skillMax; i++)
	for (size_t i = 0; i < skillMax; i++)
	{
		if (EXITFLAG) {
			break;
		}
		//Call_输出调试信息("i: %llu, skillMax: %llu", i, skillMax);

		DWORD address_技能对象 = SafeReadDWORD((DWORD*)(address_首地址 + 8 * i));

		TSkill.d技能ID = SafeReadDWORD((DWORD*)(address_技能对象 + TSkill.skillOffsets.offset_技能ID));
		TSkill.d技能冷却时间 = SafeReadDWORD((DWORD*)(address_技能对象 + TSkill.skillOffsets.offset_技能冷却时间));
		TSkill.d技能最大冷却时间 = SafeReadDWORD((DWORD*)(address_技能对象 + TSkill.skillOffsets.offset_技能最大冷却时间));
		TSkill.d技能对象 = address_技能对象;


		Call_输出调试信息("address_技能对象:%p ", address_技能对象);
		Call_输出调试信息("qq技能ID:%d ", TSkill.d技能ID);
		//Call_输出调试信息("q技能冷却时间:%d ", TSkill.q技能冷却时间);
		//Call_输出调试信息("qq技能最大冷却时间:%d ", TSkill.q技能最大冷却时间);

		if (Viewer_packet::cSKILL_NAME_ARRY.empty())
		{
			break;
		}


		for (const CString& index : Viewer_packet::cSKILL_NAME_ARRY)
		{
			//Call_输出调试信息W(_T("index:%s"), index.GetString());
			CString strSkillID;

			size_t temp10 = TSkill.d技能ID;
			temp10 = temp10 * 10;

			strSkillID.Format(_T("%llu"), temp10); // 将 QWORD 转换为整形字符串
			//Call_输出调试信息("strSkillID:%S", strSkillID.GetString());
			if (index.Find(strSkillID) == 0)
			{
				// 查找第一个双引号的位置
				int start = index.Find(_T('"'));
				if (start == -1)
				{
					Call_输出调试信息("错误：未找到开始双引号");
					continue; // 跳过当前循环
				}

				// 查找第二个双引号的位置
				int end = index.Find(_T('"'), start + 1);
				if (end == -1)
				{
					Call_输出调试信息("错误：未找到结束双引号");
					continue; // 跳过当前循环
				}
				// 提取双引号之间的内容
				TSkill.p技能名称 = index.Mid(start + 1, end - start - 1);

				Call_输出调试信息W(_T("找到的技能名称:%s "), TSkill.p技能名称);

				break;
			}
			
		}
		TSkills.push_back(TSkill);



		/*QWORD 技能名称temp = address_技能对象;
		for (QWORD offset : TSkill.skillOffsets.offsets_技能名称)
		{
			技能名称temp = SafeReadQWORD((QWORD*)(技能名称temp + offset));
		}
		Call_输出调试信息("qq技能名称temp:%p ", 技能名称temp);*/

		//wchar_t constrcName[15];
		//try
		//{
		//	
		//	wchar_t* wptr = (wchar_t*)技能名称temp;
		//	wptr++; //忽略第一个f
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
		//	Call_输出调试信息("技能名称字符串转化失败");
		//}




		//Call_输出调试信息("constrcName:%p ", constrcName);
		/*if (isString((QWORD*)constrcName, 15))
		{
			TSkill.p技能名称 = constrcName;
		}*/
	}

	return	TSkills;
}


void seekCityRoll() {
	const CString cityRollName = L"回城券";
}
// 获取自身人物属性
T人物属性  getPersonAtt() {
	QWORD userName = SafeReadQWORD((QWORD*)tPersonOffset.BASE_ADDRESS_NAME);
	if (isString((QWORD*)userName, 20)) {
		tperson.p名称 = (wchar_t*)userName;
	}
	else {
		tperson.p名称 = L"error";
	}

	QWORD tfirst = SafeReadQWORD((QWORD*)(BASE_ADDRESS_ATT));
	for (QWORD offset : tPersonOffset.d一级偏移vector)
	{
		tfirst = SafeReadQWORD((QWORD*)(tfirst + offset));
	}

	if (tPersonOffset.d一级偏移r == 0) {
		tPersonOffset.d一级偏移r = tfirst;
		tperson.p坐标ptr = (DWORD*)(tfirst + tPersonOffset.fX偏移);
	}

	tperson.d血量 = SafeReadDWORD((DWORD*)(tfirst + tPersonOffset.d血量偏移));
	tperson.d真气 = SafeReadDWORD((DWORD*)(tfirst + tPersonOffset.d真气偏移));
	tperson.d元力 = SafeReadDWORD((DWORD*)(tfirst + tPersonOffset.d元力偏移));
	tperson.d无视 = SafeReadDWORD((DWORD*)(tfirst + tPersonOffset.d无视偏移));
	tperson.d减免 = SafeReadDWORD((DWORD*)(tfirst + tPersonOffset.d减免偏移));

	return tperson;
}

bool isString(QWORD* ptr, size_t maxLength = 20) {
	//CString str;
	//str.Format(_T("Pointer address: 0x%p"), BASE_ADDRESS_ATT); // %p 用于格式化指针
	//AfxMessageBox(str); // 弹出消息框显示指针地址
	CString str;
	wchar_t* temp_ptr = (wchar_t*)(ptr);

	if (temp_ptr == nullptr) {
		//AfxMessageBox(_T("Invalid pointer or memory range!"));
		str.Format(_T("Pointer address: 0x%p is nullptr pointer !"), temp_ptr); // %p 用于格式化指针
		g_Log.Write(str);
		return false;  // 空指针不是字符串
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
			return true;  // 遇到空字符，说明是字符串
		}
		temp_ptr++;
		//str.Format(_T("Pointer address: 0x%p!"), temp_ptr); // %p 用于格式化指针
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
		AfxMessageBox(L"读取属性基地址异常");
		MessageBox(NULL, L"读取属性基地址异常", NULL, 0U);
		//exit(1);
		return -20; // 返回默认值或错误码
	}
}