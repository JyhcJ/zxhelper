#include <Windows.h>
#include <vector>
//���Ի���ַ
const QWORD BASE_ADDRESS_ATT = 0x14171CFD8;

const std::vector<QWORD> OFFSETS_BASE = { 0x38,0x10,0x48,0x10,0x60 };

//const QWORD BASE_ADDRESS_NAME = 0x7FFC440F4978;
//��ɫ���ƻ���ַ
const QWORD BASE_ADDRESS_NAME = 0x1417243B0;
//��Ѫƫ��
const int  OFFSET_QIXUE = 0x558;
struct T��������
{
	DWORD dѪ��;
	DWORD d���Ѫ��;
	DWORD d����;
	DWORD dԪ��;
	DWORD d����;
	DWORD d����;
	FLOAT fX;
	FLOAT fY;
	FLOAT fZ;
	DWORD dID;
	DWORD d����;
	FLOAT f����;
	char* p����;
	char pgbk����[100];
	void c��ʼ��();
};

struct T������Ʒ����
{
	wchar_t* d��Ʒ����;
	QWORD d��ƷID;
	QWORD d����;
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

	const std::vector<QWORD> dһ��ƫ��vector = OFFSETS_BASE;
	QWORD dһ��ƫ��r = 0;		//����ʱ��ֵ ,��ʵ����dһ��ƫ��vector�Ľ��
	const QWORD d����ƫ�� = 0x34E0;
	const QWORD d��������ƫ�� = 0x20;//0x24Ҳ��
	const QWORD d��������ƫ�� = 0x18;
	const QWORD d������ƷIDƫ�� = 0xC;
	const QWORD d������Ʒ����ƫ�� = 0x18;
	const QWORD d�Ƿ��ʹ��ƫ�� = 0x20;
	const std::vector<std::vector<QWORD>> d������Ʒ����ƫ�� = { {0x90,0xC},{0x80,0x4},{0x88,4} };//�ϸ�˳����
};