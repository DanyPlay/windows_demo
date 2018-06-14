#include "stdafx.h"
#include "InteractGlobalManager.h"
#include "../CommonLib/Util/Util.h"
#include "../../CommonLib/md5/md5.h"



InteractGlobalManager::InteractGlobalManager()
{
    m_hMainWindow = NULL;
}


InteractGlobalManager::~InteractGlobalManager()
{
}

tstring InteractGlobalManager::GetBusinessId()
{
    return m_strBusinessId;
}

void InteractGlobalManager::SetBusinessId(const tstring& strBusinessId)
{
    m_strBusinessId = strBusinessId;
}

tstring InteractGlobalManager::GetChannelId()
{
    return m_strChannelId;
}

void InteractGlobalManager::SetChannelId(const tstring& strBusinessId)
{
    m_strChannelId = strBusinessId;
}

tstring InteractGlobalManager::CreateSessionId()
{
    std::string sessionId = Util::OS::GetTheFirstMac();
    sessionId.append(Util::Time::CurrentTimeString());

    std::string ret = CMD5Checksum::GetMD5((BYTE*)sessionId.c_str(), sessionId.size());

    return  ConvertString(ret);
}

tstring InteractGlobalManager::CreateStreamId(const tstring& strRoomId)
{
    TCHAR buffer[MAX_PATH] = { 0 };
    _sntprintf_s(buffer, ARRAYSIZE(buffer), _TRUNCATE, _T("%s_%s_%s_%s_%d"),
        GetBusinessId().c_str(), GetChannelId().c_str(), strRoomId.c_str(), GetInteractUserId().c_str(), GetTickCount());

    return buffer;
}

tstring InteractGlobalManager::CreatePushAddr(const tstring& strStreamId)
{
    TCHAR buffer[MAX_PATH] = { 0 };
    _sntprintf_s(buffer, ARRAYSIZE(buffer), _TRUNCATE, _T("rtmp://ps1.live.huajiao.com/live_huajiao_v2/%s"), strStreamId.c_str());

    return buffer;
}

tstring InteractGlobalManager::CreatePullAddr(const tstring& strStreamId)
{
    TCHAR buffer[MAX_PATH] = { 0 };
    _sntprintf_s(buffer, ARRAYSIZE(buffer), _TRUNCATE, _T("http://pl1.live.huajiao.com/live_huajiao_v2/%s.flv"), strStreamId.c_str());

    return buffer;
}

tstring InteractGlobalManager::CreateMergeRtmp(const tstring& strStreamId)
{
    TCHAR buffer[MAX_PATH] = { 0 };
    _sntprintf_s(buffer, ARRAYSIZE(buffer), _TRUNCATE, _T("rtmp://ps1.live.huajiao.com/live_huajiao_v2/%s_l"), strStreamId.c_str());

    return buffer;
}

tstring InteractGlobalManager::GetDeviceId()
{
    std::string deviceId = Util::OS::GetTheFirstMac();
    std::string ret = CMD5Checksum::GetMD5((BYTE*)deviceId.c_str(), deviceId.size());
    return ConvertString(ret);
}

tstring InteractGlobalManager::GetAppKey()
{
    return m_strAppKey;
}

void InteractGlobalManager::SetAppKey(const tstring& strAppKey)
{
    m_strAppKey = strAppKey;
}

tstring InteractGlobalManager::GetAppSecret()
{
    return m_strAppSecret;
}

void InteractGlobalManager::SetAppSecret(const tstring& strAppSecret)
{
    m_strAppSecret = strAppSecret;
}

tstring InteractGlobalManager::GetUsign()
{
    return m_strUsign;
}

void InteractGlobalManager::SetUsign(const tstring& strUsign)
{
    m_strUsign = strUsign;
}

InteractIMModel& InteractGlobalManager::GetInteractIMModel()
{
    return mInteractImModel;
}

void InteractGlobalManager::SetInteractImModel(const InteractIMModel& imModel)
{
    mInteractImModel = imModel;
}

InteractUserModel* InteractGlobalManager::GetInteractUserModel()
{
    return &mInteractUserModel;
}

void InteractGlobalManager::SetInteractUserModel(const InteractUserModel& userModel)
{
    mInteractUserModel = userModel;
}

tstring InteractGlobalManager::ConvertString(std::string str)
{
#ifdef UNICODE
    return Util::Encode::ASCIIToWide(str);
#else
    return str;
#endif // !UNICODE
}

tstring InteractGlobalManager::GetInteractUserId()
{
    return mInteractUserModel.userid;
}

bool InteractGlobalManager::IsLogin()
{
    return !mInteractUserModel.userid.empty();
}

HWND InteractGlobalManager::GetMainWindow()
{
    return m_hMainWindow;
}

void InteractGlobalManager::SetMainWindow(HWND hwnd)
{
    m_hMainWindow = hwnd;
}

void InteractGlobalManager::ShowMainWindow(bool bShow)
{
    if (m_hMainWindow != NULL && ::IsWindow(m_hMainWindow)) {
        if (bShow) {
            Util::Display::ForceShowWindow(m_hMainWindow);
        }
        else {
            ShowWindow(m_hMainWindow, SW_HIDE);
        }
    }
}
