#include "stdafx.h"
#include "DebugSetting.h"


DebugSetting::DebugSetting()
{
    TCHAR szPath[MAX_PATH] = { 0 };
    GetModuleFileName(NULL, szPath, 511);
    tstring module_path = szPath;
    m_strFilePath = module_path.substr(0, module_path.rfind(_T("\\")) + 1);
    m_strFilePath.append(_T("debug.ini"));
}

DebugSetting::~DebugSetting()
{

}

DebugSetting& DebugSetting::GetInstance()
{
    static DebugSetting sInstance;
    return sInstance;
}

int DebugSetting::Read(LPCTSTR lpAppName, LPCTSTR lpKeyName, int nDefault)
{
    return ::GetPrivateProfileInt(lpAppName, lpKeyName, nDefault, m_strFilePath.c_str());
}

bool DebugSetting::Read(LPCTSTR lpAppName, LPCTSTR lpKeyName, bool bDefault)
{
    return ::GetPrivateProfileInt(lpAppName, lpKeyName, bDefault, m_strFilePath.c_str()) != 0 ? true : false;
}

tstring DebugSetting::Read(LPCTSTR lpAppName, LPCTSTR lpKeyName, LPCTSTR lpDefault)
{
    TCHAR buffer[MAX_PATH] = { 0 };
    ::GetPrivateProfileString(lpAppName, lpKeyName, lpDefault, buffer, ARRAYSIZE(buffer), m_strFilePath.c_str());

    return buffer;
}

tstring DebugSetting::GetCloudRole()
{
    return Read(_T("common"), _T("cloud_role"), _T(""));
}

int DebugSetting::GetMixIntValue(LPCTSTR lpKeyName, int nDefault/* = 0*/)
{
    return Read(_T("mix"), lpKeyName, nDefault);
}

QHVC::INTERACT::RENDER_MODE DebugSetting::GetLocalRenderMode()
{
    int render_mode = Read(_T("common"), _T("local_render_mode"), 2);
    if (render_mode >= 1 && render_mode <= 3) {
        return (QHVC::INTERACT::RENDER_MODE)render_mode;
    }

    return QHVC::INTERACT::RENDER_MODE_FIT;
}

QHVC::INTERACT::RENDER_MODE DebugSetting::GetRemoteRenderMode()
{
    int render_mode = Read(_T("common"), _T("remote_render_mode"), 2);
    if (render_mode >= 1 && render_mode <= 3) {
        return (QHVC::INTERACT::RENDER_MODE)render_mode;
    }

    return QHVC::INTERACT::RENDER_MODE_FIT;
}

