#include <Windows.h>
#include <vector>
//���Ի���ַ
//const QWORD BASE_ADDRESS_ATT = 0x14171CFD8;
//const QWORD BASE_ADDRESS_ATT = 14171DFD0;
//const QWORD BASE_ADDRESS_ATT = 0x000000014171DFD0;
const QWORD BASE_ADDRESS_ATT = 0x0000000141725098;

// һ��ƫ�� OFFSETS_BASE+558��Ѫ
const std::vector<QWORD> OFFSETS_BASE = { 0x38,0x10,0x48,0x10,0x60 };

// ʹ����ƷRCX��ַ rcx = [[14171CFD8]+40]+170
const std::vector<QWORD> USEOBJ_RCX_OFFSETS = { BASE_ADDRESS_ATT ,0x40 ,0x170 };

//���ʹ����Ʒ  rcx = [[[[[[0x14171CFD8]+40]+f0]+8]+8]+28]
//const std::vector<QWORD> USEOBJ_ENCAP_RCX_OFFSETS = { BASE_ADDRESS_ATT ,0x40 ,0xF0,0x08,0x08,0x28 };
const std::vector<QWORD> USEOBJ_ENCAP_RCX_OFFSETS = { BASE_ADDRESS_ATT ,0x40 };
//const QWORD BASE_ADDRESS_NAME = 0x7FFC440F4978;

//��Χ��������ַ
// [[[[14171DFD0]+38]+10]+48]+30   ��+30��
const std::vector<QWORD> PERSON_AROUD_OFFSETS = { BASE_ADDRESS_ATT ,0x38 ,0x10 ,0x48,0x30 };

//��Ѫƫ��
const int  OFFSET_QIXUE = 0x558;
struct T��������ƫ��
{
	// ��ɫ���ƻ���ַ
	// const QWORD BASE_ADDRESS_NAME = 0x1417243B0;
	// ��ɫ���� : [0x00000001417253C0]��ֵ
	const QWORD BASE_ADDRESS_NAME = 0x00000001417253C0;

	const std::vector<QWORD> dһ��ƫ��vector = OFFSETS_BASE;
	QWORD dһ��ƫ��r = 0;		//����ʱ��ֵ ,��ʵ����dһ��ƫ��vector�Ľ��
	const static DWORD dIDƫ�� = 0x10;
	const static QWORD dѪ��ƫ�� = 0x558;
	const static QWORD d�ȼ�ƫ�� = dѪ��ƫ�� - 0xC;
	const static QWORD d���Ѫ��ƫ�� = 0;
	const static QWORD d����ƫ�� = dѪ��ƫ�� + 0x8;
	const static QWORD dԪ��ƫ�� = dѪ��ƫ�� + 0xC;
	const static QWORD d����ƫ�� = dѪ��ƫ�� + 0x450C;
	const static QWORD d����ƫ�� = dѪ��ƫ�� + 0x450C - 0x8;
	const static DWORD fXƫ�� = 0x48;
	const static DWORD fYƫ�� = fXƫ�� + 0x8;
	const static DWORD fZƫ�� = fXƫ�� + 0x4;
	//FLOAT f����ƫ�� = 0;
	char* p����ƫ�� = 0;
	char pgbk����[100] = { 0 };

};
struct T��������
{
	T��������ƫ�� offset;
	bool ��Ч��;
	DWORD d�ȼ�;
	DWORD dѪ��;
	DWORD d���Ѫ��;
	DWORD d����;
	DWORD dԪ��;
	DWORD d����;
	DWORD d����;
	FLOAT fX;
	DWORD* p����ptr;
	FLOAT fY;
	FLOAT fZ;
	DWORD dID;
	DWORD d����;
	FLOAT f����;
	wchar_t* p����;


	T��������() = default;
};



struct T������Ʒ����
{
	bool b��Ч��;
	wchar_t* p��Ʒ����;
	QWORD q��ƷID;
	QWORD q����;
	QWORD q���ڸ���;
	char d�Ƿ��ʹ��;
};

struct T����ƫ��
{
	/*< Address>0x14171CFD8 < / Address >
	*< Offset>C      4    4< / Offset >����
	*< Offset>90����80	  88< / Offset >
	< Offset>18	C  20< / Offset >18����	C:ID  20:�ܷ�ʹ��?
	< Offset>index * 8 < / Offset >index����λ��
	< Offset>18/20 < / Offset > 18d�������� 20d��������
	< Offset>34e0 < / Offset >����ƫ��
	< Offset>60 < / Offset > OFFSETS_BASE
	< Offset>10 < / Offset >
	< Offset>48 < / Offset >
	< Offset>10 < / Offset >
	< Offset>38 < / Offset >*/

