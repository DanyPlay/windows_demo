#include "stdafx.h"
#include "InteractAnchorDlg.h"
#include "../InteractGlobalManager.h"
#include "../IM/InteractImManager.h"
#include "../../CommonLib/md5/md5.h"
#include "InteractInfoPanel.h"
#include "../InteractSetting.h"
#include <time.h>
#include <sstream>
#include "Toast.h"
#include <Dbt.h>

#ifdef UNICODE
#define tstringstream  std::wstringstream
#else
#define tstringstream  std::stringstream
#endif // !UNICODE

#define   TIMER_ID_HEART 1000


typedef struct _INTERACT_VIDEO_INFO_PARAM
{
    char	uid[256];
    int		width;
    int		height;
    int		elapsed;
    _INTERACT_VIDEO_INFO_PARAM() {
        memset(this, 0, sizeof(_INTERACT_VIDEO_INFO_PARAM));
    }

} INTERACT_VIDEO_INFO_PARAM;

typedef struct _INTERACT_COMMON_PARAM
{
    char	roomId[128];
    char	uid[128];
    int     intValue;
    char	charValue[256];
    int		elapsed;
    _INTERACT_COMMON_PARAM() {
        memset(this, 0, sizeof(_INTERACT_COMMON_PARAM));
    }

} INTERACT_COMMON_PARAM;

typedef struct _INTERACT_VOLUME_INDICATION
{
    QHVC::INTERACT::AudioVolumeInfo* speakers;
    int speakerNumber;
    unsigned int totalVolume;

    _INTERACT_VOLUME_INDICATION() {
        memset(this, 0, sizeof(_INTERACT_VOLUME_INDICATION));
    }

    ~_INTERACT_VOLUME_INDICATION() {
        if (speakers) {
            delete[] speakers;
            speakers = NULL;
        }
    }

} INTERACT_VOLUME_INDICATION;

typedef struct _INTERACT_AUDIO_QUALITY
{
    char	uid[128];
    QHVC::INTERACT::QUALITY	quality;
    unsigned short delay;
    unsigned short lost;

    _INTERACT_AUDIO_QUALITY() {
        memset(this, 0, sizeof(_INTERACT_AUDIO_QUALITY));
    }

} INTERACT_AUDIO_QUALITY;

typedef struct _INTERACT_NETWRK_QUALITY
{
    QHVC::INTERACT::QUALITY	txQuality;
    QHVC::INTERACT::QUALITY	rxQuality;

    _INTERACT_NETWRK_QUALITY() {
        memset(this, 0, sizeof(_INTERACT_NETWRK_QUALITY));
    }

} INTERACT_NETWRK_QUALITY;

typedef struct _INTERACT_DEVICE_CHANGE
{
    char	deviceID[128];;
    QHVC::INTERACT::AUDIO_DEVICE_TYPE deviceType;
    QHVC::INTERACT::DEVICE_STATE deviceState;

    _INTERACT_DEVICE_CHANGE() {
        memset(this, 0, sizeof(_INTERACT_DEVICE_CHANGE));
    }

} INTERACT_DEVICE_CHANGE;

class QHVCInteractCallbackImpl : public QHVC::INTERACT::IQHVCInteractCallback {
private:
    CInteractAnchorDlg* m_pDialog;
    HWND m_hNotifyWnd;
public:
    QHVCInteractCallbackImpl(CInteractAnchorDlg* pDialog) {
        m_pDialog = pDialog;
        if (m_pDialog != NULL) {
            m_hNotifyWnd = m_pDialog->GetHWND();
        }
        else {
            m_hNotifyWnd = NULL;
        }
    }

    virtual void OnWarning(int warn, const char* msg) {
        if (IsWindow(m_hNotifyWnd)) {
            INTERACT_COMMON_PARAM* lpData = new INTERACT_COMMON_PARAM();
            if (msg != NULL) {
                strcpy(lpData->charValue, msg);
            }

            lpData->intValue = warn;

            PostMessage(m_hNotifyWnd, WM_INTERACT_ON_WARNING, (WPARAM)lpData, NULL);
        }
    }


    virtual void OnError(int error, const char* msg) {
        if (IsWindow(m_hNotifyWnd)) {
            INTERACT_COMMON_PARAM* lpData = new INTERACT_COMMON_PARAM();
            if (msg != NULL) {
                strcpy(lpData->charValue, msg);
            }

            lpData->intValue = error;

            PostMessage(m_hNotifyWnd, WM_INTERACT_ON_ERROR, (WPARAM)lpData, NULL);
        }
    }

    virtual void OnLoadEngineSuccess(const char* roomId, const char* userId) {
        if (IsWindow(m_hNotifyWnd)) {
            INTERACT_COMMON_PARAM* lpData = new INTERACT_COMMON_PARAM();
            if (roomId != NULL) {
                strcpy(lpData->roomId, roomId);
            }
            if (userId != NULL) {
                strcpy(lpData->uid, userId);
            }

            SendMessage(m_hNotifyWnd, WM_INTERACT_ON_LOAD_ENGINE_SUCCESS, (WPARAM)lpData, NULL);
        }

    }

    virtual void OnJoinChannelSuccess(const char* roomId, const char* userId, long elapsed) {
        if (IsWindow(m_hNotifyWnd)) {
            INTERACT_COMMON_PARAM* lpData = new INTERACT_COMMON_PARAM();
            if (roomId != NULL) {
                strcpy(lpData->roomId, roomId);
            }
            if (userId != NULL) {
                strcpy(lpData->uid, userId);
            }
            lpData->elapsed = elapsed;

            PostMessage(m_hNotifyWnd, WM_INTERACT_ON_JOIN_CHANNEL_SUCCESS, (WPARAM)lpData, NULL);
        }
    }

    virtual void OnRejoinChannelSuccess(const char* roomId, const char* userId, long elapsed) {
        if (IsWindow(m_hNotifyWnd)) {
            INTERACT_COMMON_PARAM* lpData = new INTERACT_COMMON_PARAM();
            if (roomId != NULL) {
                strcpy(lpData->roomId, roomId);
            }
            if (userId != NULL) {
                strcpy(lpData->uid, userId);
            }
            lpData->elapsed = elapsed;

            PostMessage(m_hNotifyWnd, WM_INTERACT_ON_REJOIN_CHANNEL_SUCCESS, (WPARAM)lpData, NULL);
        }
    }

    virtual void OnLeaveChannel(const QHVC::INTERACT::RtcStats& rtcStats) {
        if (IsWindow(m_hNotifyWnd)) {
            PostMessage(m_hNotifyWnd, WM_INTERACT_ON_LEAVE_CHANNEL, NULL, NULL);
        }
    }

    virtual void OnChangeClientRoleSuccess(QHVC::INTERACT::CLIENT_ROLE clientRole) {
        if (IsWindow(m_hNotifyWnd)) {
            PostMessage(m_hNotifyWnd, WM_INTERACT_ON_CHANGE_CLIENT_ROLE, (WPARAM)clientRole, NULL);
        }
    }

    virtual void OnConnectionInterrupted() {
        if (IsWindow(m_hNotifyWnd)) {
            PostMessage(m_hNotifyWnd, WM_INTERACT_ON_CONNECT_INTERRUPTED, NULL, NULL);
        }
    }

    virtual void OnConnectionLost(int error) {
        if (IsWindow(m_hNotifyWnd)) {
            INTERACT_COMMON_PARAM* lpData = new INTERACT_COMMON_PARAM();
            lpData->intValue = error;

            PostMessage(m_hNotifyWnd, WM_INTERACT_ON_CONNECT_LOST, (WPARAM)lpData, NULL);
        }
    }

    virtual void OnUserJoined(const char* userId, long elapsed) {
        if (IsWindow(m_hNotifyWnd)) {
            PostMessage(m_hNotifyWnd, WM_INTERACT_ON_USER_JOIN, NULL, NULL);
        }
    }

    virtual void OnUserOffline(const char* userId, QHVC::INTERACT::USER_OFFLINE_REASON userOfflineReason) {
        if (IsWindow(m_hNotifyWnd)) {
            PostMessage(m_hNotifyWnd, WM_INTERACT_ON_USER_OFFLINE, NULL, NULL);
        }
    }

    virtual void OnUserEnableVideo(const char* userId, bool enabled) {
        if (IsWindow(m_hNotifyWnd)) {
            INTERACT_COMMON_PARAM* lpData = new INTERACT_COMMON_PARAM();
            if (userId != NULL) {
                strcpy(lpData->uid, userId);
            }
            lpData->intValue = enabled;

            PostMessage(m_hNotifyWnd, WM_INTERACT_ON_USER_ENABLE_VIDEO, (WPARAM)lpData, NULL);
        }
    }

    virtual void OnUserMuteAudio(const char* userId, bool muted) {
        if (IsWindow(m_hNotifyWnd)) {
            INTERACT_COMMON_PARAM* lpData = new INTERACT_COMMON_PARAM();
            if (userId != NULL) {
                strcpy(lpData->uid, userId);
            }
            lpData->intValue = muted;

            PostMessage(m_hNotifyWnd, WM_INTERACT_ON_USER_MUTE_AUDIO, (WPARAM)lpData, NULL);
        }
    }

    virtual void OnUserMuteVideo(const char* userId, bool muted) {
        if (IsWindow(m_hNotifyWnd)) {
            INTERACT_COMMON_PARAM* lpData = new INTERACT_COMMON_PARAM();
            if (userId != NULL) {
                strcpy(lpData->uid, userId);
            }
            lpData->intValue = muted;

            PostMessage(m_hNotifyWnd, WM_INTERACT_ON_USER_MUTE_VIDEO, (WPARAM)lpData, NULL);
        }
    }

    virtual void OnCameraReady() {
        if (IsWindow(m_hNotifyWnd)) {
            PostMessage(m_hNotifyWnd, WM_INTERACT_ON_CAMERA_READY, NULL, NULL);
        }
    }

    virtual void OnVideoStopped() {
        if (IsWindow(m_hNotifyWnd)) {
            PostMessage(m_hNotifyWnd, WM_INTERACT_ON_VIDEO_STOP, NULL, NULL);
        }
    }

