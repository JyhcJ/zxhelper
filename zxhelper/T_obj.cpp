#include "pch.h"
#include "T_obj.h"



QWORD& GetBaseAddress() {
	static QWORD BASE_ADDRESS_ATT = 0x0000000141725098; // Ĭ��ֵ
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

const std::vector<QWORD>& T����ƫ��::Get_offsets_һ��() {
	static  const std::vector<QWORD> offsets_һ�� = { GetBaseAddress(), 0x38,0x60 };
	return offsets_һ��;
}

QWORD T����ƫ��::d����ƫ�� = 0x34E0;


// ������:C3 CC CC 48 89 5C 24 08 44 8B 99 ? ? ? ? ? ? ? ? 33 C0 44 8B D0 45 85 DB 7E
// ƥ��ֵ:24 48 8B 99
QWORD T����ƫ��::offset_���������׵�ַ = 0x3800;

QWORD T����ƫ��::offset_���ܱ���������� = 0x3808;