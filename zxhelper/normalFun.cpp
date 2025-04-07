#include "pch.h"

#include "normalFun.h"
#include "zxhelper.h"
#include <functional>
#include <immintrin.h>
#include <vector>
#include "counst.h"
#include <cstring> // memcmp
#include <iostream>
#include <Psapi.h> // GetModuleInformation
#include <vector>
#include <Windows.h>
//#include "afxdialogex.h"

//��ȫ���ز�����ַ��ֵ
QWORD SafeReadQWORD(QWORD* ptr) {
	if (ptr == nullptr || !IsValidPointer(ptr, sizeof(QWORD))) {
		return -10; // ����Ĭ��ֵ�������
	}
	return *(QWORD*)ptr;
}

float SafeReadFloat(DWORD* ptr) {
	if (ptr == nullptr || !IsValidPointer(ptr, sizeof(float))) {
		return -20; // ����Ĭ��ֵ�������
	}
	return *(float*)ptr;
}

QWORD SafeReadQWORD_Arry_NotLast(std::vector<QWORD> arry) {
	size_t i = arry.size();
	QWORD temp = 0;
	QWORD ret = 0;
	for (QWORD offset : arry)
	{
		i--;
		if (i != 0)
		{
			temp = SafeReadQWORD((QWORD*)(temp + offset));
		}
		else {
			ret = temp + offset;
		}
	}
	return ret;
}

DWORD SafeReadDWORD(DWORD* ptr) {
	if (ptr == nullptr || !IsValidPointer(ptr, sizeof(DWORD))) {
		return -30; // ����Ĭ��ֵ�������
	}
	return *(DWORD*)ptr;
}

// ͨ�õ� SafeRead ����
template <typename T>
T SafeRead(T* ptr) {
	if (ptr == nullptr || !IsValidPointer(ptr, sizeof(T))) {
		return static_cast<T>(-100); // ����Ĭ��ֵ�������
	}
	return *ptr;
}

//QWORD ReadChain(QWORD base,std::vector<QWORD> offsets) {
//    QWORD value = base;
//    for (QWORD offset : offsets) {
//        value = SafeReadQWORD((QWORD*)(value + offset));
//        if (value == -10) {
//            break; // ��ȡʧ��
//        }
//    }
//    return value;
//}

