#pragma once


#define WM_MY_CROSS_PROCESS_MSG (WM_APP + 1)
#define WM_MY_CROSS_PROCESS_MSG_QUIT (1) //关闭注入程序


const CString LOG_PATH = _T("C:\\Users\\54469\\Desktop\\zxhelper\\MyAppLog.txt"); // 日志文件路径

//因为CString是引用类型,所有不用static?
static const char* MOUDLE_ZX_NAME = "elementclient64.exe";
