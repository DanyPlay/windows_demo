#ifndef _INTERACT_DEBUG_SETTING_H_
#define _INTERACT_DEBUG_SETTING_H_

#pragma once


class DebugSetting
{
protected:
    DebugSetting();
    ~DebugSetting();

public:
    static DebugSetting& GetInstance();

private:
    int Read(LPCTSTR lpAppName, LPCTSTR lpKeyName, int nDefault);
    bool Read(LPCTSTR lpAppName, LPCTSTR lpKeyName, bool bDefault);
    tstring Read(LPCTSTR lpAppName, LPCTSTR lpKeyName, LPCTSTR lpDefault);

public:
    int GetMixIntValue(LPCTSTR lpKeyName, int nDefault = 0);
    tstring GetCloudRole();

    QHVC::INTERACT::RENDER_MODE GetLocalRenderMode();
    QHVC::INTERACT::RENDER_MODE GetRemoteRenderMode();
private:
    tstring m_strFilePath;
};

#endif