    virtual void OnFirstLocalVideoFrame(int width, int height, int elapsed) {
        if (IsWindow(m_hNotifyWnd)) {
            INTERACT_VIDEO_INFO_PARAM* lpData = new INTERACT_VIDEO_INFO_PARAM();
            lpData->width = width;
            lpData->height = height;
            lpData->elapsed = elapsed;

            PostMessage(m_hNotifyWnd, WM_INTERACT_ON_FIRST_LOCAL_VIDEO_FRAME, (WPARAM)lpData, NULL);
        }
    }

    virtual void OnFirstRemoteVideoFrame(const char* userId, int width, int height, int elapsed) {
        if (IsWindow(m_hNotifyWnd)) {
            INTERACT_VIDEO_INFO_PARAM* lpData = new INTERACT_VIDEO_INFO_PARAM();
            strcpy(lpData->uid, userId);
            lpData->width = width;
            lpData->height = height;
            lpData->elapsed = elapsed;

            PostMessage(m_hNotifyWnd, WM_INTERACT_ON_FIRST_REMOTE_VIDEO_FRAME, (WPARAM)lpData, NULL);
        }
    }

    virtual void OnFirstRemoteVideoDecoded(const char* userId, int width, int height, int elapsed) {
        if (IsWindow(m_hNotifyWnd)) {
            INTERACT_VIDEO_INFO_PARAM* lpData = new INTERACT_VIDEO_INFO_PARAM();
            strcpy(lpData->uid, userId);
            lpData->width = width;
            lpData->height = height;
            lpData->elapsed = elapsed;

            PostMessage(m_hNotifyWnd, WM_INTERACT_ON_FIRST_REMOTE_VIDEO_DECODE, (WPARAM)lpData, NULL);
        }
    }

    virtual void OnLocalVideoStats(const QHVC::INTERACT::LocalVideoStats& stats) {
        if (IsWindow(m_hNotifyWnd)) {
            QHVC::INTERACT::LocalVideoStats* pStats = new QHVC::INTERACT::LocalVideoStats;
            if (pStats) {
                memcpy(pStats, &stats, sizeof(QHVC::INTERACT::LocalVideoStats));
            }

            PostMessage(m_hNotifyWnd, WM_INTERACT_ON_LOCAL_VIDEO_STATS, (WPARAM)pStats, NULL);
        }
    }

    virtual void OnRemoteVideoStats(const QHVC::INTERACT::RemoteVideoStats& stats) {
        if (IsWindow(m_hNotifyWnd)) {
            QHVC::INTERACT::RemoteVideoStats* pStats = new QHVC::INTERACT::RemoteVideoStats;
            if (pStats) {
                memcpy(pStats, &stats, sizeof(QHVC::INTERACT::RemoteVideoStats));
            }

            PostMessage(m_hNotifyWnd, WM_INTERACT_ON_REMOTE_VIDEO_STATS, (WPARAM)pStats, NULL);
        }
    }

    virtual void OnAudioQuality(const char* userId, QHVC::INTERACT::QUALITY quality, unsigned short delay, unsigned short lost) {
        if (IsWindow(m_hNotifyWnd)) {
            INTERACT_AUDIO_QUALITY * lpData = new INTERACT_AUDIO_QUALITY();
            if (lpData) {
                memcpy(lpData->uid, userId, sizeof(lpData->uid));
                lpData->quality = quality;
                lpData->delay = delay;
                lpData->lost = lost;
            }
            PostMessage(m_hNotifyWnd, WM_INTERACT_ON_AUDIO_QUALITY, (WPARAM)lpData, NULL);
        }
    }

    virtual void OnAudioVolumeIndication(const QHVC::INTERACT::AudioVolumeInfo* speakers, int speakerNumber, unsigned int totalVolume) {
        if (IsWindow(m_hNotifyWnd)) {
            INTERACT_VOLUME_INDICATION* lpData = new INTERACT_VOLUME_INDICATION();
            if (lpData) {
                if (speakers && speakerNumber > 0) {
                    lpData->speakers = new QHVC::INTERACT::AudioVolumeInfo[speakerNumber];
                    for (int i = 0; i < speakerNumber; i++) {
                        memcpy(lpData->speakers + i, speakers + i, sizeof(QHVC::INTERACT::AudioVolumeInfo));
                    }
                }
                lpData->speakerNumber = speakerNumber;
                lpData->totalVolume = totalVolume;
            }

            PostMessage(m_hNotifyWnd, WM_INTERACT_ON_AUDIO_VOLUME_INDICATION, (WPARAM)lpData, NULL);
        }
    }

    virtual void OnAudioMixingFinished() {
        if (IsWindow(m_hNotifyWnd)) {
            PostMessage(m_hNotifyWnd, WM_INTERACT_ON_AUDIO_MIXING_FINISH, NULL, NULL);
        }
    }

    virtual void OnRtcStats(const QHVC::INTERACT::RtcStats& stats) {
        if (IsWindow(m_hNotifyWnd)) {
            QHVC::INTERACT::RtcStats* pStats = new QHVC::INTERACT::RtcStats;
            if (pStats) {
                memcpy(pStats, &stats, sizeof(QHVC::INTERACT::RtcStats));
            }
            PostMessage(m_hNotifyWnd, WM_INTERACT_ON_RTC_STATS, (WPARAM)pStats, NULL);
        }
    }

    virtual void OnNetworkQuality(const char* userId, QHVC::INTERACT::QUALITY txQuality, QHVC::INTERACT::QUALITY rxQuality) {
        if (IsWindow(m_hNotifyWnd)) {
            INTERACT_NETWRK_QUALITY* lpData = new INTERACT_NETWRK_QUALITY();
            if (lpData) {
                lpData->txQuality = txQuality;
                lpData->rxQuality = rxQuality;
            }

            PostMessage(m_hNotifyWnd, WM_INTERACT_ON_NETWORK_QUALITY, (WPARAM)lpData, NULL);
        }
    }

    virtual void OnAudioDeviceStateChanged(const char* deviceId, QHVC::INTERACT::AUDIO_DEVICE_TYPE deviceType, QHVC::INTERACT::DEVICE_STATE deviceState) {
        if (IsWindow(m_hNotifyWnd)) {
            INTERACT_DEVICE_CHANGE* lpData = new INTERACT_DEVICE_CHANGE();
            if (lpData) {
                if (deviceId) {
                    memcpy(lpData->deviceID, deviceId, sizeof(lpData->deviceID));
                }

                lpData->deviceType = deviceType;
                lpData->deviceState = deviceState;
            }

            PostMessage(m_hNotifyWnd, WM_INTERACT_ON_AUDIO_DEVICE_CHANGE, (WPARAM)lpData, NULL);
        }
    }

    virtual void OnVideoDeviceStateChanged(const char* deviceId, QHVC::INTERACT::DEVICE_STATE deviceState) {
        if (IsWindow(m_hNotifyWnd)) {
            INTERACT_DEVICE_CHANGE* lpData = new INTERACT_DEVICE_CHANGE();
            if (lpData) {
                if (deviceId) {
                    memcpy(lpData->deviceID, deviceId, sizeof(lpData->deviceID));
                }

                lpData->deviceState = deviceState;
            }

            PostMessage(m_hNotifyWnd, WM_INTERACT_ON_VIDEO_DEVICE_CHANGE, (WPARAM)lpData, NULL);
        }
    }
};


DUI_BEGIN_MESSAGE_MAP(CInteractAnchorDlg, CDuiDialogImpl<CInteractAnchorDlg>)
DUI_ON_MSGTYPE(DUI_MSGTYPE_CLICK, OnClick)
DUI_END_MESSAGE_MAP()

CInteractAnchorDlg::CInteractAnchorDlg()
: m_pInfoPanel(NULL)
, m_bBroadcasterCountChange(true)
, m_bShowExitDailog(false)
, m_bAddHeartLog(false)
, m_bAddRoomInfoLog(false)
, m_bAddRoomUserListLog(false)
, m_bJoinChannelSuccess(false)
, m_bEngineFatalError(false)
, m_bEngineLoadOk(false)
{
    mUserIdentity = InteractConstant::USER_IDENTITY_AUDIENCE;
    mTargetUserIdentity = InteractConstant::USER_IDENTITY_AUDIENCE;
}


CInteractAnchorDlg::~CInteractAnchorDlg()
{
    // 移除demo服务器回调
    InteractServerApi::GetInstance().RemoveApiCallback(this);

    // 移除连麦引擎回调
    InteractImManager::GetInstance().RemoveApiCallback(this);

    // 释放引擎
    QHVC::INTERACT::QHVCInteract::ReleaseEngine();

    if (m_pQHVCInteractCallback != NULL) {
        delete m_pQHVCInteractCallback;
        m_pQHVCInteractCallback = NULL;
    }


    for (int i = 0; i < m_vectVideoWnd.size(); i++) {
        delete m_vectVideoWnd[i];
    }
    m_vectVideoWnd.clear();
}

LRESULT CInteractAnchorDlg::OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    m_pQHVCInteractCallback = new QHVCInteractCallbackImpl(this);

    SetWindowText(INTERACT_TITLE);

    if (IsAudioMode()) {
        CContainerUI* pLayoutVideo = static_cast<CContainerUI*>(FindControl(_T("layout_video")));
        CContainerUI* pLayoutAudio = static_cast<CContainerUI*>(FindControl(_T("layout_audio")));
        if (pLayoutVideo) {
            pLayoutVideo->SetVisible(false);
        }

        if (pLayoutAudio) {
            pLayoutAudio->SetVisible(true);
        }
        InitAudioList();
    }

    InteractServerApi::GetInstance().AddApiCallback(this);

    InteractImManager::GetInstance().AddApiCallback(this);

    tstring userId = InteractGlobalManager::GetInstance().GetInteractUserId();

    std::string allParams = "sname__" + STR2A(InteractGlobalManager::GetInstance().GetChannelId()) +
        "room_id__" + STR2A(mInteractRoomModel.roomId) +
        "uid__" + STR2A(InteractGlobalManager::GetInstance().GetInteractUserId()) +
        STR2A(tstring(InteractGlobalManager::GetInstance().GetAppSecret()));
    std::string uSign = CMD5Checksum::GetMD5((BYTE*)allParams.c_str(), allParams.size());
    InteractGlobalManager::GetInstance().SetUsign(STR2T(uSign));


    if (_tcsicmp(userId.c_str(), mInteractRoomModel.bindRoleId.c_str()) == 0) {
        // 创建房间的主播，不加入房间也行
        //InteractServerApi::GetInstance().JoinRoom(userId, mInteractRoomModel.roomId, InteractConstant::USER_IDENTITY_AUDIENCE);
        InteractServerApiRet ret;
        OnServerApiJoinRoomResult(ret, mInteractRoomModel);
    }
    else {
        InteractServerApi::GetInstance().JoinRoom(userId, mInteractRoomModel.roomId, InteractConstant::USER_IDENTITY_AUDIENCE);
    }


    DelayRefresh(1000);

    CListUI* m_pListLog = static_cast<CListUI*>(FindControl(_T("list_log")));
    if (m_pListLog != NULL) {
        m_pListLog->SetTextCallback(this);
    }

    return 0;
}