void ReadUtf8FileAndSplitByNewline(CStringArray& lines)
{
	// �����ļ�ѡ��Ի���
	CFileDialog fileDlg(TRUE);  // TRUE ��ʾ���ļ��Ի���
	fileDlg.m_ofn.lpstrTitle = _T("ѡ���ļ�");  // ���öԻ������
	fileDlg.m_ofn.lpstrFilter = _T("�ı��ļ� (*.txt)\0*.txt\0�����ļ� (*.*)\0*.*\0");  // �����ļ�������

	CString filePath;
	if (fileDlg.DoModal() == IDOK)
	{
		filePath = fileDlg.GetPathName();  // ��ȡ�û�ѡ����ļ�·��
		if (filePath.IsEmpty())
		{
			AfxMessageBox(_T("δѡ���ļ���"));
			return;
		}
	}
	else
	{
		return; // �û�ȡ��ѡ��
	}

	// ���ļ�
	CStdioFile file;
	if (!file.Open(filePath, CFile::modeRead | CFile::typeBinary))
	{
		AfxMessageBox(_T("�޷����ļ���"));
		return;
	}

	// ��ȡ�ļ���С
	ULONGLONG fileSize = file.GetLength();
	if (fileSize == 0)
	{
		AfxMessageBox(_T("�ļ�Ϊ�գ�"));
		file.Close();
		return;
	}

	// ��ȡ�ļ�����
	CStringA contentA;
	char* buffer = contentA.GetBuffer(static_cast<int>(fileSize));
	file.Read(buffer, static_cast<UINT>(fileSize));
	contentA.ReleaseBuffer();
	file.Close();

	// ��鲢���� UTF-8 BOM��������ڣ�
	const char* pData = contentA.GetString();
	if (fileSize >= 3 && static_cast<unsigned char>(pData[0]) == 0xEF &&
		static_cast<unsigned char>(pData[1]) == 0xBB &&
		static_cast<unsigned char>(pData[2]) == 0xBF)
	{
		pData += 3; // ���� BOM
		contentA = pData; // ���¸�ֵ
	}

	// �� UTF-8 ת��Ϊ Unicode
	int length = MultiByteToWideChar(CP_UTF8, 0, contentA, -1, nullptr, 0);
	if (length <= 0)
	{
		AfxMessageBox(_T("�ļ�����ת��ʧ�ܣ�"));
		return;
	}

	CString contentW;
	MultiByteToWideChar(CP_UTF8, 0, contentA, -1, contentW.GetBuffer(length), length);
	contentW.ReleaseBuffer();

	// �����з��ָ�����
	int start = 0;
	int end = contentW.Find(_T("\n")); // ���һ��з�

	while (end != -1)
	{
		CString line = contentW.Mid(start, end - start); // ��ȡһ��
		line.TrimRight(_T("\r")); // ȥ�����ܵĻس���
		lines.Add(line); // ��ӵ�����
		start = end + 1; // �ƶ�����һ��
		end = contentW.Find(_T("\n"), start); // �������һ��з�
	}

	// ������һ�У����û�л��з���β��
	if (start < contentW.GetLength())
	{
		CString line = contentW.Mid(start);
		line.TrimRight(_T("\r")); // ȥ�����ܵĻس���
		lines.Add(line);
	}

	// ����ָ�������
	for (int i = 0; i < lines.GetSize(); i++)
	{
		//AfxMessageBox(lines[i]); // ������Ϣ����ʾÿһ��
		//Call_���������Ϣ("������Ϣ:str = %s", lines[i]);
	}

}
// �ȽϺ���������CString��qsort
// �Զ���ȽϺ���
int CompareFirstColumn(const void* a, const void* b) {
	// �� void* ת��Ϊ CString*
	const CString* strA = static_cast<const CString*>(a);
	const CString* strB = static_cast<const CString*>(b);
	int curPos = 0;
	// ��ȡ��һ�е�����
	int numA = _ttoi(strA->Tokenize(_T(" "), curPos)); // ��ȡ��һ���ո�ǰ�Ĳ��ֲ�ת��Ϊ����
	int numB = _ttoi(strB->Tokenize(_T(" "), curPos)); // ��ȡ��һ���ո�ǰ�Ĳ��ֲ�ת��Ϊ����

	// ����һ�е�������������
	return numA - numB;
}
// ��ȡ�ַ�����ͷ������
int ExtractNumber(const CString& str) {
	// �� CString ת��Ϊ const char*
	CStringA strA(str); // ת��Ϊ���ֽ��ַ���
	const char* cstr = strA;

	// ʹ�� strtok_s �ָ��ַ���
	char* context = nullptr; // ���ڴ洢 strtok_s ��״̬
	char* token = strtok_s((char*)cstr, " ", &context); // ���ո�ָ�
	if (token != nullptr) {
		return atoi(token); // ����һ������ת��Ϊ����
	}
	return 0; // ���û�����֣����� 0
}

bool IsValidPointer(void* ptr, size_t size) {
	return !IsBadReadPtr(ptr, size);
}

