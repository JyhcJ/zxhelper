#include "pch.h"
#include "Log.h"
CLog::CLog() : m_bInitialized(FALSE) {
}

CLog::~CLog() {
    Close();
}

// ��ʼ����־�ļ�
BOOL CLog::Init(LPCTSTR lpszFilePath) {
    if (m_bInitialized) {
        return TRUE; // �Ѿ���ʼ��
    }

    m_FilePath = lpszFilePath;

    // ����־�ļ���׷��ģʽ��
    if (!m_LogFile.Open(m_FilePath, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite | CFile::shareDenyNone)) {
        return FALSE;
    }

    // �ƶ����ļ�ĩβ
    m_LogFile.SeekToEnd();

    m_bInitialized = TRUE;
    return TRUE;
}

// д����־
void CLog::Write(LPCTSTR lpszFormat, ...) {
    if (!m_bInitialized) {
        return;
    }

    CString strLog;
    va_list args;
    va_start(args, lpszFormat);
    strLog.FormatV(lpszFormat, args);
    va_end(args);

    // ���ʱ���
    CString strTime;
    CTime time = CTime::GetCurrentTime();
    strTime = time.Format(_T("[%Y-%m-%d %H:%M:%S] "));

    // д����־
    CString strFinalLog = strTime + strLog + _T("\n");
    m_LogFile.WriteString(strFinalLog);
    m_LogFile.Flush(); // ����д���ļ�
}

// �ر���־�ļ�
void CLog::Close() {
    if (m_bInitialized) {
        m_LogFile.Close();
        m_bInitialized = FALSE;
    }
}
