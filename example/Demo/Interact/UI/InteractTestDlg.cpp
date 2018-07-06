#include "stdafx.h"
#include "InteractTestDlg.h"
#include "../InteractGlobalManager.h"
#include "../DebugSetting.h"


DUI_BEGIN_MESSAGE_MAP(CInteractTestDlg, CDuiDialogImpl<CInteractTestDlg>)
DUI_ON_MSGTYPE(DUI_MSGTYPE_CLICK, OnClick)
DUI_END_MESSAGE_MAP()

CInteractTestDlg::CInteractTestDlg()
{
    m_bAllVideoMute = false;
    m_bAllAudioMute = false;
    m_bAudioVolumeIndication = false;
    m_bDualStreamMode = false;
    m_bRomoteStreamLow = false;
    m_bEnableAudio = false;
    m_bEnableVideo = true;
}

CInteractTestDlg::~CInteractTestDlg()
{

}

LRESULT CInteractTestDlg::OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    return 0;
}

LRESULT CInteractTestDlg::OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    return 0;
}

void CInteractTestDlg::OnClick(TNotifyUI& msg)
{
    CDuiString sCtrlName = msg.pSender->GetName();

    if (sCtrlName == _T("SetCloudControlRole"))
    {
        tstring role = DebugSetting::GetInstance().GetCloudRole();
        if (!role.empty()) {
            QHVC::INTERACT::QHVCInteract::SetCloudControlRole(STR2A(role).c_str());
        }
        SetTextInfo(_T("SetCloudControlRole:") + role);
    }
    else if (sCtrlName == _T("MuteAllRemoteVideoStreams"))
    {
        m_bAllVideoMute = !m_bAllVideoMute;
        SetTextInfo(_T("MuteAllRemoteVideoStreams"), m_bAllVideoMute);
        QHVC::INTERACT::QHVCInteract::MuteAllRemoteVideoStreams(m_bAllVideoMute);
    }
    else if (sCtrlName == _T("MuteAllRemoteAudioStreams"))
    {
        m_bAllAudioMute = !m_bAllAudioMute;
        SetTextInfo(_T("MuteAllRemoteAudioStreams"), m_bAllAudioMute);

        QHVC::INTERACT::QHVCInteract::MuteAllRemoteVideoStreams(m_bAllAudioMute);
    }
    else if (sCtrlName == _T("EnableAudioVolumeIndication"))
    {
        m_bAudioVolumeIndication = !m_bAudioVolumeIndication;
        int interval = m_bAudioVolumeIndication ? 1000 : 0;

        SetTextInfo(_T("EnableAudioVolumeIndication"), m_bAudioVolumeIndication);

        QHVC::INTERACT::QHVCInteract::EnableAudioVolumeIndication(interval, 10);
    }
    else if (sCtrlName == _T("EnableDualStreamMode"))
    {
        m_bDualStreamMode = !m_bDualStreamMode;

        SetTextInfo(_T("EnableDualStreamMode"), m_bDualStreamMode);

        QHVC::INTERACT::QHVCInteract::EnableDualStreamMode(m_bDualStreamMode);
    }
    else if (sCtrlName == _T("SetLowStreamVideoProfile"))
    {
        SetTextInfo(_T("SetLowStreamVideoProfile:"));
        QHVC::INTERACT::QHVCInteract::SetLowStreamVideoProfile(320, 180, 15, 200);
    }
    else if (sCtrlName == _T("SetRemoteVideoStream"))
    {
        m_bRomoteStreamLow = !m_bRomoteStreamLow;
        QHVC::INTERACT::VIDEO_STREAM_LEVEL level = QHVC::INTERACT::VIDEO_STREAM_LEVEL_HIGH;
        if (m_bRomoteStreamLow) {
            level = QHVC::INTERACT::VIDEO_STREAM_LEVEL_LOW;
        }
        else {
            level = QHVC::INTERACT::VIDEO_STREAM_LEVEL_HIGH;
        }

        SetTextInfo(_T("SetRemoteVideoStream"), m_bRomoteStreamLow);

        std::string userId = STR2A(InteractGlobalManager::GetInstance().GetInteractUserId());
        for (int i = 0; i < m_vectAnchorID.size(); i++) {
            if (strcmp(userId.c_str(), m_vectAnchorID[i].c_str()) != 0) {
                QHVC::INTERACT::QHVCInteract::SetRemoteVideoStream(m_vectAnchorID[i].c_str(), level);
            }
        }
    }
    else if (sCtrlName == _T("EnableAudio"))
    {
        m_bEnableAudio = true;
        SetTextInfo(_T("EnableAudio"), m_bEnableAudio);
        QHVC::INTERACT::QHVCInteract::EnableAudio();
    }
    else if (sCtrlName == _T("DisableAudio"))
    {
        m_bEnableAudio = false;
        SetTextInfo(_T("DisableAudio"), m_bEnableAudio);
        QHVC::INTERACT::QHVCInteract::DisableAudio();
    }
    else if (sCtrlName == _T("EnableVideo"))
    {
        m_bEnableVideo = true;
        SetTextInfo(_T("EnableVideo"), m_bEnableVideo);
        QHVC::INTERACT::QHVCInteract::EnableVideo();
    }
    else if (sCtrlName == _T("DisableVideo"))
    {
        m_bEnableVideo = false;
        SetTextInfo(_T("DisableVideo"), m_bEnableVideo);
        QHVC::INTERACT::QHVCInteract::DisableVideo();
    }
    else if (sCtrlName == _T("ClearVideoCompositingLayout"))
    {
        SetTextInfo(_T("ClearVideoCompositingLayout"));
        QHVC::INTERACT::QHVCInteract::ClearVideoCompositingLayout();
    }

    Super::OnClick(msg);
}

void CInteractTestDlg::SetAnchorIDList(const std::vector<std::string >& vectAnchorID)
{
    m_vectAnchorID = vectAnchorID;
}

void CInteractTestDlg::SetInteractRoomModel(const InteractRoomModel& interactRoomModel)
{
    mInteractRoomModel = interactRoomModel;
}

void CInteractTestDlg::SetTextInfo(const tstring& info, bool bValue)
{
    tstring log = info;
    if (bValue) {
        log.append(_T(":true"));
    }
    else {
        log.append(_T(":false"));
    }

    CLabelUI* pLabelInfo = static_cast<CLabelUI*>(FindControl(_T("lb_text_info")));
    if (pLabelInfo) {
        pLabelInfo->SetText(log.c_str());
    }
}

void CInteractTestDlg::SetTextInfo(const tstring& info)
{
    CLabelUI* pLabelInfo = static_cast<CLabelUI*>(FindControl(_T("lb_text_info")));
    if (pLabelInfo) {
        pLabelInfo->SetText(info.c_str());
    }
}
