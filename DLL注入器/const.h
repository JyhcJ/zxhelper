
#pragma once


//����	WM_USER	WM_APP
//��Χ	0x0400�C0x7FFF	0x8000�C0xBFFF
//��;	�����������ڲ�	Ӧ�ó���ȫ��
//��ͻ����	�ϸߣ���ͬ�ؼ������ظ���	�ϵ�
//����̼�����	���Ƽ�	���ã��������� RegisterWindowMessage��
//���ͳ���	MFC �ؼ��ڲ���Ϣ	Ӧ����ģ��ͨ��

#define WM_MY_CROSS_PROCESS_MSG (WM_APP + 1)
#define WM_MY_CROSS_PROCESS_MSG_QUIT (1) //�ر�ע�����
const wchar_t* TARGET_CLASSNAME = L"ZElementClient Window";

// ����2��ע��ȫ��Ψһ��Ϣ���Ƽ�����̣��� ���ͷ� �� ���շ� ʹ����ͬ����Ϣ ID��
//UINT WM_MY_CROSS_PROCESS_MSG = ::RegisterWindowMessage(_T("MyCrossProcessMessage"));
//(4) ���շ�������Ϣ��MFC ���ڣ�
//��Ŀ�괰�ڵ� ��Ϣӳ�� �� ��Ϣ������ �д���
//
//�� ������Ϣ��������ͷ�ļ���
//cpp
//����
//// ��Ŀ�괰�����ͷ�ļ���
//afx_msg LRESULT OnMyCrossProcessMsg(WPARAM wParam, LPARAM lParam);
//�� �����Ϣӳ�䣨.cpp �ļ���
//cpp
//����
//BEGIN_MESSAGE_MAP(CMyTargetWnd, CWnd)
//    ON_REGISTERED_MESSAGE(WM_MY_CROSS_PROCESS_MSG, OnMyCrossProcessMsg)
//END_MESSAGE_MAP()
//�� ʵ����Ϣ����
//cpp
//����
//LRESULT CMyTargetWnd::OnMyCrossProcessMsg(WPARAM wParam, LPARAM lParam)
//{
//    int nReceivedData = (int)wParam;
//    AfxMessageBox(_T("�յ��������Ϣ��"));
//
//    // ������������� DestroyWindow() ����������
//    // this->DestroyWindow();
//    return 0;
//}