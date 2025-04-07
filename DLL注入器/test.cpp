#include "pch.h"
#include <Windows.h>
#include <vector>
#include <iostream>
#include <cstring> // memcmp
#include <Psapi.h> // GetModuleInformation

#pragma comment(lib, "Psapi.lib") // 链接 Psapi.lib

// 将通配符 "?" 替换为 0x00（可匹配任意字节）
std::vector<BYTE> ParsePattern(const char* pattern) {
    std::vector<BYTE> bytes;
    for (const char* p = pattern; *p; ++p) {
        if (*p == '?') {
            bytes.push_back(0x00); // 通配符
            if (*(p + 1) == '?') ++p; // 跳过第二个 '?'
        }
        else if (isxdigit(*p)) {
            BYTE byte = strtoul(p, nullptr, 16);
            bytes.push_back(byte);
            ++p; // 跳过第二个十六进制字符
        }
    }
    return bytes;
}

// 使用 SIMD (SSE4.1) 加速模式匹配（比 memcmp 更快）
bool MemoryMatchSSE(const BYTE* data, const BYTE* pattern, size_t patternSize) {
    size_t i = 0;
    for (; i + 16 <= patternSize; i += 16) {
        __m128i dataChunk = _mm_loadu_si128((__m128i*)(data + i));
        __m128i patternChunk = _mm_loadu_si128((__m128i*)(pattern + i));
        __m128i mask = _mm_cmpeq_epi8(dataChunk, patternChunk);
        uint16_t maskBits = _mm_movemask_epi8(mask);
        if (maskBits != 0xFFFF) return false;
    }
    // 剩余部分逐字节比较
    for (; i < patternSize; ++i) {
        if (pattern[i] != 0x00 && data[i] != pattern[i]) {
            return false;
        }
    }
    return true;
}

// 在 elementclient64.exe 模块内搜索字节模式（直接指针访问 + SIMD 加速）
uintptr_t FindPatternInElementClient(HANDLE hProcess , const char* patternStr) {
    std::vector<BYTE> pattern = ParsePattern(patternStr);

    // 获取 elementclient64.exe 模块基址和大小
    HMODULE hModule = GetModuleHandleA("elementclient64.exe");
    if (!hModule) {
        std::cerr << "Failed to find elementclient64.exe module!" << std::endl;
        return 0;
    }

    MODULEINFO modInfo;
    if (!GetModuleInformation(hProcess, hModule, &modInfo, sizeof(modInfo))) {
        std::cerr << "Failed to get module info!" << std::endl;
        return 0;
    }

    // 直接访问模块内存
    BYTE* scanStart = (BYTE*)hModule;
    BYTE* scanEnd = scanStart + modInfo.SizeOfImage;

    // 使用 SIMD 加速搜索
    for (BYTE* p = scanStart; p < scanEnd - pattern.size(); ++p) {
        if (MemoryMatchSSE(p, pattern.data(), pattern.size())) {
            return (uintptr_t)p;
        }
    }

    return 0; // 未找到
}

// 解析 RIP-relative 偏移量（直接指针访问）
uintptr_t ResolveRipRelative(uintptr_t instructionAddr) {
    // 读取 "48 8B 0D AA BB CC DD" 的后 4 字节（偏移量）
    int32_t offset = *(int32_t*)(instructionAddr + 3);

    // 下一条指令地址 = 当前指令地址 + 7（因为 "48 8B 0D AA BB CC DD" 占 7 字节）
    uintptr_t nextInstructionAddr = instructionAddr + 7;

    // 目标地址 = 下一条指令地址 + 偏移量
    return nextInstructionAddr + offset;
}

int main111(HANDLE hProcess) {
    // 示例：搜索 "66 89 03 44 88 73 ?? ?? 66 89 6B ?? ?? 89 73 ?? ?? ..."
    const char* pattern = "66 89 03 44 88 73 ?? ?? 66 89 6B ?? ?? 89 73 ?? ?? 44 8D 40 ?? ?? 40 88 7B ?? ?? 48 8B 0D ?? ?? ?? ?? ?? ?? ?? ?? 48 8B 49 ?? ??";

    // 在 elementclient64.exe 模块内搜索
    uintptr_t foundAddr = FindPatternInElementClient(hProcess,pattern);
    if (!foundAddr) {
        std::cerr << "Pattern not found in elementclient64.exe!" << std::endl;
        return 1;
    }

    std::cout << "Pattern found at: 0x" << std::hex << foundAddr << std::endl;

    // 找到 "48 8B 0D" 的位置（假设它在模式匹配的某个偏移处）
    uintptr_t movInstructionAddr = foundAddr + 0x20; // 假设 "48 8B 0D" 在匹配后的 0x20 字节处（需调整）

    // 解析 RIP-relative 地址
    uintptr_t targetAddr = ResolveRipRelative(movInstructionAddr);
    std::cout << "Target address: 0x" << std::hex << targetAddr << std::endl;

    // 直接读取目标地址的值
    uintptr_t value = *(uintptr_t*)targetAddr;
    std::cout << "Value at target address: 0x" << std::hex << value << std::endl;

    return 0;
}