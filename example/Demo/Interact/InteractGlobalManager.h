#pragma once
#include "ModelDef.h"
class InteractGlobalManager
{
public:
    InteractGlobalManager();
    ~InteractGlobalManager();

    static InteractGlobalManager& GetInstance() {
        static InteractGlobalManager instance;
        return instance;
    }


public:
    tstring GetBusinessId();
    void SetBusinessId(const tstring& strBusinessId);

    tstring GetChannelId();
    void SetChannelId(const tstring& strChannelId);

    tstring GetAppKey();
    void SetAppKey(const tstring& strAppKey);

    tstring GetAppSecret();
    void SetAppSecret(const tstring& strAppSecret);

    tstring GetUsign();
    void SetUsign(const tstring& strUsign);

    tstring CreateSessionId();

    tstring CreateStreamId(const tstring& strRoomId);
    tstring CreatePushAddr(const tstring& strStreamId);
    tstring CreatePullAddr(const tstring& strStreamId);
    tstring CreateMergeRtmp(const tstring& strStreamId);

    tstring GetDeviceId();

    InteractIMModel& GetInteractIMModel();
    void SetInteractImModel(const InteractIMModel& imModel);

    InteractUserModel* GetInteractUserModel();
    void SetInteractUserModel(const InteractUserModel& userModel);

    tstring GetInteractUserId();
    bool IsLogin();

    HWND GetMainWindow();
    void SetMainWindow(HWND hwnd);
    void ShowMainWindow(bool bShow);

private:
    tstring ConvertString(std::string str);

private:
    tstring m_strBusinessId;
    tstring m_strChannelId;
    tstring m_strAppKey;
    tstring m_strAppSecret;
    tstring m_strUsign;

    InteractIMModel mInteractImModel;
    InteractUserModel mInteractUserModel;

    HWND m_hMainWindow;
};

