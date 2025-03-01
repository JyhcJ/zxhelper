#include "pch.h"
#include "Log.h"
CLog::CLog() : m_bInitialized(FALSE) {
}

CLog::~CLog() {
    Close();
}

// 初始化日志文件
BOOL CLog::Init(LPCTSTR lpszFilePath) {
    if (m_bInitialized) {
        return TRUE; // 已经初始化
    }

    m_FilePath = lpszFilePath;

    // 打开日志文件（追加模式）
    if (!m_LogFile.Open(m_FilePath, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite | CFile::shareDenyNone)) {
        return FALSE;
    }

    // 移动到文件末尾
    m_LogFile.SeekToEnd();

    m_bInitialized = TRUE;
    return TRUE;
}

// 写入日志
void CLog::Write(LPCTSTR lpszFormat, ...) {
    if (!m_bInitialized) {
        return;
    }

    CString strLog;
    va_list args;
    va_start(args, lpszFormat);
    strLog.FormatV(lpszFormat, args);
    va_end(args);

    // 添加时间戳
    CString strTime;
    CTime time = CTime::GetCurrentTime();
    strTime = time.Format(_T("[%Y-%m-%d %H:%M:%S] "));

    // 写入日志
    CString strFinalLog = strTime + strLog + _T("\n");
    m_LogFile.WriteString(strFinalLog);
    m_LogFile.Flush(); // 立即写入文件
}

// 关闭日志文件
void CLog::Close() {
    if (m_bInitialized) {
        m_LogFile.Close();
        m_bInitialized = FALSE;
    }
}