LRESULT CInteractAnchorDlg::OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    return 0;
}

void CInteractAnchorDlg::SetInteractUserIdentity(InteractConstant::USER_IDENTITY userIdentity)
{
    mUserIdentity = userIdentity;
}


CControlUI* CInteractAnchorDlg::CreateControl(LPCTSTR pstrClass)
{
    if (!pstrClass)
        return NULL;

    if (_tcsicmp(pstrClass, DUI_CTR_WINDOW) == 0)
    {
        //ATLASSERT(m_pVideoWnd == NULL);
        //ATLASSERT(m_pWindowUI == NULL);
        if (!IsAudioMode()) {
            CWindowUI* pWindowUI = new CWindowUI();

            CVideoWnd* pVideoWnd = new CVideoWnd();
            pVideoWnd->CreateEx(m_hWnd, pWindowUI);
            pVideoWnd->SetNotifyWnd(m_hWnd);

            m_vectVideoWnd.push_back(pVideoWnd);

            return pWindowUI;
        }
        else {
            return NULL;
        }

    }
    else if (_tcsicmp(pstrClass, _T("InteractInfoPanel")) == 0)
    {
        CWindowUI* pWindowUI = new CWindowUI();

        m_pInfoPanel = new CInteractInfoPanel();
        m_pInfoPanel->Create(m_hWnd, NULL);
        m_pInfoPanel->SetWindowUI(pWindowUI);

        pWindowUI->Attach(m_pInfoPanel->GetHWND());

        if (m_pInfoPanel) {
            m_pInfoPanel->SetInteractRoomModel(mInteractRoomModel);
        }

        return pWindowUI;
    }

    return NULL;
}

void CInteractAnchorDlg::OnClick(TNotifyUI& msg)
{
    CDuiString sCtrlName = msg.pSender->GetName();
    if (sCtrlName == _T("closebtn"))
    {
        // 定时
        KillTimer(GetHWND(), TIMER_ID_HEART);

        tstring userId = InteractGlobalManager::GetInstance().GetInteractUserId();

        if (m_bJoinChannelSuccess) {
            // 连麦引擎离开频道
            QHVC::INTERACT::QHVCInteract::LeaveChannel();
        }
        else {
            DoLeaveChannel();
        }


        return;
    }
    else if (sCtrlName == _T("btn_more")) {
        int tag = msg.pSender->GetTag();
        PostMessage(WM_TOOLBAR_SHOW, (WPARAM)tag, NULL);
    }
    else if (sCtrlName == _T("btn_kickout")) {
        int tag = msg.pSender->GetTag();
        PostMessage(WM_TOOLBAR_KICKOUT_GUEST, (WPARAM)tag, NULL);
    }
    else if (sCtrlName == _T("chk_video")) {
        int tag = msg.pSender->GetTag();
        CCheckBoxUI *pCheck = static_cast<CCheckBoxUI*>(msg.pSender);
        PostMessage(WM_TOOLBAR_MUTE_VIDEO, (WPARAM)tag, (LPARAM)(pCheck ? !pCheck->IsSelected() : false));
    }
    else if (sCtrlName == _T("chk_audio")) {
        int tag = msg.pSender->GetTag();
        CCheckBoxUI *pCheck = static_cast<CCheckBoxUI*>(msg.pSender);
        PostMessage(WM_TOOLBAR_MUTE_AUDIO, (WPARAM)tag, (LPARAM)(pCheck ? !pCheck->IsSelected() : false));
    }
    else if (sCtrlName == _T("chk_micro")) {
        int tag = msg.pSender->GetTag();
        CCheckBoxUI *pCheck = static_cast<CCheckBoxUI*>(msg.pSender);
        PostMessage(WM_TOOLBAR_MUTE_MICRO, (WPARAM)tag, (LPARAM)(pCheck ? !pCheck->IsSelected() : false));
    }

    Super::OnClick(msg);
}

void CInteractAnchorDlg::SetInteractRoomModel(const InteractRoomModel& interactRoomModel)
{
    mInteractRoomModel = interactRoomModel;
}

void CInteractAnchorDlg::OnServerApiJoinRoomResult(const InteractServerApiRet& ret, const InteractRoomModel& interactRoomModel)
{
    tstringstream ostr(_T(""));
    ostr << _T("OnServerApiJoinRoomResult ret:") << ret.errCode << _T(" msg:") << ret.errMessage;
    AddLog(ostr.str());

    if (ret.errCode == 0) {
        // 加入demo的房间后
        // 1、加入IM房间
        JoinImRoom();


        // 2、加载连麦引擎，引擎加载成功后，加入频道
        QHVC::INTERACT::QHVCInteract::SetPublicServiceInfo(
            STR2A(InteractGlobalManager::GetInstance().GetChannelId()).c_str(),
            STR2A(InteractGlobalManager::GetInstance().GetAppKey()).c_str(),
            STR2A(InteractGlobalManager::GetInstance().GetUsign()).c_str()
            );

        tstring streamId = InteractGlobalManager::GetInstance().CreateStreamId(interactRoomModel.roomId);
        tstring pushAddr = InteractGlobalManager::GetInstance().CreatePushAddr(streamId);
        tstring pullAddr = InteractGlobalManager::GetInstance().CreatePullAddr(streamId);
        m_strMergeRtmp = InteractGlobalManager::GetInstance().CreateMergeRtmp(streamId);
        LOGD(TAG_INTERACT, _T("pushAddr %s"), pushAddr.c_str());
        LOGD(TAG_INTERACT, _T("pullAddr %s"), pullAddr.c_str());
        LOGD(TAG_INTERACT, _T("mergeRtmp %s"), m_strMergeRtmp.c_str());

        Json::Value jobj;
        Json::FastWriter writer;
        //发送消息
        jobj[QHVC::INTERACT::ENGINE_OPTION::PUSH_ADDR] = STR2A(pushAddr);
        jobj[QHVC::INTERACT::ENGINE_OPTION::PULL_ADDR] = STR2A(pullAddr);

        std::string option = writer.write(jobj);

        tstring userId = InteractGlobalManager::GetInstance().GetInteractUserId();

        QHVC::INTERACT::QHVCInteract::LoadEngine(
            STR2A(mInteractRoomModel.roomId).c_str(),
            STR2A(userId).c_str(),
            STR2A(InteractGlobalManager::GetInstance().CreateSessionId()).c_str(),
            option.c_str(),
            m_pQHVCInteractCallback);

        // 4、更新demo的用户列表
        RefreshUserList();
    }
    else {
        if (IsWindowVisible(m_hWnd) && IsWindowEnabled(m_hWnd)) {
            TCHAR szMsg[MAX_PATH] = { 0 };
            _sntprintf_s(szMsg, ARRAYSIZE(szMsg), _T("进入房间失败：错误(%d)：%s"), ret.errCode, ret.errMessage.c_str());
            DuiMessageBox(m_hWnd, szMsg, INTERACT_TITLE, MB_OK);
            Close();
        }
    }
}

void CInteractAnchorDlg::OnServerApiUserLeaveRoomResult(const InteractServerApiRet& ret, const InteractRoomModel& interactRoomModel)
{
    tstringstream ostr(_T(""));
    ostr << _T("OnServerApiUserLeaveRoomResult ret:") << ret.errCode << _T(" msg:") << ret.errMessage;
    AddLog(ostr.str());
    // 暂时什么都不做
}

void CInteractAnchorDlg::OnServerApiGetRoomInfoResult(const InteractServerApiRet& ret, const InteractRoomModel& interactRoomModel)
{
    if (!m_bAddRoomInfoLog) {
        m_bAddRoomInfoLog = true;

        tstringstream ostr(_T(""));
        ostr << _T("OnServerApiGetRoomInfoResult ret:") << ret.errCode << _T(" msg:") << ret.errMessage;
        AddLog(ostr.str());
    }

    if (ret.errCode == 0) {
        m_pInfoPanel->SetInteractRoomModel(interactRoomModel);
    }
}


void CInteractAnchorDlg::OnServerApiGetRoomUserListResult(const InteractServerApiRet& ret, const std::vector<InteractUserModel>& interactUserModelList)
{
    if (!m_bAddRoomUserListLog) {
        m_bAddRoomUserListLog = true;

        tstringstream ostr(_T(""));
        ostr << _T("OnServerApiGetRoomUserListResult ret:") << ret.errCode << _T(" msg:") << ret.errMessage;
        AddLog(ostr.str());
    }

    if (ret.errCode == 0) {
        std::vector<InteractUserModel> userModelList;
        for (int i = 0; i < (int)interactUserModelList.size(); i++) {
            if (interactUserModelList[i].identity == InteractConstant::USER_IDENTITY_GUEST || interactUserModelList[i].identity == InteractConstant::USER_IDENTITY_ANCHOR) {
                userModelList.push_back(interactUserModelList[i]);
            }
        }

        RebuildUIdList(userModelList);
        if (!IsAudioMode()) {
            UpdateVideoList();
        }
        else {
            UpdateAudioList();
        }

        m_pInfoPanel->SetAnchorList(userModelList);
    }

}

