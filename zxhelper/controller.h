#pragma once
#include "T_obj.h"

void relex();

void useObj(QWORD index, QWORD objId, bool isEncap);

void selectObj(DWORD id);

void teleportation(DWORD obj_location_ptr);

void hanHua(CString ptr);

std::vector<T������Ʒ����> traversePackage();

std::vector<T��������> traversePerson();

std::vector<T����> traverseSkill();

T�������� getPersonAtt();

wchar_t* getName();

QWORD getAtt_TBaseAddress();

void updateAddress();

bool isString(QWORD* ptr, size_t maxLength);