void Call_���������Ϣ(char* pszFormat, ...)
{
#ifdef _DEBUG
	// ����̶�ǰ׺�ַ���
	const char* prefix = "[������Ϣ] ";
	char szbufFormat[0x1000];
	char szbufFormat_Game[0x1100] = "";
	// ���̶�ǰ׺ƴ�ӵ� szbufFormat_Game
	strcat_s(szbufFormat_Game, prefix);
	va_list argList;
	va_start(argList, pszFormat);//�����б��ʼ��
	vsprintf_s(szbufFormat, pszFormat, argList);
	strcat_s(szbufFormat_Game, szbufFormat);
	OutputDebugStringA(szbufFormat_Game);
	va_end(argList);

#endif
}
void Call_���������ϢW(const wchar_t* pszFormat, ...)
{
#ifdef _DEBUG
	// ����̶�ǰ׺�ַ���
	const wchar_t* prefix = L"[������Ϣ] ";
	wchar_t szbufFormat[0x1000];
	wchar_t szbufFormat_Game[0x1100] = L"";

	// ���̶�ǰ׺ƴ�ӵ� szbufFormat_Game
	wcscat_s(szbufFormat_Game, prefix);

	// ����ɱ����
	va_list argList;
	va_start(argList, pszFormat); // �����б��ʼ��
	vswprintf_s(szbufFormat, pszFormat, argList); // ��ʽ�����ַ��ַ���
	wcscat_s(szbufFormat_Game, szbufFormat); // ƴ�ӽ��
	OutputDebugStringW(szbufFormat_Game); // ��� UTF-16 ������Ϣ
	va_end(argList);
#endif
}
// ����һ��������������ά�ռ�������֮��ľ���
float calculateDistance(DWORD* myptr, DWORD* objptr) {
	Call_���������Ϣ("1:%p ,,,2:%p ", myptr, objptr);
	// ���������ֵ
	float dx = SafeReadFloat(myptr++) - SafeReadFloat(objptr++);
	float dz = SafeReadFloat(myptr++) - SafeReadFloat(objptr++);
	float dy = SafeReadFloat(myptr) - SafeReadFloat(objptr);
	// �������
	float distance = sqrtf(powf(dx, 2.0f) + powf(dy, 2.0f) + powf(dz, 2.0f));
	Call_���������Ϣ("%f", distance);
	return distance;
}

CStringA LoadTextFromResource(int resourceID)
{
	HRSRC hResource = FindResource(AfxGetResourceHandle(), MAKEINTRESOURCE(resourceID), _T("txt"));
	if (!hResource)
	{
		AfxMessageBox(_T("Failed to find resource!"));
		return ("");
	}

	HGLOBAL hMemory = LoadResource(AfxGetResourceHandle(), hResource);
	if (!hMemory)
	{
		AfxMessageBox(_T("Failed to load resource!"));
		return ("");
	}

	DWORD size = SizeofResource(AfxGetResourceHandle(), hResource);
	LPVOID data = LockResource(hMemory);
	if (!data)
	{
		AfxMessageBox(_T("Failed to lock resource!"));
		return ("");
	}

	// ����Դ����ת��ΪCString
	CStringA text((const char*)data, size);
	return text;
}

CStringW UTF8ToUnicode(const char* utf8Str)
{
	if (!utf8Str || *utf8Str == '\0')
	{
		return CStringW();
	}

	// ������Ҫ�Ŀ��ַ���
	int wideCharCount = MultiByteToWideChar(CP_UTF8, 0, utf8Str, -1, nullptr, 0);
	if (wideCharCount == 0)
	{
		return CStringW();
	}

	// ���仺����
	CStringW unicodeStr;
	wchar_t* buffer = unicodeStr.GetBuffer(wideCharCount);

	// ת�� UTF-8 �� UTF-16
	MultiByteToWideChar(CP_UTF8, 0, utf8Str, -1, buffer, wideCharCount);

	// �ͷŻ�����
	unicodeStr.ReleaseBuffer();

	return unicodeStr;
}

// ������%P
void PrintRegisters(CONTEXT* ctx) {
	Call_���������Ϣ("�쳣����ʱ�Ĵ���״̬: RCX=0x%p , RDX=0x%p , R8=0x%p , R9=0x%p", ctx->Rcx, ctx->Rdx, ctx->R8, ctx->R9);
	Call_���������Ϣ("�쳣����ʱ�Ĵ���״̬: RDI=0x%p , RAX=0x%p , RBX=0x%p", ctx->Rdi, ctx->Rax, ctx->Rbx);
	Call_���������Ϣ("�쳣����ʱ�Ĵ���״̬: RSP=0x%p , RBP=0x%p", ctx->Rsp, ctx->Rbp);
}

// ��������˴˷��������쳣����,˵��asm��ȷ,�쳣������call��.
void call_asm(std::function<void()>&& func) {
	CONTEXT ctx;

	__try
	{
		// Move the function object to a local variable

		// Call the function
		std::invoke(func);

		Call_���������Ϣ("__try��ȫ����_call");
	}
	__except (
		ctx = *(GetExceptionInformation())->ContextRecord,
		PrintRegisters(&ctx),
		EXCEPTION_EXECUTE_HANDLER
		) {
		DWORD code;
		code = GetExceptionCode();
		Call_���������Ϣ("�쳣����: 0x%X", code);

		return;
	}
}