void CInteractAnchorDlg::OnServerApiChangeUserdentityResult(const InteractServerApiRet& ret, const InteractRoomModel& interactRoomModel)
{
    tstringstream ostr(_T(""));
    ostr << _T("OnServerApiChangeUserdentityResult ret:") << ret.errCode << _T(" msg:") << ret.errMessage;
    AddLog(ostr.str());
    if (ret.errCode == 0) {
        if (mUserIdentity == InteractConstant::USER_IDENTITY_AUDIENCE &&  mTargetUserIdentity == InteractConstant::USER_IDENTITY_GUEST) {
            mUserIdentity = InteractConstant::USER_IDENTITY_GUEST;

            QHVC::INTERACT::QHVCInteract::SetClientRole(QHVC::INTERACT::CLIENT_ROLE_BROADCASTER);

        }
        else if (mUserIdentity == InteractConstant::USER_IDENTITY_GUEST &&  mTargetUserIdentity == InteractConstant::USER_IDENTITY_AUDIENCE) {
            mUserIdentity = InteractConstant::USER_IDENTITY_AUDIENCE;
            QHVC::INTERACT::QHVCInteract::SetClientRole(QHVC::INTERACT::CLIENT_ROLE_AUDIENCE);
        }
    }
}

void CInteractAnchorDlg::OnServerApiDismissRoomResult(const InteractServerApiRet& ret)
{
    tstringstream ostr(_T(""));
    ostr << _T("OnServerApiDismissRoomResult ret:") << ret.errCode << _T(" msg:") << ret.errMessage;
    AddLog(ostr.str());
    // todo 暂时什么都不做
}

void CInteractAnchorDlg::OnServerApiUserHeartResult(const InteractServerApiRet& ret)
{
    if (!m_bAddHeartLog) {
        m_bAddHeartLog = true;

        tstringstream ostr(_T(""));
        ostr << _T("OnServerApiUserHeartResult ret:") << ret.errCode << _T(" msg:") << ret.errMessage;
        AddLog(ostr.str());
    }

    // 返回10001表示房间不在了，退出
    if (ret.errCode == 10001) {
        if (m_bShowExitDailog) {
            return;
        }

        m_bShowExitDailog = true;
        DuiMessageBox(m_hWnd, _T("主播已掉线！"), INTERACT_TITLE, MB_OK);
        m_bShowExitDailog = false;
        CButtonUI* pbtnClose = static_cast<CButtonUI*>(FindControl(_T("closebtn")));
        if (pbtnClose != NULL) {
            pbtnClose->Activate();
        }
    }
}

void CInteractAnchorDlg::OnImConnectStatusChanged(ImConstant::CONNECT_STATUS connectStatus)
{
    AddLog(_T("OnImConnectStatusChanged status:") + STR2T(Util::String::Int2Str(connectStatus)));

    if (connectStatus == ImConstant::CONNECT_STATUS_DISCONNECTED || connectStatus == ImConstant::CONNECT_STATUS_SERVER_INVALID || connectStatus == ImConstant::CONNECT_STATUS_NETWORK_UNAVAILABLE) {
        DuiShowToast(GetHWND(), _T("网络断开"), 3);
    }
}

void CInteractAnchorDlg::OnImSendMessageSuccess()
{
    AddLog(_T("OnImSendMessageSuccess"));
}

void CInteractAnchorDlg::OnImSendMessageFailed(int errCode, const std::string& errMessage)
{
    AddLog(_T("OnImSendMessageFailed ") + STR2T(Util::String::StrFormat("err: %d msg: %s", errCode, errMessage.c_str())));
}

void CInteractAnchorDlg::OnImJoinRoomSuccess(const std::string& roomId)
{
    AddLog(_T("OnImJoinRoomSuccess ") + STR2T(roomId));
}

void CInteractAnchorDlg::OnImJoinRoomFailed(int errCode, const std::string& errMessage)
{
    AddLog(_T("OnImJoinRoomFailed ") + STR2T(Util::String::StrFormat("err: %d msg: %s", errCode, errMessage.c_str())));

    if (IsWindowVisible(m_hWnd) && IsWindowEnabled(m_hWnd)) {
        TCHAR szMsg[MAX_PATH] = { 0 };
        _sntprintf_s(szMsg, ARRAYSIZE(szMsg), _T("错误(%d)：%s"), errCode, STR2T(errMessage).c_str());
        DuiShowToast(GetHWND(), szMsg, 3);
    }
}

void CInteractAnchorDlg::OnImReceiveCommand(const InteractUserModel& userFrom, const CommandInfo& command)
{
    AddLog(_T("OnImReceiveCommand ") + STR2T(Util::String::StrFormat("%d", command.cmd)));

    tstring msg = userFrom.userid + _T(" ");
    if (command.cmd == ImConstant::CMD_ANCHOR_AGREE_JOIN) {
        msg.append(_T("同意加入房间"));
        tstring userId = InteractGlobalManager::GetInstance().GetInteractUserId();

        // 主播同意加入房间后，调整身份为嘉宾
        mTargetUserIdentity = InteractConstant::USER_IDENTITY_GUEST;
        InteractServerApi::GetInstance().ChangeUserdentity(userId, mInteractRoomModel.roomId, InteractConstant::USER_IDENTITY_GUEST);

        if (IsWindowVisible(m_hWnd) && IsWindowEnabled(m_hWnd)) {
            DuiShowToast(m_hWnd, msg.c_str(), 3);
        }
    }
    else if (command.cmd == ImConstant::CMD_ANCHOR_REFUSE_JOIN) {
        m_pInfoPanel->EnableJoin(true);
        msg.append(_T("拒绝加入房间"));
        if (IsWindowVisible(m_hWnd) && IsWindowEnabled(m_hWnd)) {
            DuiShowToast(m_hWnd, msg.c_str(), 3);
        }
    }
    else if (command.cmd == ImConstant::CMD_GUEST_ASK_JOIN) {
        std::vector<tstring>::iterator itFind = std::find(m_vectAskJoin.begin(), m_vectAskJoin.end(), userFrom.userid);
        if (itFind == m_vectAskJoin.end())
        {
            m_vectAskJoin.push_back(userFrom.userid);
            // 只有等于1发送消息，大于1说明消息框还在显示中
            if (m_vectAskJoin.size() == 1) {
                PostMessage(WM_GUEST_ASK_JOIN);
            }

        }
    }
    else if (command.cmd == ImConstant::CMD_ANCHOR_KICKOUT_GUEST) {
        // 收到踢出的消息，变为观众
        if (mUserIdentity == InteractConstant::USER_IDENTITY_GUEST) {
            tstring userId = InteractGlobalManager::GetInstance().GetInteractUserId();
            mTargetUserIdentity = InteractConstant::USER_IDENTITY_AUDIENCE;
            InteractServerApi::GetInstance().ChangeUserdentity(userId, mInteractRoomModel.roomId, InteractConstant::USER_IDENTITY_AUDIENCE);
        }
    }
    else if (command.cmd == ImConstant::CMD_GUEST_JOIN_NOTIFY) {
        RefreshUserList();
    }
    else if (command.cmd == ImConstant::CMD_ANCHOR_QUIT_NOTIFY) {
        msg.append(_T("主播退出了房间"));

        if (!m_bShowExitDailog) {
            m_bShowExitDailog = true;
            if (IsWindowVisible(m_hWnd) && IsWindowEnabled(m_hWnd)) {
                DuiMessageBox(m_hWnd, msg.c_str(), INTERACT_TITLE, MB_OK);
            }

            CButtonUI* pbtnClose = static_cast<CButtonUI*>(FindControl(_T("closebtn")));
            if (pbtnClose != NULL) {
                pbtnClose->Activate();
            }

            m_bShowExitDailog = false;
        }
    }
    else if (command.cmd == ImConstant::CMD_GUEST_QUIT_NOTIFY) {
        RefreshUserList();
    }
    else if (command.cmd == ImConstant::CMD_GUEST_KICKOUT_NOTIFY) {
        RefreshUserList();
    }
    else if (command.cmd == ImConstant::CMD_AUDIENT_JOIN_NOTIFY) {
        RefreshRoomModel();
    }
    else if (command.cmd == ImConstant::CMD_AUDIENT_QUIT_NOTIFY) {
        RefreshRoomModel();
    }
}

void CInteractAnchorDlg::OnImReceiveOtherCommand(const InteractUserModel& userFrom, const std::string& str)
{
    AddLog(_T("OnImReceiveOtherCommand"));
}

void CInteractAnchorDlg::JoinImRoom()
{
    InteractImManager::GetInstance().JoinChatRoom(mInteractRoomModel.roomId);
}

void CInteractAnchorDlg::LeaveImRoom()
{
    InteractImManager::GetInstance().QuitChatRoom();
}

LRESULT CInteractAnchorDlg::OnWarning(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    INTERACT_COMMON_PARAM* lpData = (INTERACT_COMMON_PARAM*)wParam;
    if (lpData) {
        AddLog(_T("OnWarning ") + STR2T(Util::String::StrFormat("err: %d msg: %s", lpData->intValue, lpData->charValue)));
        delete lpData;
    }
    return 0;
}

LRESULT CInteractAnchorDlg::OnError(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    m_bEngineFatalError = true;
    INTERACT_COMMON_PARAM* lpData = (INTERACT_COMMON_PARAM*)wParam;
    if (lpData) {
        AddLog(_T("OnError") + STR2T(Util::String::StrFormat("err: %d msg: %s", lpData->intValue, lpData->charValue)));

        if (!m_bShowExitDailog) {
            m_bShowExitDailog = true;
            DuiMessageBox(m_hWnd, STR2T(Util::String::StrFormat("err: %d msg: %s", lpData->intValue, lpData->charValue)).c_str(), INTERACT_TITLE, MB_OK);


            CButtonUI* pbtnClose = static_cast<CButtonUI*>(FindControl(_T("closebtn")));
            if (pbtnClose != NULL) {
                pbtnClose->Activate();
            }

            m_bShowExitDailog = false;
        }

        delete lpData;
    }
    return 0;
}

