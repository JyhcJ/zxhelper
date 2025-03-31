#include "pch.h"
#include "normalFun.h"
#include "zxhelper.h"
#include <vector>
#include <functional>
//#include "afxdialogex.h"

//安全返回参数地址的值
QWORD SafeReadQWORD(QWORD* ptr) {
	if (ptr == nullptr || !IsValidPointer(ptr, sizeof(QWORD))) {
		return -10; // 返回默认值或错误码
	}
	return *(QWORD*)ptr;
}

float SafeReadFloat(DWORD* ptr) {
	if (ptr == nullptr || !IsValidPointer(ptr, sizeof(float))) {
		return -20; // 返回默认值或错误码
	}
	return *(float*)ptr;
}

QWORD SafeReadQWORD_Arry_NotLast(std::vector<QWORD> arry) {
	size_t i = arry.size();
	QWORD temp = 0;
	QWORD ret = 0 ;
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
		return -30; // 返回默认值或错误码
	}
	return *(DWORD*)ptr;
}

// 通用的 SafeRead 函数
template <typename T>
T SafeRead(T* ptr) {
	if (ptr == nullptr || !IsValidPointer(ptr, sizeof(T))) {
		return static_cast<T>(-100); // 返回默认值或错误码
	}
	return *ptr;
}

//QWORD ReadChain(QWORD base,std::vector<QWORD> offsets) {
//    QWORD value = base;
//    for (QWORD offset : offsets) {
//        value = SafeReadQWORD((QWORD*)(value + offset));
//        if (value == -10) {
//            break; // 读取失败
//        }
//    }
//    return value;
//}

void ReadUtf8FileAndSplitByNewline(CStringArray& lines)
{
	// 创建文件选择对话框
	CFileDialog fileDlg(TRUE);  // TRUE 表示打开文件对话框
	fileDlg.m_ofn.lpstrTitle = _T("选择文件");  // 设置对话框标题
	fileDlg.m_ofn.lpstrFilter = _T("文本文件 (*.txt)\0*.txt\0所有文件 (*.*)\0*.*\0");  // 设置文件过滤器

	CString filePath;
	if (fileDlg.DoModal() == IDOK)
	{
		filePath = fileDlg.GetPathName();  // 获取用户选择的文件路径
		if (filePath.IsEmpty())
		{
			AfxMessageBox(_T("未选择文件！"));
			return ;
		}
	}
	else
	{
		return; // 用户取消选择
	}

	// 打开文件
	CStdioFile file;
	if (!file.Open(filePath, CFile::modeRead | CFile::typeBinary))
	{
		AfxMessageBox(_T("无法打开文件！"));
		return ;
	}

	// 获取文件大小
	ULONGLONG fileSize = file.GetLength();
	if (fileSize == 0)
	{
		AfxMessageBox(_T("文件为空！"));
		file.Close();
		return ;
	}

	// 读取文件内容
	CStringA contentA;
	char* buffer = contentA.GetBuffer(static_cast<int>(fileSize));
	file.Read(buffer, static_cast<UINT>(fileSize));
	contentA.ReleaseBuffer();
	file.Close();

	// 检查并跳过 UTF-8 BOM（如果存在）
	const char* pData = contentA.GetString();
	if (fileSize >= 3 && static_cast<unsigned char>(pData[0]) == 0xEF &&
		static_cast<unsigned char>(pData[1]) == 0xBB &&
		static_cast<unsigned char>(pData[2]) == 0xBF)
	{
		pData += 3; // 跳过 BOM
		contentA = pData; // 重新赋值
	}

	// 将 UTF-8 转换为 Unicode
	int length = MultiByteToWideChar(CP_UTF8, 0, contentA, -1, nullptr, 0);
	if (length <= 0)
	{
		AfxMessageBox(_T("文件内容转换失败！"));
		return ;
	}

	CString contentW;
	MultiByteToWideChar(CP_UTF8, 0, contentA, -1, contentW.GetBuffer(length), length);
	contentW.ReleaseBuffer();

	// 按换行符分割内容
	int start = 0;
	int end = contentW.Find(_T("\n")); // 查找换行符

	while (end != -1)
	{
		CString line = contentW.Mid(start, end - start); // 提取一行
		line.TrimRight(_T("\r")); // 去除可能的回车符
		lines.Add(line); // 添加到数组
		start = end + 1; // 移动到下一行
		end = contentW.Find(_T("\n"), start); // 继续查找换行符
	}

	// 添加最后一行（如果没有换行符结尾）
	if (start < contentW.GetLength())
	{
		CString line = contentW.Mid(start);
		line.TrimRight(_T("\r")); // 去除可能的回车符
		lines.Add(line);
	}

	// 输出分割后的内容
	for (int i = 0; i < lines.GetSize(); i++)
	{
		//AfxMessageBox(lines[i]); // 弹出消息框显示每一行
		//Call_输出调试信息("调试信息:str = %s", lines[i]);
	}
	
}
// 比较函数，用于CString的qsort
// 自定义比较函数
int CompareFirstColumn(const void* a, const void* b) {
	// 将 void* 转换为 CString*
	const CString* strA = static_cast<const CString*>(a);
	const CString* strB = static_cast<const CString*>(b);
	int curPos = 0;
	// 提取第一列的整数
	int numA = _ttoi(strA->Tokenize(_T(" "), curPos)); // 提取第一个空格前的部分并转换为整数
	int numB = _ttoi(strB->Tokenize(_T(" "), curPos)); // 提取第一个空格前的部分并转换为整数

	// 按第一列的整数升序排序
	return numA - numB;
}
// 提取字符串开头的数字
int ExtractNumber(const CString& str) {
	// 将 CString 转换为 const char*
	CStringA strA(str); // 转换为多字节字符串
	const char* cstr = strA;

	// 使用 strtok_s 分割字符串
	char* context = nullptr; // 用于存储 strtok_s 的状态
	char* token = strtok_s((char*)cstr, " ", &context); // 按空格分割
	if (token != nullptr) {
		return atoi(token); // 将第一个部分转换为整数
	}
	return 0; // 如果没有数字，返回 0
}

