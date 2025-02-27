#pragma once
class Call
{
};

void relex();

wchar_t* getName();

QWORD getAtt_TBaseAddress();

QWORD getAtt_qixue();

QWORD getAtt_zhenqi();

QWORD getAtt_yuanli();

QWORD getAtt_wushi();

QWORD getAtt_jianmian();

bool isString(const char* ptr, size_t maxLength);