LRESULT CInteractAnchorDlg::OnLoadEngineSuccess(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    AddLog(_T("OnLoadEngineSuccess"));

    m_bEngineLoadOk = true;

    INTERACT_COMMON_PARAM* lpData = (INTERACT_COMMON_PARAM*)wParam;

    // 即使是只是观众，也要EnableVideo，要不看不了视频
    if (!IsAudioMode()) {
        QHVC::INTERACT::QHVCInteract::EnableVideo();
    }

    QHVC::INTERACT::QHVCInteract::SetChannelProfile(QHVC::INTERACT::CHANNEL_PROFILE_LIVE_BROADCASTING);

    tstring userId = InteractGlobalManager::GetInstance().GetInteractUserId();
    if (mUserIdentity == InteractConstant::USER_IDENTITY_ANCHOR || mUserIdentity == InteractConstant::USER_IDENTITY_GUEST) {
        QHVC::INTERACT::QHVCInteract::SetClientRole(QHVC::INTERACT::CLIENT_ROLE_BROADCASTER);

        tstring userId = InteractGlobalManager::GetInstance().GetInteractUserId();
        AddUIdToList(STR2A(userId));

        if (!IsAudioMode()) {
            UpdateVideoProfile();

            UpdateVideoList();
            QHVC::INTERACT::QHVCInteract::StartPreview();
        }
    }

    //注意：设置的信息会作为加入频道的参数，
    //更新合流信息
    UpdateMixStreamInfo();

    QHVC::INTERACT::QHVCInteract::JoinChannel();

    if (lpData) {
        delete lpData;
    }

    m_pInfoPanel->UpdateDeviceInfo();

    return 0;
}

LRESULT CInteractAnchorDlg::OnJoinChannelSuccess(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    AddLog(_T("OnJoinChannelSuccess"));

    m_bJoinChannelSuccess = true;

    INTERACT_COMMON_PARAM* lpData = (INTERACT_COMMON_PARAM*)wParam;
    if (lpData) {
        if (mUserIdentity == InteractConstant::USER_IDENTITY_ANCHOR || mUserIdentity == InteractConstant::USER_IDENTITY_GUEST) {
            AddUIdToList(lpData->uid);

            // 区分是否是语音房间
            if (!IsAudioMode()) {
                m_bBroadcasterCountChange = true;
                UpdateVideoList();
            }
            else {
                UpdateAudioList();
            }

        }

        delete lpData;
    }



    if (mUserIdentity == InteractConstant::USER_IDENTITY_ANCHOR) {

    }
    else if (mUserIdentity == InteractConstant::USER_IDENTITY_GUEST) {
        InteractImManager::GetInstance().SendNotify(ImConstant::CMD_GUEST_JOIN_NOTIFY, _T(""));
    }
    else if (mUserIdentity == InteractConstant::USER_IDENTITY_AUDIENCE) {
        InteractImManager::GetInstance().SendNotify(ImConstant::CMD_AUDIENT_JOIN_NOTIFY, _T(""));
    }

    return 0;
}

LRESULT CInteractAnchorDlg::OnRejoinChannelSuccess(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    AddLog(_T("OnRejoinChannelSuccess"));

    INTERACT_COMMON_PARAM* lpData = (INTERACT_COMMON_PARAM*)wParam;
    if (lpData) {
        delete lpData;
    }
    return 0;
}

LRESULT CInteractAnchorDlg::OnLeaveChannel(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    AddLog(_T("OnLeaveChannel"));

    m_bJoinChannelSuccess = false;

    INTERACT_COMMON_PARAM* lpData = (INTERACT_COMMON_PARAM*)wParam;

    if (lpData) {
        delete lpData;
    }
    DoLeaveChannel();

    return 0;
}

LRESULT CInteractAnchorDlg::OnChangeClientRoleSuccess(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    QHVC::INTERACT::CLIENT_ROLE clientRole = (QHVC::INTERACT::CLIENT_ROLE)wParam;

    tstring log = _T("OnChangeClientRoleSuccess");
    log.append(STR2T(Util::String::StrFormat("clientRole: %d", clientRole)));
    AddLog(log);

    tstring userId = InteractGlobalManager::GetInstance().GetInteractUserId();
    if (clientRole == QHVC::INTERACT::CLIENT_ROLE_BROADCASTER) {
        UpdateVideoProfile();
        AddUIdToList(STR2A(userId));

        m_bBroadcasterCountChange = true;
        if (!IsAudioMode()) {
            UpdateVideoList();
        }
        else {
            UpdateAudioList();
        }

        QHVC::INTERACT::QHVCInteract::StartPreview();
    }
    else {
        RemoveUIdFromList(STR2A(userId));
        if (!IsAudioMode()) {
            UpdateVideoList();
        }
        else {
            UpdateAudioList();
        }

        m_pInfoPanel->EnableJoin(true);

        QHVC::INTERACT::QHVCInteract::StopPreview();
    }

    RefreshUserList();
    RefreshRoomModel();
    return 0;
}

LRESULT CInteractAnchorDlg::OnConnectionInterrupted(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    AddLog(_T("OnConnectionInterrupted"));

    INTERACT_COMMON_PARAM* lpData = (INTERACT_COMMON_PARAM*)wParam;
    if (lpData) {
        delete lpData;
    }

    return 0;
}

LRESULT CInteractAnchorDlg::OnConnectionLost(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    AddLog(_T("OnConnectionLost"));

    INTERACT_COMMON_PARAM* lpData = (INTERACT_COMMON_PARAM*)wParam;
    if (lpData) {
        delete lpData;
    }

    return 0;
}

LRESULT CInteractAnchorDlg::OnUserJoined(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    AddLog(_T("OnUserJoined"));

    INTERACT_COMMON_PARAM* lpData = (INTERACT_COMMON_PARAM*)wParam;
    if (lpData) {
        delete lpData;
    }

    DelayRefresh(200);
    return 0;
}

LRESULT CInteractAnchorDlg::OnUserOffline(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    AddLog(_T("OnUserOffline"));

    INTERACT_COMMON_PARAM* lpData = (INTERACT_COMMON_PARAM*)wParam;
    if (lpData) {
        delete lpData;
    }

    DelayRefresh(200);

    return 0;
}

LRESULT CInteractAnchorDlg::OnUserEnableVideo(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    AddLog(_T("OnUserEnableVideo"));

    INTERACT_COMMON_PARAM* lpData = (INTERACT_COMMON_PARAM*)wParam;
    if (lpData) {
        delete lpData;
    }
    return 0;
}

LRESULT CInteractAnchorDlg::OnUserMuteAudio(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    AddLog(_T("OnUserMuteAudio"));

    INTERACT_COMMON_PARAM* lpData = (INTERACT_COMMON_PARAM*)wParam;
    if (lpData) {
        delete lpData;
    }
    return 0;
}
LRESULT CInteractAnchorDlg::OnUserMuteVideo(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    AddLog(_T("OnUserMuteVideo"));

    INTERACT_COMMON_PARAM* lpData = (INTERACT_COMMON_PARAM*)wParam;
    if (lpData) {
        delete lpData;
    }
    return 0;
}
LRESULT CInteractAnchorDlg::OnCameraReady(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    AddLog(_T("OnCameraReady"));

    return 0;
}
LRESULT CInteractAnchorDlg::OnVideoStopped(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    AddLog(_T("OnVideoStopped"));

    return 0;
}

LRESULT CInteractAnchorDlg::OnFirstLocalVideoFrame(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    AddLog(_T("OnFirstLocalVideoFrame"));

    INTERACT_VIDEO_INFO_PARAM* lpData = (INTERACT_VIDEO_INFO_PARAM*)wParam;

    if (lpData) {
        delete lpData;
    }

    return 0;
}

LRESULT CInteractAnchorDlg::OnFirstRemoteVideoFrame(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    AddLog(_T("OnFirstRemoteVideoFrame"));

    INTERACT_VIDEO_INFO_PARAM* lpData = (INTERACT_VIDEO_INFO_PARAM*)wParam;

    if (lpData) {
        delete lpData;
    }

    return 0;
}

LRESULT CInteractAnchorDlg::OnFirstRemoteVideoDecoded(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    AddLog(_T("OnFirstRemoteVideoDecoded"));

    INTERACT_VIDEO_INFO_PARAM* lpData = (INTERACT_VIDEO_INFO_PARAM*)wParam;
    if (lpData) {
        AddUIdToList(lpData->uid);

        m_bBroadcasterCountChange = true;
        // 判断是否是语音房间
        if (!IsAudioMode()) {
            UpdateVideoList();

            // 远端视频有数据了，更新合流信息
            UpdateMixStreamInfo();
        }

        delete lpData;
    }
    return 0;
}

LRESULT CInteractAnchorDlg::OnLocalVideoStats(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    //AddLog(_T("OnLocalVideoStats"));

    QHVC::INTERACT::LocalVideoStats* pStats = (QHVC::INTERACT::LocalVideoStats*)wParam;
    if (pStats) {
        mInteractStats.localVideoStatus = *pStats;
        m_pInfoPanel->SetInteractStats(mInteractStats);
        delete pStats;
    }
    return 0;
}

LRESULT CInteractAnchorDlg::OnRemoteVideoStats(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    //AddLog(_T("OnRemoteVideoStats"));

    QHVC::INTERACT::RemoteVideoStats* pStats = (QHVC::INTERACT::RemoteVideoStats*)wParam;
    if (pStats) {
        mInteractStats.mapRemoteVideoStatus[pStats->userId] = *pStats;
        m_pInfoPanel->SetInteractStats(mInteractStats);
        delete pStats;
    }
    return 0;
}

LRESULT CInteractAnchorDlg::OnAudioQuality(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    //AddLog(_T("OnAudioQuality"));

    INTERACT_AUDIO_QUALITY* lpData = (INTERACT_AUDIO_QUALITY*)wParam;
    if (lpData) {
        delete lpData;
    }

    return 0;
}

LRESULT CInteractAnchorDlg::OnAudioVolumeIndication(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    //AddLog(_T("OnAudioVolumeIndication"));

    INTERACT_VOLUME_INDICATION* lpData = (INTERACT_VOLUME_INDICATION*)wParam;
    if (lpData) {
        delete lpData;
    }
    return 0;
}
LRESULT CInteractAnchorDlg::OnAudioMixingFinished(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    AddLog(_T("OnAudioMixingFinished"));

    return 0;
}
LRESULT CInteractAnchorDlg::OnRtcStats(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    //AddLog(_T("OnRtcStats"));

    QHVC::INTERACT::RtcStats* lpData = (QHVC::INTERACT::RtcStats*)wParam;
    if (lpData) {
        mInteractStats.rtcStatus = *lpData;

        m_pInfoPanel->SetInteractStats(mInteractStats);

        delete lpData;
    }

    return 0;
}
LRESULT CInteractAnchorDlg::OnNetworkQuality(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    //AddLog(_T("OnNetworkQuality"));

    INTERACT_NETWRK_QUALITY* lpData = (INTERACT_NETWRK_QUALITY*)wParam;
    if (lpData) {

        mInteractStats.rxQuality = lpData->rxQuality;
        mInteractStats.txQuality = lpData->txQuality;

        m_pInfoPanel->SetInteractStats(mInteractStats);
        delete lpData;
    }

    return 0;
}