bool IsValidPointer(void* ptr, size_t size) {
	return !IsBadReadPtr(ptr, size);
}

void Call_输出调试信息(char* pszFormat, ...)
{
#ifdef _DEBUG
	// 定义固定前缀字符串
	const char* prefix = "[调试信息] ";
	char szbufFormat[0x1000];
	char szbufFormat_Game[0x1100] = "";
	// 将固定前缀拼接到 szbufFormat_Game
	strcat_s(szbufFormat_Game, prefix);
	va_list argList;
	va_start(argList, pszFormat);//参数列表初始化
	vsprintf_s(szbufFormat, pszFormat, argList);
	strcat_s(szbufFormat_Game, szbufFormat);
	OutputDebugStringA(szbufFormat_Game);
	va_end(argList);

#endif
}
void Call_输出调试信息W(const wchar_t* pszFormat, ...)
{
#ifdef _DEBUG
	// 定义固定前缀字符串
	const wchar_t* prefix = L"[调试信息] ";
	wchar_t szbufFormat[0x1000];
	wchar_t szbufFormat_Game[0x1100] = L"";

	// 将固定前缀拼接到 szbufFormat_Game
	wcscat_s(szbufFormat_Game, prefix);

	// 处理可变参数
	va_list argList;
	va_start(argList, pszFormat); // 参数列表初始化
	vswprintf_s(szbufFormat, pszFormat, argList); // 格式化宽字符字符串
	wcscat_s(szbufFormat_Game, szbufFormat); // 拼接结果
	OutputDebugStringW(szbufFormat_Game); // 输出 UTF-16 调试信息
	va_end(argList);
#endif
}
// 定义一个函数来计算三维空间中两点之间的距离
float calculateDistance(DWORD* myptr, DWORD* objptr) {
	Call_输出调试信息("1:%p ,,,2:%p ", myptr, objptr);
	// 计算坐标差值
	float dx = SafeReadFloat(myptr++) - SafeReadFloat(objptr++);
	float dz = SafeReadFloat(myptr++) - SafeReadFloat(objptr++);
	float dy = SafeReadFloat(myptr) - SafeReadFloat(objptr);
	// 计算距离
	float distance = sqrtf(powf(dx, 2.0f) + powf(dy, 2.0f) + powf(dz, 2.0f));
	Call_输出调试信息("%f", distance);
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

	// 将资源数据转换为CString
	CStringA text((const char*)data, size);
	return text;
}

CStringW UTF8ToUnicode(const char* utf8Str)
{
	if (!utf8Str || *utf8Str == '\0')
	{
		return CStringW();
	}

	// 计算需要的宽字符数
	int wideCharCount = MultiByteToWideChar(CP_UTF8, 0, utf8Str, -1, nullptr, 0);
	if (wideCharCount == 0)
	{
		return CStringW();
	}

	// 分配缓冲区
	CStringW unicodeStr;
	wchar_t* buffer = unicodeStr.GetBuffer(wideCharCount);

	// 转换 UTF-8 到 UTF-16
	MultiByteToWideChar(CP_UTF8, 0, utf8Str, -1, buffer, wideCharCount);

	// 释放缓冲区
	unicodeStr.ReleaseBuffer();

	return unicodeStr;
}

// 不能用%P
void PrintRegisters( CONTEXT* ctx) {
	Call_输出调试信息("异常发生时寄存器状态: RCX=0x%p , RDX=0x%p , R8=0x%p , R9=0x%p", ctx->Rcx, ctx->Rdx, ctx->R8, ctx->R9);
	Call_输出调试信息("异常发生时寄存器状态: RDI=0x%p , RAX=0x%p , RBX=0x%p", ctx->Rdi, ctx->Rax, ctx->Rbx);
	Call_输出调试信息("异常发生时寄存器状态: RSP=0x%p , RBP=0x%p", ctx->Rsp, ctx->Rbp);
}


void call_asm(std::function<void()>&& func) {
    CONTEXT ctx;

    __try
    {
        // Move the function object to a local variable

        // Call the function
		std::invoke(func);
  
        Call_输出调试信息("__try安全调用_call");
    }
    __except (
        ctx = *(GetExceptionInformation())->ContextRecord,
        PrintRegisters(&ctx),
        EXCEPTION_EXECUTE_HANDLER
        ) {
        DWORD code;
        code = GetExceptionCode();
        Call_输出调试信息("异常代码: 0x%X", code);
    
        return;
    }
}

void seh_translator(unsigned int code, _EXCEPTION_POINTERS* ep) {
	if (ep && ep->ContextRecord) {
		// 将 SEH 异常信息和上下文一起抛出
		throw SEHException(
			code,
			"SEH异常",
			*ep->ContextRecord  // 复制上下文
		);
	}
	else {
		throw SEHException(code, "SEH异常（无上下文）", CONTEXT{});
	}
}

LONG WINAPI MyVectoredHandler(EXCEPTION_POINTERS* ExceptionInfo) {
	//std::cout << "捕获到异常: " << std::hex << ExceptionInfo->ExceptionRecord->ExceptionCode << std::endl;
	Call_输出调试信息("捕获到异常: 0x%X", ExceptionInfo->ExceptionRecord->ExceptionCode);
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
		Call_输出调试信息("SEH异常代码: 0x%X, 消息: %s", e.getCode(), e.what());
		PrintRegisters(e.getContext());
	}*/
}
