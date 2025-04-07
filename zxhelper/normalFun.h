#pragma once
#include <eh.h> // _set_se_translator
#include <functional>
#include <iostream>
#include <vector>
#include <Windows.h>
QWORD SafeReadQWORD(QWORD* ptr);

float SafeReadFloat(float* ptr);

QWORD SafeReadQWORD_Arry_NotLast(std::vector<QWORD> arry);

DWORD SafeReadDWORD(DWORD* ptr);

//QWORD ReadChain(QWORD base, const std::vector<QWORD> offsets);

void ReadUtf8FileAndSplitByNewline(CStringArray& lines);

int CompareFirstColumn(const void* a, const void* b);

int ExtractNumber(const CString& str);

bool IsValidPointer(void* ptr, size_t size);


// 自定义中%s 和 %S一样的
void Call_输出调试信息(char* pszFormat, ...);

void Call_输出调试信息W(const wchar_t* pszFormat, ...);

float calculateDistance(DWORD* myptr, DWORD* objptr);

CStringA LoadTextFromResource(int resourceID);

CStringW UTF8ToUnicode(const char* utf8Str);

void PrintRegisters(CONTEXT* ctx);

template <typename Func, typename... Args>
void _callPlus(Func&& func, Args&&... args) {
	//HANDLE hHandler = AddVectoredExceptionHandler(1, MyVectoredHandler);
	//func(args...);
	//RemoveVectoredExceptionHandler(hHandler);

	CONTEXT ctx;
	__try
	{
	/*	std::function<void()>&& func1 = std::move(func);
		std::invoke(func1);*/
		//executeFunction(std::forward<Func>(func));
		Call_输出调试信息("__try安全调用");
		//std::invoke(func);
	}
	__except (
		ctx = *(GetExceptionInformation())->ContextRecord,
		PrintRegisters(&ctx),
		EXCEPTION_EXECUTE_HANDLER
		) {
		DWORD code = GetExceptionCode();
		Call_输出调试信息("异常代码: 0x%X", code);
		return;
	}
}

// 将SEH异常转换为C++异常
class SEHException : public std::runtime_error {
public:

	SEHException(DWORD code, const char* msg, const CONTEXT& ctx)
		: std::runtime_error(msg), m_code(code), m_context(ctx) {}
	DWORD getCode() const { return m_code; }
	const CONTEXT& getContext() const { return m_context; }  // 获取上下文副本
private:
	DWORD m_code;
	CONTEXT m_context;  // 保存上下文副本
};

void call_asm(std::function<void()>&& func);
void executeFunction(std::function<void()>&& func);
void seh_translator(unsigned int code, _EXCEPTION_POINTERS* ep);

LONG __stdcall MyVectoredHandler(EXCEPTION_POINTERS* ExceptionInfo);

void safeCall(std::function<void(QWORD*)> fp, QWORD* params);

uintptr_t FindPatternInElementClient(const char* patternStr, BYTE* scanStart);

QWORD calAddress(char* featureCode, char* cruxCode, bool isAfter, int repeats, bool isOffet);
