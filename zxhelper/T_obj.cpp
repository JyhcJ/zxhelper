#include "pch.h"
#include "T_obj.h"



QWORD& GetBaseAddress() {
	static QWORD BASE_ADDRESS_ATT = 0x0000000141725098; // 默认值
	return BASE_ADDRESS_ATT;
}

const std::vector<QWORD>& Get_USEOBJ_RCX_OFFSETS() {
	static const std::vector<QWORD> USEOBJ_RCX_OFFSETS = {
		GetBaseAddress(), 0x40, 0x170
	};
	return USEOBJ_RCX_OFFSETS;
}

const std::vector<QWORD>& Get_USEOBJ_ENCAP_RCX_OFFSETS() {
	static  const std::vector<QWORD> USEOBJ_ENCAP_RCX_OFFSETS = { GetBaseAddress() ,0x40 };
	return USEOBJ_ENCAP_RCX_OFFSETS;
}

const std::vector<QWORD>& Get_PERSON_AROUD_OFFSETS() {
	static  const std::vector<QWORD> PERSON_AROUD_OFFSETS = { GetBaseAddress() ,0x38 ,0x10 ,0x48,0x30 };
	return PERSON_AROUD_OFFSETS;
}

const std::vector<QWORD>& T技能偏移::Get_offsets_一级() {
	static  const std::vector<QWORD> offsets_一级 = { GetBaseAddress(), 0x38,0x60 };
	return offsets_一级;
}

QWORD T包裹偏移::d二级偏移 = 0x34E0;


// 特征码:C3 CC CC 48 89 5C 24 08 44 8B 99 ? ? ? ? ? ? ? ? 33 C0 44 8B D0 45 85 DB 7E
// 匹配值:24 48 8B 99
QWORD T技能偏移::offset_技能数组首地址 = 0x3800;

QWORD T技能偏移::offset_技能遍历最大数量 = 0x3808;