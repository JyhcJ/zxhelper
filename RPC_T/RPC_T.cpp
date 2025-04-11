#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>

std::string processRegisterLine(const std::string& line) {
    // 找到寄存器名称和值的分界点
    size_t colonPos = line.find(':');
    if (colonPos == std::string::npos) {
        return "";
    }

    // 提取寄存器名称
    std::string regName = line.substr(0, colonPos);
    // 去除寄存器名称前后的空格并转为小写
    regName.erase(0, regName.find_first_not_of(" \t"));
    regName.erase(regName.find_last_not_of(" \t") + 1);
    std::transform(regName.begin(), regName.end(), regName.begin(), ::tolower);

    // 如果是RSP行，返回空字符串（跳过不处理）
    if (regName == "rsp") {
        return "";
    }

    // 提取值部分（从冒号后到注释前）
    std::string valuePart = line.substr(colonPos + 1);
    // 去除值部分前后的空格
    valuePart.erase(0, valuePart.find_first_not_of(" \t"));

    // 查找注释开始位置（空格或制表符后的部分）
    size_t commentPos = valuePart.find_first_of(" \t");
    if (commentPos != std::string::npos) {
        valuePart = valuePart.substr(0, commentPos);
    }

    // 构建mov指令字符串，添加h后缀
    return "mov " + regName + ",0x" + valuePart ;
}

int main() {
    // x64复制全部寄存器 远程调试使用 以便确定必要参数
    std::vector<std::string> inputLines = {
        "RAX : 0000000000000000",
        "RBX : 000000000382BAA0     elementskill64.000000000382BAA0",
        "RCX : 0000000021600140",
        "RDX : 00000000000000EE     'î'",
        "RBP : 0000000004F40500",
        "RSP : 000000000014DD40",
        "RSI : 000000009250572C     \"[PowerToys.PowerLauncher;;2fb682cf-1807-4194-9b7c-7c0865806ce7]>:<>\"",
        "RDI : 0000000004F40500",
        "R8  : 0000000000000000",
        "R9  : 0000000000000000",
        "R10 : 000000000382BAA0     elementskill64.000000000382BAA0",
        "R11 : 000000007842BE9D     \":0:00010003:00000000:00000000>:<YdEditDetectIconWnd>\"",
        "R12 : 0000000000000043     'C'",
        "R13 : 0000000000430276",
        "R14 : 0000000000000000",
        "R15 : 0000000000000000"
    };

    // 输出开头指令
    std::cout << "alloc(test,1024)" << std::endl;
    std::cout << "test:" << std::endl;
    std::cout << "sub rsp,0x128" << std::endl;

    // 处理每一行并输出结果
    for (const auto& line : inputLines) {
        std::string processed = processRegisterLine(line);
        if (!processed.empty()) {
            std::cout << processed << std::endl;
        }
    }

    // 输出结尾指令
    std::cout << "add rsp,0x128" << std::endl;
    std::cout << "ret" << std::endl;
    std::cout << "createthread(test)" << std::endl;
    return 0;
}