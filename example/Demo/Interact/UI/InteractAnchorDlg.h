#pragma once
#include "../../UI/UI.h"
#include "./Control/VideoWnd.h"
#include "../Net/InteractServerApi.h"
#include "../IM/InteractImManager.h"
#include "InteractUIConstant.h"
#include <set>

enum
{
    WM_INTERACT_ON_WARNING = (WM_USER + 0x100),
    WM_INTERACT_ON_ERROR,
    WM_INTERACT_ON_LOAD_ENGINE_SUCCESS,
    WM_INTERACT_ON_JOIN_CHANNEL_SUCCESS,
    WM_INTERACT_ON_REJOIN_CHANNEL_SUCCESS,
    WM_INTERACT_ON_LEAVE_CHANNEL,
    WM_INTERACT_ON_CHANGE_CLIENT_ROLE,
    WM_INTERACT_ON_CONNECT_INTERRUPTED,
    WM_INTERACT_ON_CONNECT_LOST,
    WM_INTERACT_ON_USER_JOIN,
    WM_INTERACT_ON_USER_OFFLINE,
    WM_INTERACT_ON_USER_ENABLE_VIDEO,

    WM_INTERACT_ON_USER_MUTE_AUDIO,
    WM_INTERACT_ON_USER_MUTE_VIDEO,
    WM_INTERACT_ON_CAMERA_READY,
    WM_INTERACT_ON_VIDEO_STOP,

    WM_INTERACT_ON_FIRST_LOCAL_VIDEO_FRAME,
    WM_INTERACT_ON_FIRST_REMOTE_VIDEO_FRAME,
    WM_INTERACT_ON_FIRST_REMOTE_VIDEO_DECODE,

    WM_INTERACT_ON_LOCAL_VIDEO_STATS,
    WM_INTERACT_ON_REMOTE_VIDEO_STATS,
    WM_INTERACT_ON_AUDIO_QUALITY,
    WM_INTERACT_ON_AUDIO_VOLUME_INDICATION,
    WM_INTERACT_ON_RTC_STATS,
    WM_INTERACT_ON_NETWORK_QUALITY,

    WM_INTERACT_ON_AUDIO_DEVICE_CHANGE,
    WM_INTERACT_ON_VIDEO_DEVICE_CHANGE,

    WM_INTERACT_ON_AUDIO_MIXING_FINISH,
    WM_INTERACT_ON_REMOTE_AUDIO_MIXING_BEGIN,
    WM_INTERACT_ON_REMOTE_AUDIO_MIXING_END,

    WM_INTERACT_ON_AUDIO_EFFECT_END,
};

class CInteractInfoPanel;
class QHVCInteractCallbackImpl;
class CInteractAnchorDlg : public CDuiDialogImpl < CInteractAnchorDlg >, InteractServerApiCallBack, InteractImCallback, IListCallbackUI
{
    friend QHVCInteractCallbackImpl;
public:
    CInteractAnchorDlg();
    ~CInteractAnchorDlg();

public:
    void SetInteractRoomModel(const InteractRoomModel& interactRoomModel);
    void SetInteractUserIdentity(InteractConstant::USER_IDENTITY userIdentity);

public:
    DUI_DECLARE_WND_CLASS(_T("InteractAnchorDlg"));
    DUI_DECLARE_SKIN_FILE(_T("InteractAnchorDlg.xml"));
    DUI_DECLARE_MESSAGE_MAP();

    DUI_WIN_BEGIN_MSG_MAP(CInteractAnchorDlg, CDuiDialogImpl<CInteractAnchorDlg>)
        DUI_WIN_MESSAGE_HANDLER(WM_CREATE, OnCreate)
        DUI_WIN_MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
        DUI_WIN_MESSAGE_HANDLER(WM_TIMER, OnTimer)
        DUI_WIN_MESSAGE_HANDLER(WM_SIZE, OnSize)


        DUI_WIN_MESSAGE_HANDLER(WM_INTERACT_ON_WARNING, OnWarning)
        DUI_WIN_MESSAGE_HANDLER(WM_INTERACT_ON_ERROR, OnError)
        DUI_WIN_MESSAGE_HANDLER(WM_INTERACT_ON_LOAD_ENGINE_SUCCESS, OnLoadEngineSuccess)
        DUI_WIN_MESSAGE_HANDLER(WM_INTERACT_ON_JOIN_CHANNEL_SUCCESS, OnJoinChannelSuccess)
        DUI_WIN_MESSAGE_HANDLER(WM_INTERACT_ON_REJOIN_CHANNEL_SUCCESS, OnRejoinChannelSuccess)
        DUI_WIN_MESSAGE_HANDLER(WM_INTERACT_ON_LEAVE_CHANNEL, OnLeaveChannel)
        DUI_WIN_MESSAGE_HANDLER(WM_INTERACT_ON_CHANGE_CLIENT_ROLE, OnChangeClientRoleSuccess)
        DUI_WIN_MESSAGE_HANDLER(WM_INTERACT_ON_CONNECT_INTERRUPTED, OnConnectionInterrupted)
        DUI_WIN_MESSAGE_HANDLER(WM_INTERACT_ON_CONNECT_LOST, OnConnectionLost)
        DUI_WIN_MESSAGE_HANDLER(WM_INTERACT_ON_USER_JOIN, OnUserJoined)
        DUI_WIN_MESSAGE_HANDLER(WM_INTERACT_ON_USER_OFFLINE, OnUserOffline)
        DUI_WIN_MESSAGE_HANDLER(WM_INTERACT_ON_USER_ENABLE_VIDEO, OnUserEnableVideo)

