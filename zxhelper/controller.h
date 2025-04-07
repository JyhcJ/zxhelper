#pragma once
#include "T_obj.h"

void relex();

void useObj(QWORD index, QWORD objId, bool isEncap);

void selectObj(DWORD id);

void teleportation(DWORD obj_location_ptr);

void hanHua(CString ptr);

std::vector<T包裹物品属性> traversePackage();

std::vector<T人物属性> traversePerson();

std::vector<T技能> traverseSkill();

T人物属性 getPersonAtt();

wchar_t* getName();

QWORD getAtt_TBaseAddress();

void updateAddress();

bool isString(QWORD* ptr, size_t maxLength);
