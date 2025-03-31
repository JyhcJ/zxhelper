#include <Windows.h>
#include <vector>
//属性基地址
//const QWORD BASE_ADDRESS_ATT = 0x14171CFD8;
//const QWORD BASE_ADDRESS_ATT = 14171DFD0;
//const QWORD BASE_ADDRESS_ATT = 0x000000014171DFD0;
const QWORD BASE_ADDRESS_ATT = 0x0000000141725098;

// 一级偏移 OFFSETS_BASE+558气血
const std::vector<QWORD> OFFSETS_BASE = { 0x38,0x10,0x48,0x10,0x60 };

// 使用物品RCX基址 rcx = [[14171CFD8]+40]+170
const std::vector<QWORD> USEOBJ_RCX_OFFSETS = { BASE_ADDRESS_ATT ,0x40 ,0x170 };

//封包使用物品  rcx = [[[[[[0x14171CFD8]+40]+f0]+8]+8]+28]
//const std::vector<QWORD> USEOBJ_ENCAP_RCX_OFFSETS = { BASE_ADDRESS_ATT ,0x40 ,0xF0,0x08,0x08,0x28 };
const std::vector<QWORD> USEOBJ_ENCAP_RCX_OFFSETS = { BASE_ADDRESS_ATT ,0x40 };
//const QWORD BASE_ADDRESS_NAME = 0x7FFC440F4978;

//周围人物对象基址
// [[[[14171DFD0]+38]+10]+48]+30   是+30奥
const std::vector<QWORD> PERSON_AROUD_OFFSETS = { BASE_ADDRESS_ATT ,0x38 ,0x10 ,0x48,0x30 };

//气血偏移
const int  OFFSET_QIXUE = 0x558;
struct T人物属性偏移
{
	// 角色名称基地址
	// const QWORD BASE_ADDRESS_NAME = 0x1417243B0;
	// 角色名称 : [0x00000001417253C0]的值
	const QWORD BASE_ADDRESS_NAME = 0x00000001417253C0;

	const std::vector<QWORD> d一级偏移vector = OFFSETS_BASE;
	QWORD d一级偏移r = 0;		//调用时赋值 ,其实就是d一级偏移vector的结果
	const static DWORD dID偏移 = 0x10;
	const static QWORD d血量偏移 = 0x558;
	const static QWORD d等级偏移 = d血量偏移 - 0xC;
	const static QWORD d最大血量偏移 = 0;
	const static QWORD d真气偏移 = d血量偏移 + 0x8;
	const static QWORD d元力偏移 = d血量偏移 + 0xC;
	const static QWORD d无视偏移 = d血量偏移 + 0x450C;
	const static QWORD d减免偏移 = d血量偏移 + 0x450C - 0x8;
	const static DWORD fX偏移 = 0x48;
	const static DWORD fY偏移 = fX偏移 + 0x8;
	const static DWORD fZ偏移 = fX偏移 + 0x4;
	//FLOAT f距离偏移 = 0;
	char* p名称偏移 = 0;
	char pgbk名称[100] = { 0 };

};
struct T人物属性
{
	T人物属性偏移 offset;
	bool 有效性;
	DWORD d等级;
	DWORD d血量;
	DWORD d最大血量;
	DWORD d真气;
	DWORD d元力;
	DWORD d无视;
	DWORD d减免;
	FLOAT fX;
	DWORD* p坐标ptr;
	FLOAT fY;
	FLOAT fZ;
	DWORD dID;
	DWORD d对象;
	FLOAT f距离;
	wchar_t* p名称;


	T人物属性() = default;
};



struct T包裹物品属性
{
	bool b有效性;
	wchar_t* p物品名称;
	QWORD q物品ID;
	QWORD q数量;
	QWORD q所在格数;
	char d是否可使用;
};

struct T包裹偏移
{
	/*< Address>0x14171CFD8 < / Address >
	*< Offset>C      4    4< / Offset >名称
	*< Offset>90或者80	  88< / Offset >
	< Offset>18	C  20< / Offset >18数量	C:ID  20:能否使用?
	< Offset>index * 8 < / Offset >index格子位置
	< Offset>18/20 < / Offset > 18d包裹对象 20d包裹上限
	< Offset>34e0 < / Offset >二级偏移
	< Offset>60 < / Offset > OFFSETS_BASE
	< Offset>10 < / Offset >
	< Offset>48 < / Offset >
	< Offset>10 < / Offset >
	< Offset>38 < / Offset >*/

