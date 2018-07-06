#include "stdafx.h"
#include "InteractSetting.h"
#include "InteractConfig.h"


InteractSetting::InteractSetting()
{
    TCHAR szPath[MAX_PATH] = { 0 };
    ::SHGetSpecialFolderPath(NULL, szPath, CSIDL_APPDATA, TRUE);
    ::PathAddBackslash(szPath);
    ::PathAppend(szPath, STR2T(FILE_INTERACT_CONFIG).c_str());

    Util::File::ForceCreateDir(szPath);

    m_strFilePath = szPath;
}

InteractSetting::~InteractSetting()
{

}

InteractSetting& InteractSetting::GetInstance()
{
    static InteractSetting sInstance;
    return sInstance;
}

int InteractSetting::Read(LPCTSTR lpAppName, LPCTSTR lpKeyName, int nDefault)
{
    return ::GetPrivateProfileInt(lpAppName, lpKeyName, nDefault, m_strFilePath.c_str());
}

bool InteractSetting::Read(LPCTSTR lpAppName, LPCTSTR lpKeyName, bool bDefault)
{
    return ::GetPrivateProfileInt(lpAppName, lpKeyName, bDefault, m_strFilePath.c_str()) != 0 ? true : false;
}

tstring InteractSetting::Read(LPCTSTR lpAppName, LPCTSTR lpKeyName, LPCTSTR lpDefault)
{
    TCHAR buffer[MAX_PATH] = { 0 };
    ::GetPrivateProfileString(lpAppName, lpKeyName, lpDefault, buffer, ARRAYSIZE(buffer), m_strFilePath.c_str());

    return buffer;
}

void InteractSetting::Write(LPCTSTR lpAppName, LPCTSTR lpKeyName, int nValue)
{
    TCHAR buffer[65] = { 0 };
    _sntprintf_s(buffer, ARRAYSIZE(buffer), _TRUNCATE, _T("%d"), nValue);

    Write(lpAppName, lpKeyName, buffer);
}

void InteractSetting::Write(LPCTSTR lpAppName, LPCTSTR lpKeyName, bool bValue)
{
    Write(lpAppName, lpKeyName, bValue ? 1 : 0);
}

void InteractSetting::Write(LPCTSTR lpAppName, LPCTSTR lpKeyName, LPCTSTR lpValue)
{
    ::WritePrivateProfileString(lpAppName, lpKeyName, lpValue, m_strFilePath.c_str());
}

tstring InteractSetting::GetUserIdentityTag(InteractConstant::USER_IDENTITY enIdentity)
{
    switch (enIdentity)
    {
        case InteractConstant::USER_IDENTITY_ANCHOR:
            return _T("anchor");
        case InteractConstant::USER_IDENTITY_GUEST:
            return _T("guest");
        case InteractConstant::USER_IDENTITY_AUDIENCE:
            return _T("audience");
        default:
            assert(true && "not support identity");
            return _T("");
    }
}

int InteractSetting::GetProfileMode(InteractConstant::USER_IDENTITY enIdentity, int nDefault /*= QHVC::INTERACT::VIDEO_PROFILE::VIDEO_PROFILE_360P_9*/)
{
    return Read(GetUserIdentityTag(enIdentity).c_str(), _T("profile_mode"), nDefault);
}

void InteractSetting::SetProfileMode(InteractConstant::USER_IDENTITY enIdentity, int nProfileMode)
{
    Write(GetUserIdentityTag(enIdentity).c_str(), _T("profile_mode"), nProfileMode);
}

int InteractSetting::GetResolutionWidth(InteractConstant::USER_IDENTITY enIdentity, int nDefaultWidth /*= 640*/)
{
    return Read(GetUserIdentityTag(enIdentity).c_str(), _T("resolution_width"), nDefaultWidth);
}

void InteractSetting::SetResolutionWidth(InteractConstant::USER_IDENTITY enIdentity, int nResolutionWidth)
{
    Write(GetUserIdentityTag(enIdentity).c_str(), _T("resolution_width"), nResolutionWidth);
}

int InteractSetting::GetResolutionHeight(InteractConstant::USER_IDENTITY enIdentity, int nDefaultWidth /*= 640*/)
{
    return Read(GetUserIdentityTag(enIdentity).c_str(), _T("resolution_height"), nDefaultWidth);
}

void InteractSetting::SetResolutionHeight(InteractConstant::USER_IDENTITY enIdentity, int nResolutionWidth)
{
    Write(GetUserIdentityTag(enIdentity).c_str(), _T("resolution_height"), nResolutionWidth);
}

int InteractSetting::GetFrameRate(InteractConstant::USER_IDENTITY enIdentity, int nDefault/* = 15*/)
{
    return Read(GetUserIdentityTag(enIdentity).c_str(), _T("frame_rate"), nDefault);
}

void InteractSetting::SetFrameRate(InteractConstant::USER_IDENTITY enIdentity, int nFrameRate)
{
    Write(GetUserIdentityTag(enIdentity).c_str(), _T("frame_rate"), nFrameRate);
}

int InteractSetting::GetBitrate(InteractConstant::USER_IDENTITY enIdentity, int nDefault /*= 800*/)
{
    return Read(GetUserIdentityTag(enIdentity).c_str(), _T("bitrate"), nDefault);
}

void InteractSetting::SetBitrate(InteractConstant::USER_IDENTITY enIdentity, int nBitrate)
{
    Write(GetUserIdentityTag(enIdentity).c_str(), _T("bitrate"), nBitrate);
}

tstring InteractSetting::GetBid()
{
    return Read(_T("app_info"), _T("app_bid"), InteractConstant::AppConfig::APP_BID.c_str());
}

void InteractSetting::SetBid(const tstring& strBid)
{
    Write(_T("app_info"), _T("app_bid"), strBid.c_str());
}

tstring InteractSetting::GetCid()
{
    return Read(_T("app_info"), _T("app_cid"), InteractConstant::AppConfig::APP_CID.c_str());
}

void InteractSetting::SetCid(const tstring& strCid)
{
    Write(_T("app_info"), _T("app_cid"), strCid.c_str());
}

tstring InteractSetting::GetAppKey()
{
    return Read(_T("app_info"), _T("app_key"), InteractConstant::AppConfig::APP_KEY.c_str());
}
void InteractSetting::SetAppKey(const tstring& strAppKey)
{
    Write(_T("app_info"), _T("app_key"), strAppKey.c_str());
}

tstring InteractSetting::GetAppSecret()
{
    return Read(_T("app_info"), _T("app_secret"), InteractConstant::AppConfig::APP_SCREAT_KEY.c_str());
}
void InteractSetting::SetAppSecret(const tstring& strAppSecret)
{
    Write(_T("app_info"), _T("app_secret"), strAppSecret.c_str());
}
