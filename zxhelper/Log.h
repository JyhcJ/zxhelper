#pragma once
class CLog {

public:
    CLog();

    ~CLog();

    // ��ʼ����־�ļ�
    BOOL Init(LPCTSTR lpszFilePath);

    // д����־
    void Write(LPCTSTR lpszFormat, ...);

    // �ر���־�ļ�
    void Close();

private:
    CStdioFile m_LogFile; // MFC �ļ��࣬���ڲ�����־�ļ�
    CString m_FilePath;   // ��־�ļ�·��
    BOOL m_bInitialized;  // �Ƿ��ѳ�ʼ��
};