	//[[[[[[0x14171CFD8]+ 38] + 10] + 48] + 10] + 60]
	QWORD d一级偏移r = 0;		//调用时赋值 ,其实就是d一级偏移vector的结果
	const std::vector<QWORD> d一级偏移vector = OFFSETS_BASE;


	//[[[[[[[0x14171CFD8]+38]+10]+48]+10]+60]+34e0]
	const QWORD d二级偏移 = 0x34E0;

	const QWORD d包裹上限偏移 = 0x20;//0x24也是
	const QWORD d包裹对象偏移 = 0x18;


	//[[[[[[[[[0x14171CFD8]+38]+10]+48]+10]+60]+34e0]+18]+index * 8]
	const QWORD d是否可使用偏移 = 0x20;
	const QWORD d包裹物品ID偏移 = 0xC;
	const DWORD d包裹物品数量偏移 = 0x18;

	//[[[[[[[[[[[0x14171CFD8]+38]+10]+48]+10]+60]+34e0]+18]+index * 8]+90]+c]
	//[[[[[[[[[[[0x14171CFD8]+38]+10]+48]+10]+60]+34e0]+18]+index * 8]+80]+4]
	//[[[[[[[[[[[0x14171CFD8]+38]+10]+48]+10]+60]+34e0]+18]+index * 8]+88]+4]
	//物品名称

	const std::vector<std::vector<QWORD>> d包裹物品名称偏移 = { {0x90,0xC},{0x80,0x4},{0x88,0x4},{0x2D0,0x4} ,{ 0x88,0x68 } };//严格按顺序来
};
struct T技能偏移 {
	//  48 89 5C 24 08 44 8B 99 ?? ?? ?? ?? 33 C0 44 8B D0 45 85 DB 7E 24 48 8B 99 ?? ?? ?? ?? 4C 8B CB 49 8B 09 39 51 ?? 74 24 85 D2 48 0F 44 C1 41 FF C2 49 83 C1 08 45 3B D3 7C E6 45 84 C0 74 05 48 85 C0 75 02 33 C0 48 8B 5C 24 08 C3 49 63 C2 48 8B 04 C3 48 8B 5C 24 08 C3 
	//  [[[[14171DFD0]+ 38] + 60] + 3800] 技能数组首地址
	//	[[[[14171DFD0]+ 38] + 60] + 3808] 技能遍历最大数量
	//	[[[[[14171DFD0]+ 38] + 60] + 3808] + i * 8] + 10]	技能ID
	//	[[[[[14171DFD0]+ 38] + 60] + 3808] + i * 8] + 14]	技能冷却时间(毫秒)
	//  [[[[][[14171DFD0]+ 38] + 60] + 3808] + i * 8] + 38] + C] 技能名称 00000000A3E5DC18 66 00 74 66 91
	//  00000000A3E5DC18  66 00 //74 66 91 4E B9 70 E2 6C //00 30 5E 00 66 00  f晴云点波　 ^ f  这种方式需要触发才能 打开相应技能面板才会加入内存中
	//  000000003BFFF468  66 00 //A7 78 F3 67 47 64 CE 98 //00 30 5E 00 66 00  f碧柳摇风　 ^ f
	//  00000000A3C99928  66 00 //1E 82 34 78 CD 91 71 5C //00 30 5E 00 66 00  f舞破重山　 ^ f


	std::vector<QWORD> offsets_一级 = { BASE_ADDRESS_ATT, 0x38,0x60 };
	std::vector<QWORD> offsets_技能名称 = { 0x38,0xC };
	QWORD offset_技能遍历最大数量 = 0x3808;
	QWORD offset_技能数组首地址 = 0x3800;
	QWORD offset_技能ID = 0x10;
	QWORD offset_技能冷却时间 = offset_技能ID + 4;
	QWORD offset_技能最大冷却时间 = offset_技能ID + 8;





};
struct T技能 {
	T技能偏移 skillOffsets;
	CStringW p技能名称;  //wchar_t 2字节
	DWORD d技能ID;
	DWORD d技能冷却时间;
	DWORD d技能最大冷却时间;
	DWORD d技能对象;
};
