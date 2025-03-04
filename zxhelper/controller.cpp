#include "pch.h"
#include "controller.h"
#include "normalFun.h"
#include "Viewer.h"

extern "C" void call_relex();

//rdx 暂时固定为 0
extern "C" void call_useObj(QWORD rcx, QWORD rdx, QWORD r8, QWORD r9);

extern CLog g_Log;

std::vector<T包裹物品属性> OBJS_ATT;
T人物属性偏移 tPersonOffset;
T包裹偏移 objOffset;
QWORD TEMP_FIRSTOFFSET;
// 打坐
void relex() {
	call_relex();
}

// 使用物品
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
// 使用回城卷
void goHome() {
	traversePackage();
	seekCityRoll();
}

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
		obj.d是否可使用 = SafeReadQWORD((QWORD*)(tobj + 0x20));
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

void seekCityRoll() {
	const CString cityRollName = L"回城券";
}

T人物属性  getPersonAtt() {
	T人物属性 tperson;
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