	//[[[[[[0x14171CFD8]+ 38] + 10] + 48] + 10] + 60]
	QWORD dһ��ƫ��r = 0;		//����ʱ��ֵ ,��ʵ����dһ��ƫ��vector�Ľ��
	const std::vector<QWORD> dһ��ƫ��vector = OFFSETS_BASE;


	//[[[[[[[0x14171CFD8]+38]+10]+48]+10]+60]+34e0]
	const QWORD d����ƫ�� = 0x34E0;

	const QWORD d��������ƫ�� = 0x20;//0x24Ҳ��
	const QWORD d��������ƫ�� = 0x18;


	//[[[[[[[[[0x14171CFD8]+38]+10]+48]+10]+60]+34e0]+18]+index * 8]
	const QWORD d�Ƿ��ʹ��ƫ�� = 0x20;
	const QWORD d������ƷIDƫ�� = 0xC;
	const DWORD d������Ʒ����ƫ�� = 0x18;

	//[[[[[[[[[[[0x14171CFD8]+38]+10]+48]+10]+60]+34e0]+18]+index * 8]+90]+c]
	//[[[[[[[[[[[0x14171CFD8]+38]+10]+48]+10]+60]+34e0]+18]+index * 8]+80]+4]
	//[[[[[[[[[[[0x14171CFD8]+38]+10]+48]+10]+60]+34e0]+18]+index * 8]+88]+4]
	//��Ʒ����

	const std::vector<std::vector<QWORD>> d������Ʒ����ƫ�� = { {0x90,0xC},{0x80,0x4},{0x88,0x4},{0x2D0,0x4} ,{ 0x88,0x68 } };//�ϸ�˳����
};
struct T����ƫ�� {
	//  48 89 5C 24 08 44 8B 99 ?? ?? ?? ?? 33 C0 44 8B D0 45 85 DB 7E 24 48 8B 99 ?? ?? ?? ?? 4C 8B CB 49 8B 09 39 51 ?? 74 24 85 D2 48 0F 44 C1 41 FF C2 49 83 C1 08 45 3B D3 7C E6 45 84 C0 74 05 48 85 C0 75 02 33 C0 48 8B 5C 24 08 C3 49 63 C2 48 8B 04 C3 48 8B 5C 24 08 C3 
	//  [[[[14171DFD0]+ 38] + 60] + 3800] ���������׵�ַ
	//	[[[[14171DFD0]+ 38] + 60] + 3808] ���ܱ����������
	//	[[[[[14171DFD0]+ 38] + 60] + 3808] + i * 8] + 10]	����ID
	//	[[[[[14171DFD0]+ 38] + 60] + 3808] + i * 8] + 14]	������ȴʱ��(����)
	//  [[[[][[14171DFD0]+ 38] + 60] + 3808] + i * 8] + 38] + C] �������� 00000000A3E5DC18 66 00 74 66 91
	//  00000000A3E5DC18  66 00 //74 66 91 4E B9 70 E2 6C //00 30 5E 00 66 00  f���Ƶ㲨�� ^ f  ���ַ�ʽ��Ҫ�������� ����Ӧ�������Ż�����ڴ���
	//  000000003BFFF468  66 00 //A7 78 F3 67 47 64 CE 98 //00 30 5E 00 66 00  f����ҡ�硡 ^ f
	//  00000000A3C99928  66 00 //1E 82 34 78 CD 91 71 5C //00 30 5E 00 66 00  f������ɽ�� ^ f


	std::vector<QWORD> offsets_һ�� = { BASE_ADDRESS_ATT, 0x38,0x60 };
	std::vector<QWORD> offsets_�������� = { 0x38,0xC };
	QWORD offset_���ܱ���������� = 0x3808;
	QWORD offset_���������׵�ַ = 0x3800;
	QWORD offset_����ID = 0x10;
	QWORD offset_������ȴʱ�� = offset_����ID + 4;
	QWORD offset_���������ȴʱ�� = offset_����ID + 8;





};
struct T���� {
	T����ƫ�� skillOffsets;
	CStringW p��������;  //wchar_t 2�ֽ�
	DWORD d����ID;
	DWORD d������ȴʱ��;
	DWORD d���������ȴʱ��;
	DWORD d���ܶ���;
};
