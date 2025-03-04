#include <Windows.h>
#include <vector>
//属性基地址
const QWORD BASE_ADDRESS_ATT = 0x14171CFD8;

// 一级偏移
// OFFSETS_BASE+558气血
// ...
const std::vector<QWORD> OFFSETS_BASE = { 0x38,0x10,0x48,0x10,0x60 };

// 使用物品RCX基址 rcx = [[14171CFD8]+40]+170
const std::vector<QWORD> USEOBJ_RCX_OFFSETS = { BASE_ADDRESS_ATT ,0x40 ,0x170};


//const QWORD BASE_ADDRESS_NAME = 0x7FFC440F4978;

//气血偏移
const int  OFFSET_QIXUE = 0x558;

struct T人物属性
{
	DWORD d血量;
	DWORD d最大血量;
	DWORD d真气;
	DWORD d元力;
	DWORD d无视;
	DWORD d减免;
	FLOAT fX;
	FLOAT fY;
	FLOAT fZ;
	DWORD dID;
	DWORD d对象;
	FLOAT f距离;
	wchar_t* p名称;
};

struct T人物属性偏移
{
	//角色名称基地址
	const QWORD BASE_ADDRESS_NAME = 0x1417243B0;
	
	const std::vector<QWORD> d一级偏移vector = OFFSETS_BASE;
	QWORD d一级偏移r = 0;		//调用时赋值 ,其实就是d一级偏移vector的结果

	QWORD d血量偏移 = 0x558;
	QWORD d最大血量偏移=0 ;
	QWORD d真气偏移 = d血量偏移 + 0x8;
	QWORD d元力偏移 = d血量偏移 + 0xC;
	QWORD d无视偏移 = d血量偏移 + 0x450C;
	QWORD d减免偏移 = d血量偏移 + 0x450C - 0x8;
	FLOAT fX偏移=0;
	FLOAT fY偏移=0;
	FLOAT fZ偏移=0;
	QWORD dID偏移=0;
	QWORD d对象偏移=0;
	FLOAT f距离偏移=0;
	char* p名称偏移=0;
	char pgbk名称[100] = {0};

};

struct T包裹物品属性
{
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

	const std::vector<QWORD> d一级偏移vector = OFFSETS_BASE;
	QWORD d一级偏移r = 0;		//调用时赋值 ,其实就是d一级偏移vector的结果
	const QWORD d二级偏移 = 0x34E0;
	const QWORD d包裹上限偏移 = 0x20;//0x24也是
	const QWORD d包裹对象偏移 = 0x18;
	const QWORD d包裹物品ID偏移 = 0xC;
	const QWORD d包裹物品数量偏移 = 0x18;
	const QWORD d是否可使用偏移 = 0x20;
	const std::vector<std::vector<QWORD>> d包裹物品名称偏移 = { {0x90,0xC},{0x80,0x4},{0x88,0x4},{0x2D0,0x4} ,{ 0x88,0x68 } };//严格按顺序来
};