        DUI_WIN_MESSAGE_HANDLER(WM_INTERACT_ON_USER_MUTE_AUDIO, OnUserMuteAudio)
        DUI_WIN_MESSAGE_HANDLER(WM_INTERACT_ON_USER_MUTE_VIDEO, OnUserMuteVideo)
        DUI_WIN_MESSAGE_HANDLER(WM_INTERACT_ON_CAMERA_READY, OnCameraReady)
        DUI_WIN_MESSAGE_HANDLER(WM_INTERACT_ON_VIDEO_STOP, OnVideoStopped)

        DUI_WIN_MESSAGE_HANDLER(WM_INTERACT_ON_FIRST_LOCAL_VIDEO_FRAME, OnFirstLocalVideoFrame)
        DUI_WIN_MESSAGE_HANDLER(WM_INTERACT_ON_FIRST_REMOTE_VIDEO_FRAME, OnFirstRemoteVideoFrame)
        DUI_WIN_MESSAGE_HANDLER(WM_INTERACT_ON_FIRST_REMOTE_VIDEO_DECODE, OnFirstRemoteVideoDecoded)

        DUI_WIN_MESSAGE_HANDLER(WM_INTERACT_ON_LOCAL_VIDEO_STATS, OnLocalVideoStats)
        DUI_WIN_MESSAGE_HANDLER(WM_INTERACT_ON_REMOTE_VIDEO_STATS, OnRemoteVideoStats)
        DUI_WIN_MESSAGE_HANDLER(WM_INTERACT_ON_AUDIO_QUALITY, OnAudioQuality)
        DUI_WIN_MESSAGE_HANDLER(WM_INTERACT_ON_AUDIO_VOLUME_INDICATION, OnAudioVolumeIndication)
        DUI_WIN_MESSAGE_HANDLER(WM_INTERACT_ON_AUDIO_MIXING_FINISH, OnAudioMixingFinished)
        DUI_WIN_MESSAGE_HANDLER(WM_INTERACT_ON_RTC_STATS, OnRtcStats)
        DUI_WIN_MESSAGE_HANDLER(WM_INTERACT_ON_NETWORK_QUALITY, OnNetworkQuality)
        DUI_WIN_MESSAGE_HANDLER(WM_INTERACT_ON_AUDIO_DEVICE_CHANGE, OnAudioDeviceChange)
        DUI_WIN_MESSAGE_HANDLER(WM_INTERACT_ON_VIDEO_DEVICE_CHANGE, OnVideoDeviceChange)


        DUI_WIN_MESSAGE_HANDLER(WM_TOOLBAR_SHOW, OnToolbarShow)
        DUI_WIN_MESSAGE_HANDLER(WM_TOOLBAR_KICKOUT_GUEST, OnToolbarKickoutGuest)
        DUI_WIN_MESSAGE_HANDLER(WM_TOOLBAR_MUTE_VIDEO, OnToolbarMuteVideo)
        DUI_WIN_MESSAGE_HANDLER(WM_TOOLBAR_MUTE_AUDIO, OnToolbarMuteAudio)
        DUI_WIN_MESSAGE_HANDLER(WM_TOOLBAR_MUTE_MICRO, OnToolbarMuteMicro)
        DUI_WIN_MESSAGE_HANDLER(WM_TOOLBAR_SCREEN, OnToolbarScreen)
        DUI_WIN_MESSAGE_HANDLER(WM_TOOLBAR_AUDIO_MIXING, OnToolbarAudioMixing)

