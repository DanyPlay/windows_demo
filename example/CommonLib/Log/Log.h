#ifndef _LOG_H_20180509_
#define _LOG_H_20180509_

#pragma once

#include "../Util/Util.h"

enum E_LOG_LEVEL
{
    LOG_LEVEL_TRACE = 0,
    LOG_LEVEL_DEBUG,
    LOG_LEVEL_INFO,
    LOG_LEVEL_WARN,
    LOG_LEVEL_ERROR,
};

class CLog
{
protected:
    CLog();
    ~CLog();

public:
    static CLog& GetInstance();

    void SetEnable(bool enable) { this->m_bEnable = enable; }
    bool GetEnable() { return m_bEnable; }

    void SetShowFuncInfo(bool show) { this->m_bShowFuncInfo = show; }
    bool GetShowFuncInfo() { return m_bShowFuncInfo; }

    void SetLogLevel(E_LOG_LEVEL level) { this->m_nLevel = level; }
    E_LOG_LEVEL GetLogLevel() { return m_nLevel; }

public:
    void LogA(const char* file, const char* func, int line, E_LOG_LEVEL level, const char* tag, const char* format, ...);
    void LogW(const wchar_t* file, const wchar_t* func, int line, E_LOG_LEVEL level, const wchar_t* tag, const wchar_t* format, ...);

private:
    void GenerateTimeA(char* buffer, int length);
    void GenerateTimeW(wchar_t* buffer, int length);

private:
    bool m_bEnable;
    bool m_bShowFuncInfo;
    E_LOG_LEVEL m_nLevel;
};

#define _LOGA(level, tag, format, ...) CLog::GetInstance().LogA(__FILE__, __FUNCTION__, __LINE__, level, tag, format, __VA_ARGS__)
#define _LOGW(level, tag, format, ...) CLog::GetInstance().LogW(Util::Encode::ASCIIToWide(__FILE__).c_str(), Util::Encode::ASCIIToWide(__FUNCTION__).c_str(), __LINE__, level, tag, format, __VA_ARGS__)

#define LOGTA(tag, format, ...) _LOGA(LOG_LEVEL_TRACE, tag, format, __VA_ARGS__)
#define LOGTW(tag, format, ...) _LOGW(LOG_LEVEL_TRACE, tag, format, __VA_ARGS__)

#define LOGDA(tag, format, ...) _LOGA(LOG_LEVEL_DEBUG, tag, format, __VA_ARGS__)
#define LOGDW(tag, format, ...) _LOGW(LOG_LEVEL_DEBUG, tag, format, __VA_ARGS__)

#define LOGIA(tag, format, ...) _LOGA(LOG_LEVEL_INFO, tag, format, __VA_ARGS__)
#define LOGIW(tag, format, ...) _LOGW(LOG_LEVEL_INFO, tag, format, __VA_ARGS__)

#define LOGWA(tag, format, ...) _LOGA(LOG_LEVEL_WARN, tag, format, __VA_ARGS__)
#define LOGWW(tag, format, ...) _LOGW(LOG_LEVEL_WARN, tag, format, __VA_ARGS__)

#define LOGEA(tag, format, ...) _LOGA(LOG_LEVEL_ERROR, tag, format, __VA_ARGS__)
#define LOGEW(tag, format, ...) _LOGW(LOG_LEVEL_ERROR, tag, format, __VA_ARGS__)

#ifdef UNICODE
#define LOGT LOGTW
#define LOGD LOGDW
#define LOGI LOGIW
#define LOGW LOGWW
#define LOGE LOGEW
#else

#define LOGT LOGTA
#define LOGD LOGDA
#define LOGI LOGIA
#define LOGW LOGWA
#define LOGE LOGEA
#endif

#endif
