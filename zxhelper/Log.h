#pragma once
class CLog {

public:
    CLog();

    ~CLog();

    // 初始化日志文件
    BOOL Init(LPCTSTR lpszFilePath);

    // 写入日志
    void Write(LPCTSTR lpszFormat, ...);

    // 关闭日志文件
    void Close();

private:
    CStdioFile m_LogFile; // MFC 文件类，用于操作日志文件
    CString m_FilePath;   // 日志文件路径
    BOOL m_bInitialized;  // 是否已初始化
};