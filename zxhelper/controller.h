#pragma once
#include "T_obj.h"


void relex();

void useObj(QWORD index, QWORD objId);


void hanHua(CString ptr);

std::vector<T������Ʒ����> traversePackage();

void seekCityRoll();

T�������� getPersonAtt();

wchar_t* getName();

QWORD getAtt_TBaseAddress();

QWORD getAtt_qixue();

QWORD getAtt_zhenqi();

QWORD getAtt_yuanli();

QWORD getAtt_wushi();

QWORD getAtt_jianmian();

bool isString(QWORD* ptr, size_t maxLength);
