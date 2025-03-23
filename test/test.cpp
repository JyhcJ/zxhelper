// test.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

//#include <stdio.h>
#include <iostream>

#include <Windows.h>
#include <windnsdef.h>
#include <atlstr.h>

using namespace std;

int main()
{
	// 定义 QWORD 类型的变量 q，并赋值为 0x301
	uint64_t q = 0x301; // QWORD 通常是 64 位无符号整数

	// 将 q 赋值给 size_t 类型的变量 s
	size_t s = q; // size_t 是无符号整数类型，通常与平台相关（32 位或 64 位）

	// 输出结果
	std::cout << "!!!" << q  << std::endl;
	std::cout << "QWORD q = 0x" << std::hex << q << std::dec << " (十六进制)" << std::endl;
	std::cout << "size_t s = " << s << " (十进制)" << std::endl;
	for (size_t i = 0; i < q; i++)
	{
		std::cout << "!!!" << i << std::endl;
	}
	//cout << hex <<q << endl;
	//5148 58F0 593A 4EBA 793C 76D2 0000 先声夺人礼盒
	//4E5D 9704 4E91 5929 5916 002A 0000 九霄云外天 *
	//wchar_t* wcharPtr = (wchar_t*)str16;

	//printf("1	%p\n", wcharPtr);
	//wcharPtr = wcharPtr + 1;
	//printf("2	%p\n", &str16);
	//printf("3	%p\n", &wcharPtr);
	//printf("3	%p\n", str16);
	//printf("3	%p\n", wcharPtr);
	//cout << wcharPtr[1] << std::endl;
	//cout << wcharPtr[2] << std::endl;
	//std::wcout.imbue(std::locale("")); // 设置本地化以支持宽字符输出
	/*for (int i = 0; ; i++)
	{
		if (wcharPtr == '\0')
		{
			
			break;
		}
		(*wcharPtr)++;
		wcout << wcharPtr;
		
	}*/

	//cout << &str16 << std::endl;
	//cout << &wcharPtr << std::endl;
	//cout << hex << wcharPtr << std::endl;
	//printf("%S", wchar)

	//std::wprintf(L"UTF-16 string: %ls\n", reinterpret_cast<wchar_t*>(data));

	return 0;

}

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门使用技巧: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
