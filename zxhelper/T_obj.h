#include <Windows.h>
#include <vector>
//属性基地址
const QWORD BASE_ADDRESS_ATT = 0x14171CFD8;

const std::vector<QWORD> OFFSETS_BASE = { 0x38,0x10,0x48,0x10,0x60 };

//const QWORD BASE_ADDRESS_NAME = 0x7FFC440F4978;
//角色名称基地址
const QWORD BASE_ADDRESS_NAME = 0x1417243B0;
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
	char* p名称;
	char pgbk名称[100];
	void c初始化();
};

struct T包裹物品属性
{
	wchar_t* d物品名称;
	QWORD d物品ID;
	QWORD d数量;
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
	const std::vector<std::vector<QWORD>> d包裹物品名称偏移 = { {0x90,0xC},{0x80,0x4},{0x88,4} };//严格按顺序来
};