#include "pch.h"
#include <Windows.h>
#include <vector>
#include <iostream>
#include <cstring> // memcmp
#include <Psapi.h> // GetModuleInformation

#pragma comment(lib, "Psapi.lib") // ���� Psapi.lib

// ��ͨ��� "?" �滻Ϊ 0x00����ƥ�������ֽڣ�
std::vector<BYTE> ParsePattern(const char* pattern) {
    std::vector<BYTE> bytes;
    for (const char* p = pattern; *p; ++p) {
        if (*p == '?') {
            bytes.push_back(0x00); // ͨ���
            if (*(p + 1) == '?') ++p; // �����ڶ��� '?'
        }
        else if (isxdigit(*p)) {
            BYTE byte = strtoul(p, nullptr, 16);
            bytes.push_back(byte);
            ++p; // �����ڶ���ʮ�������ַ�
        }
    }
    return bytes;
}

// ʹ�� SIMD (SSE4.1) ����ģʽƥ�䣨�� memcmp ���죩
bool MemoryMatchSSE(const BYTE* data, const BYTE* pattern, size_t patternSize) {
    size_t i = 0;
    for (; i + 16 <= patternSize; i += 16) {
        __m128i dataChunk = _mm_loadu_si128((__m128i*)(data + i));
        __m128i patternChunk = _mm_loadu_si128((__m128i*)(pattern + i));
        __m128i mask = _mm_cmpeq_epi8(dataChunk, patternChunk);
        uint16_t maskBits = _mm_movemask_epi8(mask);
        if (maskBits != 0xFFFF) return false;
    }
    // ʣ�ಿ�����ֽڱȽ�
    for (; i < patternSize; ++i) {
        if (pattern[i] != 0x00 && data[i] != pattern[i]) {
            return false;
        }
    }
    return true;
}

// �� elementclient64.exe ģ���������ֽ�ģʽ��ֱ��ָ����� + SIMD ���٣�
uintptr_t FindPatternInElementClient(HANDLE hProcess , const char* patternStr) {
    std::vector<BYTE> pattern = ParsePattern(patternStr);

    // ��ȡ elementclient64.exe ģ���ַ�ʹ�С
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

    // ֱ�ӷ���ģ���ڴ�
    BYTE* scanStart = (BYTE*)hModule;
    BYTE* scanEnd = scanStart + modInfo.SizeOfImage;

    // ʹ�� SIMD ��������
    for (BYTE* p = scanStart; p < scanEnd - pattern.size(); ++p) {
        if (MemoryMatchSSE(p, pattern.data(), pattern.size())) {
            return (uintptr_t)p;
        }
    }

    return 0; // δ�ҵ�
}

// ���� RIP-relative ƫ������ֱ��ָ����ʣ�
uintptr_t ResolveRipRelative(uintptr_t instructionAddr) {
    // ��ȡ "48 8B 0D AA BB CC DD" �ĺ� 4 �ֽڣ�ƫ������
    int32_t offset = *(int32_t*)(instructionAddr + 3);

    // ��һ��ָ���ַ = ��ǰָ���ַ + 7����Ϊ "48 8B 0D AA BB CC DD" ռ 7 �ֽڣ�
    uintptr_t nextInstructionAddr = instructionAddr + 7;

    // Ŀ���ַ = ��һ��ָ���ַ + ƫ����
    return nextInstructionAddr + offset;
}

int main111(HANDLE hProcess) {
    // ʾ�������� "66 89 03 44 88 73 ?? ?? 66 89 6B ?? ?? 89 73 ?? ?? ..."
    const char* pattern = "66 89 03 44 88 73 ?? ?? 66 89 6B ?? ?? 89 73 ?? ?? 44 8D 40 ?? ?? 40 88 7B ?? ?? 48 8B 0D ?? ?? ?? ?? ?? ?? ?? ?? 48 8B 49 ?? ??";

    // �� elementclient64.exe ģ��������
    uintptr_t foundAddr = FindPatternInElementClient(hProcess,pattern);
    if (!foundAddr) {
        std::cerr << "Pattern not found in elementclient64.exe!" << std::endl;
        return 1;
    }

    std::cout << "Pattern found at: 0x" << std::hex << foundAddr << std::endl;

    // �ҵ� "48 8B 0D" ��λ�ã���������ģʽƥ���ĳ��ƫ�ƴ���
    uintptr_t movInstructionAddr = foundAddr + 0x20; // ���� "48 8B 0D" ��ƥ���� 0x20 �ֽڴ����������

    // ���� RIP-relative ��ַ
    uintptr_t targetAddr = ResolveRipRelative(movInstructionAddr);
    std::cout << "Target address: 0x" << std::hex << targetAddr << std::endl;

    // ֱ�Ӷ�ȡĿ���ַ��ֵ
    uintptr_t value = *(uintptr_t*)targetAddr;
    std::cout << "Value at target address: 0x" << std::hex << value << std::endl;

    return 0;
}