LRESULT CInteractAnchorDlg::OnAudioDeviceChange(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    tstring log = _T("OnAudioDeviceChange");

    INTERACT_DEVICE_CHANGE* lpData = (INTERACT_DEVICE_CHANGE*)wParam;
    if (lpData) {
        if (lpData->deviceID) {
            log.append(STR2T(lpData->deviceID));
        }
        log.append(STR2T(Util::String::StrFormat("type: %d status: %d", lpData->deviceType, lpData->deviceState)));
        delete lpData;
    }
    m_pInfoPanel->UpdateDeviceInfo();

    AddLog(log);
    return 0;
}
LRESULT CInteractAnchorDlg::OnVideoDeviceChange(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    tstring log = _T("OnVideoDeviceChange");

    INTERACT_DEVICE_CHANGE* lpData = (INTERACT_DEVICE_CHANGE*)wParam;
    if (lpData) {
        if (lpData->deviceID) {
            log.append(STR2T(lpData->deviceID));
        }
        log.append(STR2T(Util::String::StrFormat("status: %d", lpData->deviceState)));

        delete lpData;
    }

    m_pInfoPanel->UpdateDeviceInfo();

    AddLog(log);
    return 0;
}

LRESULT CInteractAnchorDlg::HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    switch (uMsg)
    {
        case WM_MOVING:
            for (int i = 0; i < m_vectVideoWnd.size(); i++) {
                m_vectVideoWnd[i]->OnMove();

            }
        default:
            return Super::HandleCustomMessage(uMsg, wParam, lParam, bHandled);
    }
}

LRESULT CInteractAnchorDlg::OnTimer(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    if (wParam == TIMER_ID_HEART) {
        tstring userId = InteractGlobalManager::GetInstance().GetInteractUserId();
        InteractServerApi::GetInstance().UserHeart(userId, mInteractRoomModel.roomId);

        RefreshUserList();
        RefreshRoomModel();

        InteractImManager::GetInstance().CheckConnect();

        DelayRefresh(5000);
    }

    bHandled = false;

    return 0;
}

void CInteractAnchorDlg::RefreshUserList()
{
    tstring userId = InteractGlobalManager::GetInstance().GetInteractUserId();
    std::vector<int> vectIdentitys;
    vectIdentitys.push_back(InteractConstant::USER_IDENTITY_ANCHOR);
    vectIdentitys.push_back(InteractConstant::USER_IDENTITY_GUEST);
    vectIdentitys.push_back(InteractConstant::USER_IDENTITY_AUDIENCE);
    InteractServerApi::GetInstance().GetRoomUserList(userId, mInteractRoomModel.roomId, vectIdentitys);
}

void CInteractAnchorDlg::RefreshRoomModel()
{
    tstring userId = InteractGlobalManager::GetInstance().GetInteractUserId();
    InteractServerApi::GetInstance().GetRoomInfo(userId, mInteractRoomModel.roomId);
}

void CInteractAnchorDlg::UpdateMixStreamInfo()
{
    if (mUserIdentity != InteractConstant::USER_IDENTITY_ANCHOR) {
        return;
    }

    QHVC::INTERACT::MixStreamConfig* pMixStreamConfig = new QHVC::INTERACT::MixStreamConfig();
    pMixStreamConfig->canvasWidth = 510;
    pMixStreamConfig->canvasHeight = 646;
    strcpy(pMixStreamConfig->mixStreamAddr, STR2A(m_strMergeRtmp).c_str());
    pMixStreamConfig->mIframeInterval = 2;
    pMixStreamConfig->videoBitrate = 800;

    QHVC::INTERACT::MixStreamRegion* pMixStreamRegion = new QHVC::INTERACT::MixStreamRegion[10];
    for (int i = 0; i < (int)m_vectVideoUId.size(); i++) {
        pMixStreamRegion->zOrder = i + 1;
        if (i == 0) {
            pMixStreamRegion[i].x = 0;
            pMixStreamRegion[i].y = 0;
            pMixStreamRegion[i].width = 341;
            pMixStreamRegion[i].height = 430;
        }
        else if (i == 1) {
            pMixStreamRegion[i].x = 341;
            pMixStreamRegion[i].y = 0;
            pMixStreamRegion[i].width = 170;
            pMixStreamRegion[i].height = 215;
        }
        else if (i == 2) {
            pMixStreamRegion[i].x = 341;
            pMixStreamRegion[i].y = 170;
            pMixStreamRegion[i].width = 170;
            pMixStreamRegion[i].height = 215;
        }
        else if (i == 3) {
            pMixStreamRegion[i].x = 341;
            pMixStreamRegion[i].y = 170;
            pMixStreamRegion[i].width = 170;
            pMixStreamRegion[i].height = 215;
        }
        else if (i == 4) {
            pMixStreamRegion[i].x = 0;
            pMixStreamRegion[i].y = 340;
            pMixStreamRegion[i].width = 170;
            pMixStreamRegion[i].height = 215;

        }
        else if (i == 5) {
            pMixStreamRegion[i].x = 215;
            pMixStreamRegion[i].y = 340;
            pMixStreamRegion[i].width = 170;
            pMixStreamRegion[i].height = 215;
        }
        else if (i == 6) {
            pMixStreamRegion[i].x = 430;
            pMixStreamRegion[i].y = 340;
            pMixStreamRegion[i].width = 170;
            pMixStreamRegion[i].height = 215;;
        }
        pMixStreamRegion[i].alpha = 1;
        pMixStreamRegion[i].renderMode = QHVC::INTERACT::RENDER_MODE_FIT;
        strcpy(pMixStreamRegion[i].userID, m_vectVideoUId[i].c_str());
    }


    QHVC::INTERACT::QHVCInteract::SetMixStreamInfo(pMixStreamConfig, QHVC::INTERACT::STREAM_LIFE_CYCLE_BIND_USER);
    QHVC::INTERACT::QHVCInteract::SetVideoCompositingLayout(pMixStreamRegion, m_vectVideoUId.size());

    delete pMixStreamConfig;
    delete[] pMixStreamRegion;
}

void CInteractAnchorDlg::UpdateVideoList()
{
    if (m_bEngineFatalError) {
        return;
    }

    if (!m_bEngineLoadOk) {
        return;
    }

    // 龌蹉的写法，那个layerwindows不好使吧，先认为
    if (::IsIconic(GetHWND())) {
        return;
    }

    std::string myUserId = STR2A(InteractGlobalManager::GetInstance().GetInteractUserId());
    std::string bindUserId = STR2A(mInteractRoomModel.bindRoleId);

    for (int i = 0; i < (int)m_vectVideoWnd.size(); i++) {
        CVideoWnd* pItem = m_vectVideoWnd[i];
        pItem->SetTag(i);

        UpdateItem(pItem, i, (int)m_vectVideoUId.size());
        if (m_bBroadcasterCountChange) {
            if (i < (int)m_vectVideoUId.size()) {
                tstring controlName = STR2T(Util::String::StrFormat("videoSub%d", i));
                CWindowUI* pControl = static_cast<CWindowUI*>(FindControl(controlName.c_str()));
                if (pControl != NULL && pControl->GetHWND() != NULL && ::IsWindow(pControl->GetHWND())) {
                    if (strcmp(myUserId.c_str(), m_vectVideoUId[i].c_str()) == 0) {
                        // 本地视频需要在StartPreview前设置才有效
                        QHVC::INTERACT::QHVCInteract::SetLocalRenderMode(QHVC::INTERACT::RENDER_MODE_FIT);
                        QHVC::INTERACT::QHVCInteract::SetupLocalVideo(pControl->GetHWND(), QHVC::INTERACT::RENDER_MODE_FIT, m_vectVideoUId[i].c_str());
                    }
                    else {
                        // 远程视频需要在收到第一帧onFirstRemoteVideoDecoded后设置才有效
                        QHVC::INTERACT::QHVCInteract::SetRemoteRenderMode(m_vectVideoUId[i].c_str(), QHVC::INTERACT::RENDER_MODE_FIT);
                        QHVC::INTERACT::QHVCInteract::SetupRemoteVideo(pControl->GetHWND(), QHVC::INTERACT::RENDER_MODE_FIT, m_vectVideoUId[i].c_str(), "");
                    }
                }

            }

            ::InvalidateRect(m_vectVideoWnd[i]->GetHWND(), NULL, TRUE);
            ::UpdateWindow(m_vectVideoWnd[i]->GetHWND());
        }
    }

    m_bBroadcasterCountChange = false;
}

void CInteractAnchorDlg::UpdateAudioList()
{
    if (m_bEngineFatalError) {
        return;
    }

    if (!m_bEngineLoadOk) {
        return;
    }

    CTileLayoutUI* pAudioBroadcaster = static_cast<CTileLayoutUI*>(FindControl(_T("list_audio_broadcaster")));

    int index = -1;
    for (int i = 0; i < (int)pAudioBroadcaster->GetCount(); i++) {
        CContainerUI* pItem = static_cast<CContainerUI*>(pAudioBroadcaster->GetItemAt(i));
        if (pItem->GetClass() == DUI_CTR_SCROLLBAR) {
            continue;
        }
        index++;

        UpdateItem(pItem, index, (int)m_vectVideoUId.size());
    }
}