void seh_translator(unsigned int code, _EXCEPTION_POINTERS* ep) {
	if (ep && ep->ContextRecord) {
		// �� SEH �쳣��Ϣ��������һ���׳�
		throw SEHException(
			code,
			"SEH�쳣",
			*ep->ContextRecord  // ����������
		);
	}
	else {
		throw SEHException(code, "SEH�쳣���������ģ�", CONTEXT{});
	}
}

LONG WINAPI MyVectoredHandler(EXCEPTION_POINTERS* ExceptionInfo) {
	//std::cout << "�����쳣: " << std::hex << ExceptionInfo->ExceptionRecord->ExceptionCode << std::endl;
	Call_���������Ϣ("�����쳣: 0x%X", ExceptionInfo->ExceptionRecord->ExceptionCode);
	return EXCEPTION_CONTINUE_SEARCH;
}

void safeCall(std::function<void(QWORD*)> fp, QWORD* params) {
	HANDLE hHandler = AddVectoredExceptionHandler(1, MyVectoredHandler);
	fp(params);
	RemoveVectoredExceptionHandler(hHandler);
	/*try
	{
		fp(params);
	}
	catch (const SEHException& e) {
		Call_���������Ϣ("SEH�쳣����: 0x%X, ��Ϣ: %s", e.getCode(), e.what());
		PrintRegisters(e.getContext());
	}*/
}



#pragma comment(lib, "Psapi.lib") // ���� Psapi.lib

// ��ͨ��� "?" �滻Ϊ 0x00����ƥ�������ֽڣ�
//std::vector<BYTE> ParsePattern(const char* pattern) {
//	std::vector<BYTE> bytes;
//	for (const char* p = pattern; *p; ++p) {
//		if (*p == '?') {
//			bytes.push_back(0x00); // ͨ���
//			if (*(p + 1) == '?') ++p; // �����ڶ��� '?'
//		}
//		else if (isxdigit(*p)) {
//			BYTE byte = strtoul(p, nullptr, 16);
//			bytes.push_back(byte);
//			++p; // �����ڶ���ʮ�������ַ�
//		}
//	}
//	return bytes;
//}


std::vector<BYTE> ParsePattern(const char* patternStr) {
	std::vector<BYTE> bytes;
	while (*patternStr) {
		if (*patternStr == ' ') {  // �����ո�
			++patternStr;
			continue;
		}

		if (*patternStr == '?') {  // ����ͨ���
			++patternStr;
			if (*patternStr == '?') ++patternStr;
			bytes.push_back(0x00); // ͨ������
		}
		else if (isxdigit(*patternStr)) {  // ����ʮ�������ֽ�
			if (!isxdigit(patternStr[1])) break; // ȷ��������ʮ����������
			char byteStr[3] = { patternStr[0], patternStr[1], '\0' };
			BYTE byte = (BYTE)strtoul(byteStr, nullptr, 16);
			bytes.push_back(byte);
			patternStr += 2;
		}
		else {
			break; // �����Ƿ��ַ�
		}
	}
	return bytes;
}
// ʹ�� SIMD (SSE4.1) ����ģʽƥ�䣨�� memcmp ���죩
bool MemoryMatchSSE(const BYTE* data, const BYTE* pattern, size_t patternSize) {
	size_t i = 0;

	// ����16�ֽڿ�
	for (; i + 16 <= patternSize; i += 16) {
		__m128i dataChunk = _mm_loadu_si128((__m128i*)(data + i));
		__m128i patternChunk = _mm_loadu_si128((__m128i*)(pattern + i));

		// ����ͨ������루ģʽ��0x00��ʾͨ�����
		__m128i wildcardMask = _mm_cmpeq_epi8(patternChunk, _mm_setzero_si128());
		__m128i comparison = _mm_cmpeq_epi8(dataChunk, patternChunk);

		// ���ͨ������루ƥ��ɹ���ͨ��������ܣ�
		__m128i result = _mm_or_si128(comparison, wildcardMask);

		uint16_t maskBits = _mm_movemask_epi8(result);
		if (maskBits != 0xFFFF) return false;
	}

	// ����ʣ���ֽ�
	for (; i < patternSize; ++i) {
		if (pattern[i] != 0x00 && data[i] != pattern[i]) {
			return false;
		}
	}

	return true;
}