        DUI_WIN_MESSAGE_HANDLER(WM_GUEST_ASK_JOIN, OnGuestAskJoin)
        DUI_WIN_END_MSG_MAP()

private:
    LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT OnTimer(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

public:
    virtual LRESULT HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

    virtual CControlUI* CreateControl(LPCTSTR pstrClass);
    virtual void OnClick(TNotifyUI& msg);

private:
    void JoinImRoom();
    void LeaveImRoom();

private:
    virtual void OnServerApiJoinRoomResult(const InteractServerApiRet& ret, const InteractRoomModel& interactRoomModel);

    virtual void OnServerApiUserLeaveRoomResult(const InteractServerApiRet& ret, const InteractRoomModel& interactRoomModel);

    virtual void OnServerApiGetRoomInfoResult(const InteractServerApiRet& ret, const InteractRoomModel& interactRoomModel);

    virtual void OnServerApiGetRoomUserListResult(const InteractServerApiRet& ret, const std::vector<InteractUserModel>& interactUserModelList);

    virtual void OnServerApiChangeUserdentityResult(const InteractServerApiRet& ret, const InteractRoomModel& interactRoomModel);;

    virtual void OnServerApiDismissRoomResult(const InteractServerApiRet& ret);

    virtual void OnServerApiUserHeartResult(const InteractServerApiRet& ret);
public:
    virtual void OnImConnectStatusChanged(ImConstant::CONNECT_STATUS connectStatus);

    virtual void OnImSendMessageSuccess();

    virtual void OnImSendMessageFailed(int errCode, const std::string& errMessage);

    virtual void OnImJoinRoomSuccess(const std::string& roomId);

    virtual void OnImJoinRoomFailed(int errCode, const std::string& errMessage);

    virtual void OnImReceiveCommand(const InteractUserModel& userFrom, const CommandInfo& command);

    virtual void OnImReceiveOtherCommand(const InteractUserModel& userFrom, const std::string& str);


private:
    LRESULT OnWarning(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

    LRESULT OnError(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

    LRESULT OnLoadEngineSuccess(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

    LRESULT OnJoinChannelSuccess(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

    LRESULT OnRejoinChannelSuccess(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

    LRESULT OnLeaveChannel(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

    LRESULT OnChangeClientRoleSuccess(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

    LRESULT OnConnectionInterrupted(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

    LRESULT OnConnectionLost(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

    LRESULT OnUserJoined(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

    LRESULT OnUserOffline(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

    LRESULT OnUserEnableVideo(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

    LRESULT OnUserMuteAudio(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT OnUserMuteVideo(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT OnCameraReady(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT OnVideoStopped(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

    LRESULT OnFirstLocalVideoFrame(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

    LRESULT OnFirstRemoteVideoFrame(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

    LRESULT OnFirstRemoteVideoDecoded(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

    LRESULT OnLocalVideoStats(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT OnRemoteVideoStats(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT OnAudioQuality(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT OnAudioVolumeIndication(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT OnRtcStats(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT OnNetworkQuality(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT OnAudioDeviceChange(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT OnVideoDeviceChange(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT OnAudioMixingFinished(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT OnRemoteAudioMixingBegin(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT OnRemoteAudioMixingEnd(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

    LRESULT OnToolbarShow(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT OnToolbarKickoutGuest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT OnToolbarMuteVideo(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT OnToolbarMuteAudio(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT OnToolbarMuteMicro(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT OnToolbarScreen(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT OnToolbarAudioMixing(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

    LRESULT OnGuestAskJoin(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
private:
    void RefreshUserList();
    void RefreshRoomModel();
    void UpdateMixStreamInfo();
    void UpdateVideoList();
    void UpdateAudioList();

    void UpdateItem(void* pItem, int nIdx, int nCount);

    void AddUIdToList(const std::string& uid);
    void RemoveUIdFromList(const std::string& uid);
    void RebuildUIdList(const std::vector<InteractUserModel>& vectUserModel);

    void AddLog(const tstring & logInfo);

    void UpdateVideoProfile();

    bool IsAudioMode();
    void InitAudioList();

    void SetControlTag(CControlUI* pControl, int tag);

    void DelayRefresh(int delay);

    void DoLeaveChannel();


private:
    virtual LPCTSTR GetItemText(CControlUI* pList, int iItem, int iSubItem);

private:
    std::vector<CVideoWnd*> m_vectVideoWnd;

    InteractRoomModel mInteractRoomModel;

    InteractConstant::USER_IDENTITY mUserIdentity;
    InteractConstant::USER_IDENTITY mTargetUserIdentity;

    QHVCInteractCallbackImpl* m_pQHVCInteractCallback;

    CInteractInfoPanel* m_pInfoPanel;

    bool m_bBroadcasterCountChange;
    std::vector<std::string> m_vectVideoUId;

    std::vector<tstring> m_vectLog;

    tstring m_strMergeRtmp;

    bool m_bShowExitDailog;

    InteractStats mInteractStats;

    bool m_bAddHeartLog;
    bool m_bAddRoomInfoLog;
    bool m_bAddRoomUserListLog;

    std::set<std::string> m_setMuteVideo;
    std::set<std::string> m_setMuteAudio;

    bool m_bJoinChannelSuccess;
    bool m_bEngineFatalError;
    bool m_bEngineLoadOk;

    std::vector<tstring> m_vectAskJoin;
};