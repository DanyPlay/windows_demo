#include "stdafx.h"
#include "Log.h"

#define LOG_BUFFER_LEN      1024

CLog::CLog()
    : m_bEnable(false)
    , m_bShowFuncInfo(false)
    , m_nLevel(LOG_LEVEL_DEBUG)
{

}

CLog::~CLog()
{

}

CLog& CLog::GetInstance()
{
    static CLog sInstance;
    return sInstance;
}

void CLog::LogA(const char* file, const char* func, int line, E_LOG_LEVEL level, const char* tag, const char* format, ...)
{
    if (!m_bEnable || level < m_nLevel || !tag || !format){
        return;
    }

    char msg[LOG_BUFFER_LEN] = { 0 };
    va_list arglist;
    va_start(arglist, format);
    _vsnprintf_s(msg, LOG_BUFFER_LEN, _TRUNCATE, format, arglist);
    va_end(arglist);

    char c = 0;
    switch (level)
    {
        case LOG_LEVEL_TRACE:
            c = 'T';
            break;
        case LOG_LEVEL_DEBUG:
            c = 'D';
            break;
        case LOG_LEVEL_INFO:
            c = 'I';
            break;
        case LOG_LEVEL_WARN:
            c = 'W';
            break;
        case LOG_LEVEL_ERROR:
            c = 'E';
            break;
        default:
            break;
    }

    char time[32] = { 0 };
    GenerateTimeA(time, ARRAYSIZE(time));

    DWORD thread = GetCurrentThreadId();

    char buffer[1024] = { 0 };
    if (m_bShowFuncInfo)
        _snprintf_s(buffer, ARRAYSIZE(buffer), _TRUNCATE, "%s %d %c[%s] %s @%s (%s: %d)", time, thread, c, tag, msg, func, file, line);
    else
        _snprintf_s(buffer, ARRAYSIZE(buffer), _TRUNCATE, "%s %d %c[%s] %s", time, thread, c, tag, msg);
    OutputDebugStringA(buffer);
    OutputDebugStringA("\r\n");
}

void CLog::LogW(const wchar_t* file, const wchar_t* func, int line, E_LOG_LEVEL level, const wchar_t* tag, const wchar_t* format, ...)
{
    if (!m_bEnable || level < m_nLevel || !tag || !format){
        return;
    }

    wchar_t msg[LOG_BUFFER_LEN] = { 0 };
    va_list arglist;
    va_start(arglist, format);
    _vsnwprintf_s(msg, LOG_BUFFER_LEN, _TRUNCATE, format, arglist);
    va_end(arglist);

    wchar_t c = 0;
    switch (level)
    {
        case LOG_LEVEL_TRACE:
            c = L'T';
            break;
        case LOG_LEVEL_DEBUG:
            c = L'D';
            break;
        case LOG_LEVEL_INFO:
            c = L'I';
            break;
        case LOG_LEVEL_WARN:
            c = L'W';
            break;
        case LOG_LEVEL_ERROR:
            c = L'E';
            break;
        default:
            break;
    }

    wchar_t time[32] = { 0 };
    GenerateTimeW(time, ARRAYSIZE(time));

    DWORD thread = GetCurrentThreadId();

    wchar_t buffer[1024] = { 0 };
    if (m_bShowFuncInfo)
        _snwprintf_s(buffer, ARRAYSIZE(buffer), _TRUNCATE, L"%s %d %c[%s] %s @%s (%s: %d)", time, thread, c, tag, msg, func, file, line);
    else
        _snwprintf_s(buffer, ARRAYSIZE(buffer), _TRUNCATE, L"%s %d %c[%s] %s", time, thread, c, tag, msg);
    OutputDebugStringW(buffer);
    OutputDebugStringW(L"\r\n");
}

void CLog::GenerateTimeA(char* buffer, int length)
{
    if (!buffer || length <= 0)
        return;

    SYSTEMTIME time;
    GetLocalTime(&time);
    _snprintf_s(buffer, length, _TRUNCATE, "%02d-%02d %02d:%02d:%02d.%03d",
        time.wMonth,
        time.wDay,
        time.wHour,
        time.wMinute,
        time.wSecond,
        time.wMilliseconds);
}

void CLog::GenerateTimeW(wchar_t* buffer, int length)
{
    if (!buffer || length <= 0)
        return;

    SYSTEMTIME time;
    GetLocalTime(&time);
    _snwprintf_s(buffer, length, _TRUNCATE, L"%02d-%02d %02d:%02d:%02d.%03d",
        time.wMonth,
        time.wDay,
        time.wHour,
        time.wMinute,
        time.wSecond,
        time.wMilliseconds);
}