// �� elementclient64.exe ģ���������ֽ�ģʽ��ֱ��ָ����� + SIMD ���٣�
uintptr_t FindPatternInElementClient(const char* patternStr, BYTE* scanStart) {
	std::vector<BYTE> pattern = ParsePattern(patternStr);

	// ��ȡ elementclient64.exe ģ���ַ�ʹ�С
	HMODULE hModule = GetModuleHandleA(MOUDLE_ZX_NAME);
	if (!hModule) {
		std::cerr << "Failed to find elementclient64.exe module!" << std::endl;
		return 0;
	}

	MODULEINFO modInfo;
	if (!GetModuleInformation(GetCurrentProcess(), hModule, &modInfo, sizeof(modInfo))) {
		std::cerr << "Failed to get module info!" << std::endl;
		return 0;
	}

	// ֱ�ӷ���ģ���ڴ�
	if (scanStart == nullptr)
	{
		scanStart = (BYTE*)hModule;
	}

	BYTE* scanEnd = scanStart + modInfo.SizeOfImage;

	// ʹ�� SIMD ��������
	for (BYTE* p = scanStart; p < scanEnd - pattern.size(); ++p) {
		if (MemoryMatchSSE(p, pattern.data(), pattern.size())) {
			return (uintptr_t)p;
		}
	}

	return 0; // δ�ҵ�
}

// ����ģʽ��С������ͨ�����ʮ�������ֽڣ�
size_t GetPatternSize(const char* pattern) {
	size_t count = 0;
	while (*pattern) {
		if (*pattern == ' ') {
			pattern++;
			continue;
		}

		if (*pattern == '?') {  // ����ͨ���
			count++;
			pattern++;
			if (*pattern == '?') pattern++;  // ����˫�ʺ����
		}
		else if (isxdigit(*pattern)) {  // ����ʮ�������ֽ�
			count++;
			pattern += 2;
		}
		else {
			break;  // �Ƿ��ַ�
		}
	}
	return count;
}

// �������ʽ:99 ?? ?? ?? ?? 33 ͨ���һ����?? ������99 ? ? ? ? 33 
QWORD calAddress(char* featureCode, char* cruxCode, bool isAfter, int repeats,bool isOffset = false) {
	// ʾ�������� "66 89 03 44 88 73 ?? ?? 66 89 6B ?? ?? 89 73 ?? ?? ..."
	//const char* featureCode = "66 89 03 44 88 73 ?? ?? 66 89 6B ?? ?? 89 73 ?? ?? 44 8D 40 ?? ?? 40 88 7B ?? ?? 48 8B 0D ?? ?? ?? ?? ?? ?? ?? ?? 48 8B 49 ?? ??";


	// �� elementclient64.exe ģ��������
	uintptr_t foundAddr = FindPatternInElementClient(featureCode, nullptr);
	//Call_���������Ϣ("cruxCode: 0x%s", cruxCode);
	//Call_���������Ϣ("foundAddr: 0x%p", foundAddr);
	if (!foundAddr) {
		Call_���������Ϣ("û����ƥ���������: %s", featureCode);
		return 0;
	}

	//std::cout << "Pattern found at: 0x" << std::hex << foundAddr << std::endl;

	uintptr_t cruxCodeAddr = FindPatternInElementClient(cruxCode, (BYTE*)foundAddr + GetPatternSize(featureCode));
	//Call_���������Ϣ("cruxCodeAddr: 0x%p", cruxCodeAddr);

	uintptr_t nextAddr = cruxCodeAddr + GetPatternSize(cruxCode) + 0x4;


	//Call_���������Ϣ("nextAddr: 0x%p", nextAddr);
	int32_t dexValue = SafeReadDWORD((DWORD*)(nextAddr - 0x4));

	if (isOffset)
	{
		return dexValue;
	}

	QWORD baseAddress = nextAddr + dexValue;
	//Call_���������Ϣ("baseAddress: 0x%p", baseAddress);
	//Call_���������Ϣ("baseAddress: 0x%p", baseAddress);
	// ֱ�Ӷ�ȡĿ���ַ��ֵ
	//uintptr_t value = (uintptr_t)(SafeReadQWORD((QWORD*)nextAddr));

	return baseAddress;
}