void CInteractAnchorDlg::UpdateItem(void* pItemTemp, int nIdx, int nCount)
{
    std::string myUserId = STR2A(InteractGlobalManager::GetInstance().GetInteractUserId());
    std::string bindUserId = STR2A(mInteractRoomModel.bindRoleId);


    CLabelUI* pLabelIdentity = NULL;
    CLabelUI* pLabelUserId = NULL;
    CLabelUI* pLabelLiveDuration = NULL;
    CLabelUI* pLabelWait = NULL;

    CButtonUI* pBtnMore = NULL;
    CButtonUI* pBtnClose = NULL;
    CLabelUI* pBtnWaitIcon = NULL;
    CContainerUI* pLayoutToolbar = NULL;

    CCheckBoxUI* pCheckVideo = NULL;
    CCheckBoxUI* pCheckAudio = NULL;
    CCheckBoxUI* pCheckMicro = NULL;

    CVideoWnd* pItem = NULL;
    if (!IsAudioMode()) {
        CVideoWnd* pItem = (CVideoWnd*)pItemTemp;
        pLabelIdentity = static_cast<CLabelUI*>(pItem->FindSubControl(_T("txt_identity")));
        pLabelUserId = static_cast<CLabelUI*>(pItem->FindSubControl(_T("txt_author_id")));
        pLabelLiveDuration = static_cast<CLabelUI*>(pItem->FindSubControl(_T("txt_duration")));
        pLabelWait = static_cast<CLabelUI*>(pItem->FindSubControl(_T("txt_wait")));

        pBtnMore = static_cast<CButtonUI*>(pItem->FindSubControl(_T("btn_more")));
        pBtnClose = static_cast<CButtonUI*>(pItem->FindSubControl(_T("btn_kickout")));
        pBtnWaitIcon = static_cast<CButtonUI*>(pItem->FindSubControl(_T("btn_wait_icon")));
        pLayoutToolbar = static_cast<CContainerUI*>(pItem->FindSubControl(_T("layout_toolbar")));

        pCheckVideo = static_cast<CCheckBoxUI*>(pItem->FindSubControl(_T("chk_video")));
        pCheckAudio = static_cast<CCheckBoxUI*>(pItem->FindSubControl(_T("chk_audio")));
        pCheckMicro = static_cast<CCheckBoxUI*>(pItem->FindSubControl(_T("chk_micro")));
    }
    else {
        CContainerUI* pItem = (CContainerUI*)pItemTemp;
        pLabelIdentity = static_cast<CLabelUI*>(pItem->FindSubControl(_T("txt_identity")));
        pLabelUserId = static_cast<CLabelUI*>(pItem->FindSubControl(_T("txt_author_id")));
        pLabelLiveDuration = static_cast<CLabelUI*>(pItem->FindSubControl(_T("txt_duration")));
        pLabelWait = static_cast<CLabelUI*>(pItem->FindSubControl(_T("txt_wait")));

        pBtnMore = static_cast<CButtonUI*>(pItem->FindSubControl(_T("btn_more")));
        pBtnClose = static_cast<CButtonUI*>(pItem->FindSubControl(_T("btn_kickout")));
        pBtnWaitIcon = static_cast<CButtonUI*>(pItem->FindSubControl(_T("btn_wait_icon")));
        pLayoutToolbar = static_cast<CContainerUI*>(pItem->FindSubControl(_T("layout_toolbar")));

        pCheckVideo = static_cast<CCheckBoxUI*>(pItem->FindSubControl(_T("chk_video")));
        pCheckAudio = static_cast<CCheckBoxUI*>(pItem->FindSubControl(_T("chk_audio")));
        pCheckMicro = static_cast<CCheckBoxUI*>(pItem->FindSubControl(_T("chk_micro")));
    }


    if (nIdx >= nCount) {
        if (pLabelIdentity) {
            pLabelIdentity->SetText(_T(""));
        }
        if (pLabelUserId) {
            pLabelUserId->SetText(_T(""));
        }
        if (pLabelLiveDuration) {
            pLabelLiveDuration->SetText(_T(""));
        }
        if (pLabelWait) {
            pLabelWait->SetVisible(true);
        }
        if (pBtnMore) {
            pBtnMore->SetVisible(false);
        }
        if (pBtnClose) {
            pBtnClose->SetVisible(false);
        }
        if (IsAudioMode()) {
            if (pBtnWaitIcon) {
                pBtnWaitIcon->SetVisible(true);
                pBtnWaitIcon->SetEnabled(false);
            }
        }
        else {
            if (pBtnWaitIcon) {
                pBtnWaitIcon->SetVisible(true);
            }
        }

        if (pLayoutToolbar) {
            pLayoutToolbar->SetVisible(false);
        }
    }
    else {

        if (pLabelIdentity) {
            if (strcmp(bindUserId.c_str(), m_vectVideoUId[nIdx].c_str()) == 0) {
                pLabelIdentity->SetText(_T("主播"));
            }
            else {
                pLabelIdentity->SetText(_T("嘉宾"));
            }
        }

        if (pLabelUserId) {
            pLabelUserId->SetText(STR2T("ID:" + m_vectVideoUId[nIdx]).c_str());
        }

        if (pLabelLiveDuration) {
            pLabelLiveDuration->SetText(_T(""));
        }

        if (pLabelWait) {
            pLabelWait->SetVisible(false);
        }
        // 如果自己是主播，并且播放着不是自己，或者自己不是主播，但是自己的视频 显示踢出按钮
        if ((strcmp(bindUserId.c_str(), myUserId.c_str()) == 0 && strcmp(bindUserId.c_str(), m_vectVideoUId[nIdx].c_str()) != 0) ||
            (strcmp(bindUserId.c_str(), myUserId.c_str()) != 0 && strcmp(myUserId.c_str(), m_vectVideoUId[nIdx].c_str()) == 0)) {
            if (pBtnClose) {
                pBtnClose->SetVisible(true);
            }
        }
        else {
            if (pBtnClose) {
                pBtnClose->SetVisible(false);
            }
        }

        // 如果是自己
        std::set<std::string>::iterator itAudio = m_setMuteAudio.find(m_vectVideoUId[nIdx]);
        bool muteAudio = itAudio != m_setMuteAudio.end();

        std::set<std::string>::iterator itVideo = m_setMuteVideo.find(m_vectVideoUId[nIdx]);
        bool muteVideo = itVideo != m_setMuteVideo.end();

        if (strcmp(myUserId.c_str(), m_vectVideoUId[nIdx].c_str()) == 0) {
            if (pCheckMicro) {
                pCheckMicro->SetVisible(true);
                pCheckMicro->Selected(muteAudio);
            }
            if (pCheckAudio) {
                pCheckAudio->SetVisible(false);
                pCheckAudio->SetVisible(false);
            }

        }
        else {
            if (pCheckMicro) {
                pCheckMicro->SetVisible(false);
                pCheckMicro->Selected(muteAudio);
            }
            if (pCheckAudio) {
                pCheckAudio->SetVisible(true);
                pCheckAudio->Selected(muteAudio);
            }
        }

        if (!IsAudioMode()) {
            if (pCheckVideo) {
                pCheckVideo->SetVisible(true);
                pCheckVideo->Selected(muteVideo);
            }
        }
        else {
            if (pCheckVideo) {
                pCheckVideo->SetVisible(false);
            }
        }

        if (pBtnMore) {
            pBtnMore->SetVisible(true);
        }

        if (!IsAudioMode()) {
            if (pBtnWaitIcon) {
                pBtnWaitIcon->SetVisible(false);
            }
        }
        else {
            if (pBtnWaitIcon) {
                pBtnWaitIcon->SetVisible(true);
                pBtnWaitIcon->SetEnabled(true);
            }
        }
    }
}

void CInteractAnchorDlg::AddUIdToList(const std::string& uid)
{
    for (int i = 0; i < (int)m_vectVideoUId.size(); i++) {
        if (strcmp(uid.c_str(), m_vectVideoUId[i].c_str()) == 0) {
            return;
        }
    }

    m_vectVideoUId.push_back(uid);
    m_bBroadcasterCountChange = true;
}

void CInteractAnchorDlg::RemoveUIdFromList(const std::string& uid)
{
    for (int i = 0; i < (int)m_vectVideoUId.size(); i++) {
        if (strcmp(uid.c_str(), m_vectVideoUId[i].c_str()) == 0) {
            m_vectVideoUId.erase(m_vectVideoUId.begin() + i);
            m_bBroadcasterCountChange = true;
            return;
        }
    }
}

void CInteractAnchorDlg::RebuildUIdList(const std::vector<InteractUserModel>& vectUserModel)
{
    std::vector<std::string> vectNewUid;
    for (int i = 0; i < (int)vectUserModel.size(); i++) {
        vectNewUid.push_back(STR2A(vectUserModel[i].userid));
    }

    for (std::vector<std::string>::iterator it = m_vectVideoUId.begin(); it != m_vectVideoUId.end();) {
        std::vector<std::string>::iterator itFind = std::find(vectNewUid.begin(), vectNewUid.end(), *it);
        if (itFind == vectNewUid.end()) {
            it = m_vectVideoUId.erase(it);
            m_bBroadcasterCountChange = true;
        }
        else {
            ++it;
        }
    }

    for (std::vector<std::string>::iterator it = vectNewUid.begin(); it != vectNewUid.end(); ++it) {
        std::vector<std::string>::iterator itFind = std::find(m_vectVideoUId.begin(), m_vectVideoUId.end(), *it);
        if (itFind == m_vectVideoUId.end()) {
            m_vectVideoUId.push_back(*it);
            m_bBroadcasterCountChange = true;
        }
    }
}

void CInteractAnchorDlg::AddLog(const tstring & logInfo)
{
    time_t    now = time(0);
    char      timeTag[256] = {};
    struct tm *cur_time;

    cur_time = localtime(&now);
    _snprintf(timeTag, sizeof(timeTag), "%02d:%02d:%02d",
        cur_time->tm_hour,
        cur_time->tm_min,
        cur_time->tm_sec);

    tstring strLog = STR2T(timeTag) + _T(" ") + logInfo;

    if (m_vectLog.size() > 500) {
        CListUI* m_pListLog = static_cast<CListUI*>(FindControl(_T("list_log")));
        if (m_pListLog != NULL) {
            m_pListLog->RemoveAll();
        }
        m_vectLog.clear();
    }
    m_vectLog.push_back(strLog);

    CListUI* m_pListLog = static_cast<CListUI*>(FindControl(_T("list_log")));
    if (m_pListLog != NULL) {
        while (m_pListLog->GetCount() < m_vectLog.size())
        {
            CListTextElementUI* pListElement = new CListTextElementUI;
            if (m_pListLog != NULL) {
                m_pListLog->Add(pListElement);
            }
        }
        m_pListLog->Scroll(0, 1000000);
    }
}

