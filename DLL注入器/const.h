
#pragma once


//特性	WM_USER	WM_APP
//范围	0x0400–0x7FFF	0x8000–0xBFFF
//用途	单个窗口类内部	应用程序全局
//冲突风险	较高（不同控件可能重复）	较低
//跨进程兼容性	不推荐	可用（但优先用 RegisterWindowMessage）
//典型场景	MFC 控件内部消息	应用内模块通信

#define WM_MY_CROSS_PROCESS_MSG (WM_APP + 1)
#define WM_MY_CROSS_PROCESS_MSG_QUIT (1) //关闭注入程序
const wchar_t* TARGET_CLASSNAME = L"ZElementClient Window";

// 方法2：注册全局唯一消息（推荐跨进程）在 发送方 和 接收方 使用相同的消息 ID：
//UINT WM_MY_CROSS_PROCESS_MSG = ::RegisterWindowMessage(_T("MyCrossProcessMessage"));
//(4) 接收方处理消息（MFC 窗口）
//在目标窗口的 消息映射 和 消息处理函数 中处理：
//
//① 声明消息处理函数（头文件）
//cpp
//复制
//// 在目标窗口类的头文件中
//afx_msg LRESULT OnMyCrossProcessMsg(WPARAM wParam, LPARAM lParam);
//② 添加消息映射（.cpp 文件）
//cpp
//复制
//BEGIN_MESSAGE_MAP(CMyTargetWnd, CWnd)
//    ON_REGISTERED_MESSAGE(WM_MY_CROSS_PROCESS_MSG, OnMyCrossProcessMsg)
//END_MESSAGE_MAP()
//③ 实现消息处理
//cpp
//复制
//LRESULT CMyTargetWnd::OnMyCrossProcessMsg(WPARAM wParam, LPARAM lParam)
//{
//    int nReceivedData = (int)wParam;
//    AfxMessageBox(_T("收到跨进程消息！"));
//
//    // 可以在这里调用 DestroyWindow() 或其他操作
//    // this->DestroyWindow();
//    return 0;
//}