LPCTSTR CInteractAnchorDlg::GetItemText(CControlUI* pList, int iItem, int iSubItem)
{
    TCHAR szBuf[MAX_PATH] = { 0 };
    switch (iSubItem)
    {
        case 0: {
            _stprintf(szBuf, _T("%s"), m_vectLog[iItem].c_str());
            break;
        }
    }
    pList->SetToolTip(szBuf);
    pList->SetUserData(szBuf);
    return pList->GetUserData();
}

void CInteractAnchorDlg::UpdateVideoProfile()
{
    InteractSetting& setting = InteractSetting::GetInstance();
    int profileMode = setting.GetProfileMode(mUserIdentity);
    if (profileMode > 0) {
        QHVC::INTERACT::QHVCInteract::SetVideoProfile((QHVC::INTERACT::VIDEO_PROFILE)profileMode, false);
    }
    else {
        QHVC::INTERACT::QHVCInteract::SetVideoProfileEx(setting.GetResolutionWidth(mUserIdentity),
            setting.GetResolutionHeight(mUserIdentity),
            setting.GetFrameRate(mUserIdentity),
            setting.GetBitrate(mUserIdentity));
    }
}

bool CInteractAnchorDlg::IsAudioMode()
{
    if (mInteractRoomModel.talkType == InteractConstant::TALK_TYPE_AUDIO){
        return true;
    }

    return false;
}

void CInteractAnchorDlg::InitAudioList()
{
    CTileLayoutUI* pAudioBroadcaster = static_cast<CTileLayoutUI*>(FindControl(_T("list_audio_broadcaster")));

    CDialogBuilder builder;
    CContainerUI* pAudioItem = static_cast<CContainerUI*>(builder.Create(_T("AudioItem.xml"), (UINT)0));

    pAudioBroadcaster->RemoveAll();
    for (int i = 0; i < mInteractRoomModel.maxNum; i++)
    {
        if (pAudioItem == NULL) pAudioItem = static_cast<CContainerUI*>(builder.Create());
        if (pAudioItem != NULL) {
            SetControlTag(pAudioItem, i);
            pAudioBroadcaster->Add(pAudioItem);
            pAudioItem = NULL;
        }
    }
}

void CInteractAnchorDlg::SetControlTag(CControlUI* pControl, int tag)
{
    if (pControl != NULL) {
        pControl->SetTag(tag);
        CContainerUI* pContainer = static_cast<CContainerUI*>(pControl->GetInterface(DUI_CTR_CONTAINER));
        if (pContainer) {
            for (int i = 0; i < pContainer->GetCount(); i++) {
                SetControlTag(pContainer->GetItemAt(i), tag);
            }
        }
    }
}

LRESULT CInteractAnchorDlg::OnToolbarShow(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    int tag = (int)wParam;
    if (!IsAudioMode()) {
        if (tag >= 0 && tag < m_vectVideoWnd.size()) {
            CVideoWnd* pItem = m_vectVideoWnd[tag];
            CContainerUI* pLayoutToolbar = static_cast<CContainerUI*>(pItem->FindSubControl(_T("layout_toolbar")));
            if (pLayoutToolbar) {
                pLayoutToolbar->SetVisible(!pLayoutToolbar->IsVisible());
            }
        }
    }
    else {
        CTileLayoutUI* pAudioBroadcaster = static_cast<CTileLayoutUI*>(FindControl(_T("list_audio_broadcaster")));
        if (tag >= 0 && tag < pAudioBroadcaster->GetCount()) {
            CContainerUI* pAudioItem = static_cast<CContainerUI*>(pAudioBroadcaster->GetItemAt(tag));
            if (pAudioItem) {
                CContainerUI* pLayoutToolbar = static_cast<CContainerUI*>(pAudioItem->FindSubControl(_T("layout_toolbar")));
                if (pLayoutToolbar) {
                    pLayoutToolbar->SetVisible(!pLayoutToolbar->IsVisible());
                }
            }
        }
    }
    return 0;
}

LRESULT CInteractAnchorDlg::OnToolbarKickoutGuest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    int tag = (int)wParam;
    if (tag >= 0 && tag < m_vectVideoUId.size()) {
        InteractImManager::GetInstance().SendCommand(STR2T(m_vectVideoUId[tag]), ImConstant::CMD_ANCHOR_KICKOUT_GUEST, _T(""));
        QHVC::INTERACT::QHVCInteract::RemoveRemoteVideo("", m_vectVideoUId[tag].c_str());
        DelayRefresh(200);
    }

    return 0;
}

LRESULT CInteractAnchorDlg::OnToolbarMuteVideo(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    int tag = (int)wParam;
    bool mute = (bool)lParam;
    if (tag >= 0 && tag < m_vectVideoUId.size()) {
        std::string myUserId = STR2A(InteractGlobalManager::GetInstance().GetInteractUserId());

        if (stricmp(myUserId.c_str(), m_vectVideoUId[tag].c_str()) == 0) {
            QHVC::INTERACT::QHVCInteract::MuteLocalVideoStream(mute);
        }
        else {
            QHVC::INTERACT::QHVCInteract::MuteRemoteVideoStream(m_vectVideoUId[tag].c_str(), mute);
        }

        if (mute) {
            m_setMuteVideo.insert(m_vectVideoUId[tag]);
        }
        else {
            std::set<std::string>::iterator it = m_setMuteVideo.find(m_vectVideoUId[tag]);
            if (it != m_setMuteVideo.end()) {
                m_setMuteVideo.erase(it);
            }
        }
    }

    return 0;
}

LRESULT CInteractAnchorDlg::OnToolbarMuteAudio(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    int tag = (int)wParam;
    bool mute = (bool)lParam;
    if (tag >= 0 && tag < m_vectVideoUId.size()) {
        std::string myUserId = STR2A(InteractGlobalManager::GetInstance().GetInteractUserId());

        if (stricmp(myUserId.c_str(), m_vectVideoUId[tag].c_str()) == 0) {
            QHVC::INTERACT::QHVCInteract::MuteLocalAudioStream(mute);
        }
        else {
            QHVC::INTERACT::QHVCInteract::MuteRemoteAudioStream(m_vectVideoUId[tag].c_str(), mute);
        }

        if (mute) {
            m_setMuteAudio.insert(m_vectVideoUId[tag]);
        }
        else {
            std::set<std::string>::iterator it = m_setMuteAudio.find(m_vectVideoUId[tag]);
            if (it != m_setMuteAudio.end()) {
                m_setMuteAudio.erase(it);
            }
        }
    }

    return 0;
}

LRESULT CInteractAnchorDlg::OnToolbarMuteMicro(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    int tag = (int)wParam;
    bool mute = (bool)lParam;
    if (tag >= 0 && tag < m_vectVideoUId.size()) {
        std::string myUserId = STR2A(InteractGlobalManager::GetInstance().GetInteractUserId());

        if (stricmp(myUserId.c_str(), m_vectVideoUId[tag].c_str()) == 0) {
            QHVC::INTERACT::QHVCInteract::MuteLocalAudioStream(mute);
        }
        else {
            QHVC::INTERACT::QHVCInteract::MuteRemoteAudioStream(m_vectVideoUId[tag].c_str(), mute);
        }

        if (mute) {
            m_setMuteAudio.insert(m_vectVideoUId[tag]);
        }
        else {
            std::set<std::string>::iterator it = m_setMuteAudio.find(m_vectVideoUId[tag]);
            if (it != m_setMuteAudio.end()) {
                m_setMuteAudio.erase(it);
            }
        }
    }

    return 0;
}

void CInteractAnchorDlg::DelayRefresh(int delay)
{
    // 定时
    KillTimer(GetHWND(), TIMER_ID_HEART);

    SetTimer(GetHWND(), TIMER_ID_HEART, delay, NULL);
}

void CInteractAnchorDlg::DoLeaveChannel()
{
    if (mUserIdentity == InteractConstant::USER_IDENTITY_ANCHOR) {
        InteractImManager::GetInstance().SendNotify(ImConstant::CMD_ANCHOR_QUIT_NOTIFY, _T(""));
    }
    else if (mUserIdentity == InteractConstant::USER_IDENTITY_GUEST) {
        InteractImManager::GetInstance().SendNotify(ImConstant::CMD_GUEST_QUIT_NOTIFY, _T(""));
    }
    else if (mUserIdentity == InteractConstant::USER_IDENTITY_AUDIENCE) {
        InteractImManager::GetInstance().SendNotify(ImConstant::CMD_AUDIENT_QUIT_NOTIFY, _T(""));
    }


    tstring userId = InteractGlobalManager::GetInstance().GetInteractUserId();

    // demo离开房间
    if (_tcsicmp(userId.c_str(), mInteractRoomModel.bindRoleId.c_str()) == 0) {
        InteractServerApi::GetInstance().DismissRoom(userId, mInteractRoomModel.roomId);
    }
    else {
        InteractServerApi::GetInstance().UserLeaveRoom(userId, mInteractRoomModel.roomId);
    }

    // IM离开房间
    LeaveImRoom();

    // 关闭窗口
    Close();
}

LRESULT CInteractAnchorDlg::OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    if (wParam == SIZE_RESTORED) {
        ::InvalidateRect(GetHWND(), NULL, TRUE);
        ::UpdateWindow(GetHWND());
    }

    return 0;
}

LRESULT CInteractAnchorDlg::OnGuestAskJoin(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    while (m_vectAskJoin.size() > 0) {
        tstring userId = m_vectAskJoin[0];

        tstring msg = userId + _T(" ");

        msg.append(_T("请求加入房间, 是否同意"));

        if (IsWindowVisible(m_hWnd)) {
            int ret = DuiMessageBox(m_hWnd, msg.c_str(), INTERACT_TITLE, MB_YESNO);
            if (ret == IDYES) {
                InteractImManager::GetInstance().SendCommand(userId, ImConstant::CMD_ANCHOR_AGREE_JOIN, _T(""));
            }
            else {
                InteractImManager::GetInstance().SendCommand(userId, ImConstant::CMD_ANCHOR_REFUSE_JOIN, _T(""));
            }
        }

        m_vectAskJoin.erase(m_vectAskJoin.begin());
    }

    return 0;
}