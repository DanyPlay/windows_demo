#include "stdafx.h"
#include "InteractInfoPanel.h"
#include "../IM/InteractImManager.h"
#include "../InteractGlobalManager.h"
#include "InteractUIConstant.h"
#include "../InteractSetting.h"
#include "Toast.h"

DUI_BEGIN_MESSAGE_MAP(CInteractInfoPanel, CDuiDialogImpl<CInteractInfoPanel>)
DUI_ON_MSGTYPE(DUI_MSGTYPE_CLICK, OnClick)
DUI_ON_MSGTYPE(DUI_MSGTYPE_LINK, OnItemLink)
DUI_ON_MSGTYPE(DUI_MSGTYPE_SELECTCHANGED, OnSelectChanged)
DUI_ON_MSGTYPE(DUI_MSGTYPE_ITEMSELECT, OnItemSelect)
DUI_ON_MSGTYPE(DUI_MSGTYPE_VALUECHANGED, OnValueChange)
DUI_END_MESSAGE_MAP()

CInteractInfoPanel::CInteractInfoPanel()
{
    m_bMuteLocalVideoStream = false;
    mUserIdentity = InteractConstant::USER_IDENTITY_AUDIENCE;
    m_bShowNetworkInfo = false;
    m_dwUpdateLogTickCount = 0;
    m_bInitControlStatus = false;
}

CInteractInfoPanel::~CInteractInfoPanel()
{

}

LRESULT CInteractInfoPanel::OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    CListUI* pUserList = static_cast<CListUI*>(FindControl(_T("list_user")));
    if (pUserList) {
        pUserList->SetTextCallback(this);
    }

    CListUI* pRoomInfoList = static_cast<CListUI*>(FindControl(_T("list_room")));
    if (pRoomInfoList) {
        pRoomInfoList->SetTextCallback(this);
    }


    InitDevicInfo();
    return 0;
}

LRESULT CInteractInfoPanel::OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    return 0;
}

void CInteractInfoPanel::OnClick(TNotifyUI& msg)
{
    CDuiString sCtrlName = msg.pSender->GetName();
    if (sCtrlName == _T("btn_close_video")) {
        if (m_bMuteLocalVideoStream) {
            QHVC::INTERACT::QHVCInteract::EnableLocalVideo(true);
            m_bMuteLocalVideoStream = false;
        }
        else {
            QHVC::INTERACT::QHVCInteract::EnableLocalVideo(false);
            m_bMuteLocalVideoStream = true;
        }

        UpdateVideoControl();
    }
    else if (sCtrlName == _T("btn_close_audio")) {
        bool mute = false;
        QHVC::INTERACT::QHVCInteract::GetAudioDeviceMute(QHVC::INTERACT::AUDIO_DEVICE_OUTPUT, mute);
        QHVC::INTERACT::QHVCInteract::SetAudioDeviceMute(QHVC::INTERACT::AUDIO_DEVICE_OUTPUT, !mute);
        UpdateAudioControl();
    }
    else if (sCtrlName == _T("btn_close_micro")) {
        bool mute = false;
        QHVC::INTERACT::QHVCInteract::GetAudioDeviceMute(QHVC::INTERACT::AUDIO_DEVICE_INPUT, mute);
        QHVC::INTERACT::QHVCInteract::SetAudioDeviceMute(QHVC::INTERACT::AUDIO_DEVICE_INPUT, !mute);
        UpdateMicroControl();
    }
    else if (sCtrlName == _T("btn_join")) {
        InteractImManager::GetInstance().SendCommand(mInteractRoomModel.bindRoleId, ImConstant::CMD_GUEST_ASK_JOIN, _T(""));
        msg.pSender->SetEnabled(false);
        DuiShowToast(::GetParent(GetHWND()), _T("���뻥�������ѷ���"), 3);
    }
    else if (sCtrlName == _T("btn_refresh")) {
        InitDevicInfo();
    }
}

void CInteractInfoPanel::OnItemLink(TNotifyUI& msg)
{

    CListTextElementUI * pListTextElement = dynamic_cast<CListTextElementUI *>(msg.pSender);

    if (pListTextElement != NULL) {
        // TODO �жϲ��Ͻ�
        if (msg.wParam == 0) {
            int tag = msg.pSender->GetTag();
            if (tag >= 0 && tag < m_vectAnchor.size()) {
                ::PostMessage(::GetParent(GetHWND()), WM_TOOLBAR_KICKOUT_GUEST, (WPARAM)tag, NULL);
            }
        }
    }
}

void CInteractInfoPanel::OnSelectChanged(TNotifyUI& msg)
{
    CDuiString sCtrlName = msg.pSender->GetName();
    if (sCtrlName == _T("opt_room_info"))
    {
        CControlUI* pLayoutRoomInfo = FindControl(_T("layout_room_info"));
        if (pLayoutRoomInfo != NULL) {
            pLayoutRoomInfo->SetVisible(true);
        }

        CControlUI* pLayoutDebugInfo = FindControl(_T("layout_debug_info"));
        if (pLayoutDebugInfo != NULL) {
            pLayoutDebugInfo->SetVisible(false);
        }
    }
    else if (sCtrlName == _T("opt_debug_info"))
    {
        CControlUI* pLayoutRoomInfo = FindControl(_T("layout_room_info"));
        if (pLayoutRoomInfo != NULL) {
            pLayoutRoomInfo->SetVisible(false);
        }

        CControlUI* pLayoutDebugInfo = FindControl(_T("layout_debug_info"));
        if (pLayoutDebugInfo != NULL) {
            pLayoutDebugInfo->SetVisible(true);
        }
    }
    else if (sCtrlName == _T("opt_debug_room_info")) {
        m_bShowNetworkInfo = false;
        m_dwUpdateLogTickCount = 0;
    }
    else if (sCtrlName == _T("opt_debug_net_info")) {
        m_bShowNetworkInfo = true;
        m_dwUpdateLogTickCount = 0;
    }
}

void CInteractInfoPanel::OnItemSelect(TNotifyUI& msg)
{
    CDuiString sCtrlName = msg.pSender->GetName();

    if (sCtrlName == _T("cmb_video_list") || sCtrlName == _T("cmb_audio_list") || sCtrlName == _T("cmb_micro_list")) {
        CComboUI* pCombo = static_cast<CComboUI*>(msg.pSender);
        int nIdx = pCombo->GetCurSel();

        if (sCtrlName == _T("cmb_video_list")) {
            if (nIdx >= 0 && nIdx < m_vectVideoDevicInfo.size()) {
                QHVC::INTERACT::QHVCInteract::SetVideoDevice(m_vectVideoDevicInfo[nIdx].strDeviceId.c_str());
            }
        }
        else if (sCtrlName == _T("cmb_audio_list")) {
            if (nIdx >= 0 && nIdx < m_vectAudioDevicInfo.size()) {
                QHVC::INTERACT::QHVCInteract::SetAudioDevice(QHVC::INTERACT::AUDIO_DEVICE_OUTPUT, m_vectAudioDevicInfo[nIdx].strDeviceId.c_str());
                UpdateAudioControl();
            }
        }
        else if (sCtrlName == _T("cmb_micro_list")) {
            if (nIdx >= 0 && nIdx < m_vectMicroDevicInfo.size()) {
                QHVC::INTERACT::QHVCInteract::SetAudioDevice(QHVC::INTERACT::AUDIO_DEVICE_INPUT, m_vectMicroDevicInfo[nIdx].strDeviceId.c_str());
                UpdateMicroControl();
            }
        }
    }

}

void CInteractInfoPanel::OnValueChange(TNotifyUI& msg)
{
    CDuiString sCtrlName = msg.pSender->GetName();
    int nIdx = (int)msg.pSender->GetTag();
    if (sCtrlName == _T("sld_audio") || sCtrlName == _T("sld_micro")) {
        CSliderUI* pSlider = static_cast<CSliderUI*>(msg.pSender);
        if (pSlider) {
            int value = pSlider->GetValue();
            if (sCtrlName == _T("sld_audio")) {
                QHVC::INTERACT::QHVCInteract::SetAudioDeviceVolume(QHVC::INTERACT::AUDIO_DEVICE_OUTPUT, value);
                CLabelUI* pLabel = static_cast<CLabelUI*>(FindControl(_T("txt_audio")));
                if (pLabel) {
                    pLabel->SetText(STR_A2T(Util::String::StrFormat("%d", value)).c_str());
                }
            }
            else {
                QHVC::INTERACT::QHVCInteract::SetAudioDeviceVolume(QHVC::INTERACT::AUDIO_DEVICE_INPUT, value);
                CLabelUI* pLabel = static_cast<CLabelUI*>(FindControl(_T("txt_micro")));
                if (pLabel) {
                    pLabel->SetText(STR_A2T(Util::String::StrFormat("%d", value)).c_str());
                }
            }
        }
    }
}


void CInteractInfoPanel::InitDevicInfo()
{
    int count = 0;
    QHVC::INTERACT::DeviceInfo* pVideoDeviceInfos = QHVC::INTERACT::QHVCInteract::GetVideoDeviceList(count);
    m_vectVideoDevicInfo.clear();
    for (int i = 0; i < count; i++) {
        InteractDeviceInfo info;
        info.strDeviceId = pVideoDeviceInfos[i].szDeviceId;
        info.strDeviceName = pVideoDeviceInfos[i].szDeviceName;
        m_vectVideoDevicInfo.push_back(info);
    }
    QHVC::INTERACT::QHVCInteract::FreeDeviceList(pVideoDeviceInfos);

    count = 0;
    QHVC::INTERACT::DeviceInfo* pAudioDeviceInfos = QHVC::INTERACT::QHVCInteract::GetAudioDeviceList(QHVC::INTERACT::AUDIO_DEVICE_OUTPUT, count);
    m_vectAudioDevicInfo.clear();
    for (int i = 0; i < count; i++) {
        InteractDeviceInfo info;
        info.strDeviceId = pAudioDeviceInfos[i].szDeviceId;
        info.strDeviceName = pAudioDeviceInfos[i].szDeviceName;
        m_vectAudioDevicInfo.push_back(info);
    }
    QHVC::INTERACT::QHVCInteract::FreeDeviceList(pAudioDeviceInfos);

    count = 0;
    QHVC::INTERACT::DeviceInfo* pMicroDeviceInfos = QHVC::INTERACT::QHVCInteract::GetAudioDeviceList(QHVC::INTERACT::AUDIO_DEVICE_INPUT, count);
    m_vectMicroDevicInfo.clear();
    for (int i = 0; i < count; i++) {
        InteractDeviceInfo info;
        info.strDeviceId = pMicroDeviceInfos[i].szDeviceId;
        info.strDeviceName = pMicroDeviceInfos[i].szDeviceName;
        m_vectMicroDevicInfo.push_back(info);
    }
    QHVC::INTERACT::QHVCInteract::FreeDeviceList(pMicroDeviceInfos);

    char device[256] = { 0 };
    memset(device, 0, 256);
    QHVC::INTERACT::QHVCInteract::GetVideoDevice(device, 255);
    std::string strCurrentVideo = device;

    memset(device, 0, 256);
    QHVC::INTERACT::QHVCInteract::GetAudioDevice(QHVC::INTERACT::AUDIO_DEVICE_OUTPUT, device, 255);
    std::string strCurrentAudio = device;

    memset(device, 0, 256);
    QHVC::INTERACT::QHVCInteract::GetAudioDevice(QHVC::INTERACT::AUDIO_DEVICE_INPUT, device, 255);
    std::string strCurrentMicro = device;

    FillComboList("cmb_video_list", m_vectVideoDevicInfo, strCurrentVideo);
    FillComboList("cmb_audio_list", m_vectAudioDevicInfo, strCurrentAudio);
    FillComboList("cmb_micro_list", m_vectMicroDevicInfo, strCurrentMicro);


    UpdateAudioControl();
    UpdateMicroControl();
}

void CInteractInfoPanel::UpdateVideoControl()
{
    CButtonUI* pbtnVideo = static_cast<CButtonUI*>(FindControl(_T("btn_close_video")));
    if (pbtnVideo) {
        pbtnVideo->SetText(m_bMuteLocalVideoStream ? _T("��") : _T("�ر�"));
    }
}

void CInteractInfoPanel::UpdateAudioControl()
{
    int volume = 0;
    QHVC::INTERACT::QHVCInteract::GetAudioDeviceVolume(QHVC::INTERACT::AUDIO_DEVICE_OUTPUT, volume);

    bool mute = false;
    QHVC::INTERACT::QHVCInteract::GetAudioDeviceMute(QHVC::INTERACT::AUDIO_DEVICE_OUTPUT, mute);

    CSliderUI* pSliderAudio = static_cast<CSliderUI*>(FindControl(_T("sld_audio")));
    if (pSliderAudio) {
        pSliderAudio->SetValue(volume);
        pSliderAudio->SetEnabled(!mute && m_vectAudioDevicInfo.size() > 0);
    }

    CLabelUI* pLabelAudio = static_cast<CLabelUI*>(FindControl(_T("txt_audio")));
    if (pLabelAudio) {
        pLabelAudio->SetText(STR_A2T(Util::String::StrFormat("%d", volume)).c_str());
    }

    CButtonUI* pbtnAudio = static_cast<CButtonUI*>(FindControl(_T("btn_close_audio")));
    if (pbtnAudio) {
        pbtnAudio->SetText(mute ? _T("��") : _T("�ر�"));
        pbtnAudio->SetEnabled(m_vectAudioDevicInfo.size() > 0);
    }
}

void CInteractInfoPanel::UpdateMicroControl()
{
    int volume = 0;
    QHVC::INTERACT::QHVCInteract::GetAudioDeviceVolume(QHVC::INTERACT::AUDIO_DEVICE_INPUT, volume);

    bool mute = false;
    QHVC::INTERACT::QHVCInteract::GetAudioDeviceMute(QHVC::INTERACT::AUDIO_DEVICE_INPUT, mute);

    CSliderUI* pSliderMicro = static_cast<CSliderUI*>(FindControl(_T("sld_micro")));
    if (pSliderMicro) {
        pSliderMicro->SetValue(volume);
        pSliderMicro->SetEnabled(!mute && m_vectMicroDevicInfo.size() > 0);
    }
    CLabelUI* pLabelMicro = static_cast<CLabelUI*>(FindControl(_T("txt_micro")));
    if (pLabelMicro) {
        pLabelMicro->SetText(STR_A2T(Util::String::StrFormat("%d", volume)).c_str());
    }

    CButtonUI* pbtnMicro = static_cast<CButtonUI*>(FindControl(_T("btn_close_micro")));
    if (pbtnMicro) {
        pbtnMicro->SetText(mute ? _T("��") : _T("�ر�"));
        pbtnMicro->SetEnabled(m_vectMicroDevicInfo.size() > 0);
    }
}

void CInteractInfoPanel::FillComboList(const std::string& comboName, std::vector<InteractDeviceInfo> vectDeviceInfo, const std::string& deviceId)
{
    CComboUI *pCombo = (CComboUI*)FindControl(STR_A2T(comboName).c_str());
    if (pCombo != NULL) {
        pCombo->RemoveAll();
        int selectIdx = -1;
        for (int i = 0; i < vectDeviceInfo.size(); i++) {
            CListLabelElementUI* pItem = new CListLabelElementUI();
            pItem->SetText(STR_A2T(vectDeviceInfo[i].strDeviceName).c_str());
            if (stricmp(vectDeviceInfo[i].strDeviceId.c_str(), deviceId.c_str()) == 0) {
                pItem->SetTag(i);
                selectIdx = i;
            }

            pCombo->Add(pItem);
        }

        if (selectIdx != -1) {
            pCombo->SelectItem(selectIdx);
        }

    }
}

LPCTSTR CInteractInfoPanel::GetItemText(CControlUI* pList, int iItem, int iSubItem)
{
    TCHAR szBuf[MAX_PATH] = { 0 };
    CDuiString sCtrlName = pList->GetParent()->GetParent()->GetName();

    if (pList->GetParent()->GetParent()->GetName() == _T("list_user")) {
        switch (iSubItem)
        {
            case 0: {
                _stprintf(szBuf, _T("   <i icon_broadcaster.png>"), _T(""));
                break;
            }
            case 1: {
                _stprintf(szBuf, _T("%s"), m_vectAnchor[iItem].userid.c_str());
                break;
            }
            case 2: {
                if (m_vectAnchor[iItem].identity == InteractConstant::USER_IDENTITY_ANCHOR) {
                    _stprintf(szBuf, _T("%s"), USER_ANCHOR_NAME);
                }
                else  if (m_vectAnchor[iItem].identity == InteractConstant::USER_IDENTITY_GUEST) {
                    _stprintf(szBuf, _T("%s"), USER_GUEST_NAME);
                }

                break;
            }
            case 3: {
                tstring userId = InteractGlobalManager::GetInstance().GetInteractUserId();
                if ((_tcsicmp(userId.c_str(), mInteractRoomModel.bindRoleId.c_str()) == 0 && _tcsicmp(userId.c_str(), m_vectAnchor[iItem].userid.c_str()) != 0) ||
                    (_tcsicmp(mInteractRoomModel.bindRoleId.c_str(), userId.c_str()) != 0 && _tcsicmp(userId.c_str(), m_vectAnchor[iItem].userid.c_str()) == 0)) {
                    _stprintf(szBuf, _T("%s"), _T("<a><i icon_kickout.png 4 0></a>"));
                }
                break;
            }
        }
    }
    else  if (pList->GetParent()->GetParent()->GetName() == _T("list_room")) {
        if (iItem < m_vectDebugInfo.size()) {
            switch (iSubItem)
            {
                case 0: {
                    _stprintf(szBuf, _T("%s"), m_vectDebugInfo[iItem].key.c_str());
                    break;
                }
                case 1: {
                    _stprintf(szBuf, _T("%s"), m_vectDebugInfo[iItem].value.c_str());
                    break;
                }
            }
        }
    }

    return szBuf;
}


void CInteractInfoPanel::SetAnchorList(const std::vector<InteractUserModel>& vectAnchor)
{
    m_vectAnchor = vectAnchor;

    InteractConstant::USER_IDENTITY userIdentity = InteractConstant::USER_IDENTITY_AUDIENCE;
    tstring userId = InteractGlobalManager::GetInstance().GetInteractUserId();

    for (int i = 0; i < m_vectAnchor.size(); i++)
    {
        if (m_vectAnchor[i].identity == InteractConstant::USER_IDENTITY_ANCHOR) {
            tstring createTime = m_vectAnchor[i].createTime;

            time_t time_current = Util::Time::GetCurrentTime();
            time_t time_begin = Util::Time::StringToDatetime(STR2A(createTime).c_str());

            std::string duration = Util::Time::format_hhmmss(time_current - time_begin);

            CLabelUI* pLabelDuration = static_cast<CLabelUI*>(FindControl(_T("txt_room_druation")));
            if (pLabelDuration) {
                pLabelDuration->SetText(STR2T(duration).c_str());
            }
        }
    }

    CListUI* pUserList = static_cast<CListUI*>(FindControl(_T("list_user")));
    if (pUserList) {
        pUserList->RemoveAll();
        for (int i = 0; i < m_vectAnchor.size(); i++)
        {
            CListTextElementUI* pListElement = new CListTextElementUI;
            pListElement->SetTag(i);
            pListElement->SetMinHeight(25);

            if (_tcsicmp(userId.c_str(), m_vectAnchor[i].userid.c_str()) == 0) {
                userIdentity = (InteractConstant::USER_IDENTITY)m_vectAnchor[i].identity;
            }

            if (pUserList != NULL) {
                pUserList->Add(pListElement);
            }
        }
    }

    if (mUserIdentity != userIdentity || !m_bInitControlStatus) {
        mUserIdentity = userIdentity;
        m_bInitControlStatus = true;
        CContainerUI* pLayoutVideo = static_cast<CContainerUI*>(FindControl(_T("layout_video_info")));
        CContainerUI* pLayoutMicro = static_cast<CContainerUI*>(FindControl(_T("layout_micro_info")));

        CContainerUI* pLayoutUserList = static_cast<CContainerUI*>(FindControl(_T("layout_user_list")));
        CContainerUI* pLayoutJoin = static_cast<CContainerUI*>(FindControl(_T("layout_join")));

        if (userIdentity == InteractConstant::USER_IDENTITY_ANCHOR || userIdentity == InteractConstant::USER_IDENTITY_GUEST) {
            if (pLayoutVideo) {
                pLayoutVideo->SetVisible(!IsAudioMode());
            }

            if (pLayoutMicro) {
                pLayoutMicro->SetVisible(true);
            }

            if (pLayoutUserList) {
                pLayoutUserList->SetVisible(true);
            }

            if (pLayoutJoin) {
                pLayoutJoin->SetVisible(false);
            }

            if (userIdentity == InteractConstant::USER_IDENTITY_GUEST) {
                CButtonUI* pbtn = static_cast<CButtonUI*>(FindControl(_T("btn_join")));
                if (pbtn) {
                    pbtn->SetText(_T("������"));
                    pbtn->SetEnabled(false);
                }
            }
        }
        else {
            if (pLayoutVideo) {
                pLayoutVideo->SetVisible(false);
            }

            if (pLayoutMicro) {
                pLayoutMicro->SetVisible(false);
            }

            if (pLayoutUserList) {
                pLayoutUserList->SetVisible(false);
            }

            if (pLayoutJoin) {
                pLayoutJoin->SetVisible(true);
            }

            CButtonUI* pbtn = static_cast<CButtonUI*>(FindControl(_T("btn_join")));
            if (pbtn) {
                pbtn->SetText(_T("����"));
                pbtn->SetEnabled(true);
            }
        }
        InitDevicInfo();
    }
}

void CInteractInfoPanel::SetInteractRoomModel(const InteractRoomModel& interactRoomModel)
{
    mInteractRoomModel = interactRoomModel;

    CLabelUI* pLabelRoomName = static_cast<CLabelUI*>(FindControl(_T("txt_room_name")));
    if (pLabelRoomName) {
        pLabelRoomName->SetText(mInteractRoomModel.roomName.c_str());
    }

    CLabelUI* pLabelRoomType = static_cast<CLabelUI*>(FindControl(_T("txt_room_type")));
    if (pLabelRoomType) {
        if (interactRoomModel.talkType == InteractConstant::ROOM_TYPE_ANCHOR_AND_ANCHOR) {
            pLabelRoomType->SetText(ROOM_MODES[0]);
        }
        else if (interactRoomModel.talkType == InteractConstant::ROOM_TYPE_ANCHOR_AND_GUEST) {
            pLabelRoomType->SetText(ROOM_MODES[1]);
        }
        else if (interactRoomModel.talkType == InteractConstant::ROOM_TYPE_HOME_PARTY) {
            pLabelRoomType->SetText(ROOM_MODES[2]);
        }
    }
    CLabelUI* pLabelRoomUserCount = static_cast<CLabelUI*>(FindControl(_T("txt_room_user_count")));
    if (pLabelRoomUserCount) {
        pLabelRoomUserCount->SetText(STR2T(Util::String::StrFormat("<i icon_online.png> %d", mInteractRoomModel.onlineNum)).c_str());
    }

    CLabelUI* pLabelRoomId = static_cast<CLabelUI*>(FindControl(_T("txt_room_id")));
    if (pLabelRoomId) {
        pLabelRoomId->SetText(mInteractRoomModel.roomId.c_str());
    }

    tstring userId = InteractGlobalManager::GetInstance().GetInteractUserId();
    if (_tcsicmp(userId.c_str(), mInteractRoomModel.bindRoleId.c_str()) == 0) {
        CControlUI* pJoin = FindControl(_T("btn_join"));
        if (pJoin != NULL) {
            pJoin->SetVisible(false);
        }
        else {
            pJoin->SetVisible(true);
        }
    }
}

void CInteractInfoPanel::SetInteractStats(const InteractStats& mInteractStats)
{
    if (GetTickCount() - m_dwUpdateLogTickCount < 1000) {
        return;
    }
    m_dwUpdateLogTickCount = GetTickCount();
    //unsigned int totalDuration;         //�ۼ�ͨ��ʱ��(��)
    //unsigned int txBytes;               //�ۼƷ����ֽ���(bytes)
    //unsigned int rxBytes;               //�ۼƽ����ֽ���(bytes)
    //unsigned int txKBitRate;            //��������(kbps)��˲ʱֵ
    //unsigned int rxKBitRate;            //��������(kbps)��˲ʱֵ

    //unsigned int txAudioKBitRate;       //��Ƶ��������(kbps)��˲ʱֵ
    //unsigned int rxAudioKBitRate;       //��Ƶ��������(kbps)��˲ʱֵ
    //unsigned int txVideoKBitRate;       //��Ƶ��������(kbps)��˲ʱֵ
    //unsigned int rxVideoKBitRate;       //��Ƶ��������(kbps)��˲ʱֵ

    //int users;                          //�û���
    //double cpuTotalUsage;               //��ǰϵͳ��CPUʹ����(%)
    //double cpuAppUsage;                 //��ǰӦ�ó����CPUʹ����(%)

    m_vectDebugInfo.clear();
    if (!m_bShowNetworkInfo) {
        m_vectDebugInfo.push_back(InteractDebugInfo(_T("�����"), mInteractRoomModel.roomId));
        m_vectDebugInfo.push_back(InteractDebugInfo(_T("SDK�汾"), STR2T(QHVC::INTERACT::QHVCInteract::GetVersion())));

        InteractUserModel* pInteractUerModel = InteractGlobalManager::GetInstance().GetInteractUserModel();
        if (mUserIdentity == InteractConstant::USER_IDENTITY_ANCHOR) {
            m_vectDebugInfo.push_back(InteractDebugInfo(_T("��ɫ"), USER_ANCHOR_NAME));
        }
        else  if (mUserIdentity == InteractConstant::USER_IDENTITY_GUEST) {
            m_vectDebugInfo.push_back(InteractDebugInfo(_T("��ɫ"), USER_GUEST_NAME));
        }
        else {
            m_vectDebugInfo.push_back(InteractDebugInfo(_T("��ɫ"), USER_AUDIENCE_NAME));
        }

        m_vectDebugInfo.push_back(InteractDebugInfo(_T("�ۼ�ͨ��ʱ��(��)"), STR2T(Util::String::StrFormat("%d", mInteractStats.rtcStatus.totalDuration))));
        m_vectDebugInfo.push_back(InteractDebugInfo(_T("�ۼƷ����ֽ���(bytes)"), STR2T(Util::String::StrFormat("%d", mInteractStats.rtcStatus.txBytes))));
        m_vectDebugInfo.push_back(InteractDebugInfo(_T("�ۼƽ����ֽ���(bytes)"), STR2T(Util::String::StrFormat("%d", mInteractStats.rtcStatus.rxBytes))));
        m_vectDebugInfo.push_back(InteractDebugInfo(_T("��������(kbps)"), STR2T(Util::String::StrFormat("%d", mInteractStats.rtcStatus.txKBitRate))));
        m_vectDebugInfo.push_back(InteractDebugInfo(_T("��������(kbps)"), STR2T(Util::String::StrFormat("%d", mInteractStats.rtcStatus.rxKBitRate))));

        m_vectDebugInfo.push_back(InteractDebugInfo(_T("��Ƶ��������(kbps)"), STR2T(Util::String::StrFormat("%d", mInteractStats.rtcStatus.txAudioKBitRate))));
        m_vectDebugInfo.push_back(InteractDebugInfo(_T("��Ƶ��������(kbps)"), STR2T(Util::String::StrFormat("%d", mInteractStats.rtcStatus.rxAudioKBitRate))));
        m_vectDebugInfo.push_back(InteractDebugInfo(_T("��Ƶ��������(kbps)"), STR2T(Util::String::StrFormat("%d", mInteractStats.rtcStatus.txVideoKBitRate))));
        m_vectDebugInfo.push_back(InteractDebugInfo(_T("��Ƶ��������(kbps)"), STR2T(Util::String::StrFormat("%d", mInteractStats.rtcStatus.rxVideoKBitRate))));

        m_vectDebugInfo.push_back(InteractDebugInfo(_T("�û���"), STR2T(Util::String::StrFormat("%d", mInteractStats.rtcStatus.users))));
        m_vectDebugInfo.push_back(InteractDebugInfo(_T("��ǰϵͳ��CPUʹ����(%)"), STR2T(Util::String::StrFormat("%.2f", mInteractStats.rtcStatus.cpuTotalUsage))));
        m_vectDebugInfo.push_back(InteractDebugInfo(_T("��ǰӦ�ó����CPUʹ����(%)"), STR2T(Util::String::StrFormat("%.2f", mInteractStats.rtcStatus.cpuAppUsage))));

        if (mUserIdentity == InteractConstant::USER_IDENTITY_ANCHOR || mUserIdentity == InteractConstant::USER_IDENTITY_GUEST) {
            InteractSetting& setting = InteractSetting::GetInstance();
            m_vectDebugInfo.push_back(InteractDebugInfo(_T("���͵���Ƶ֡��"), STR2T(Util::String::StrFormat("%d", mInteractStats.localVideoStatus.sentFrameRate))));
            m_vectDebugInfo.push_back(InteractDebugInfo(_T("������Ƶ�ֱ���-��"), STR2T(Util::String::StrFormat("%d", setting.GetResolutionWidth(mUserIdentity)))));
            m_vectDebugInfo.push_back(InteractDebugInfo(_T("������Ƶ�ֱ���-��"), STR2T(Util::String::StrFormat("%d", setting.GetResolutionHeight(mUserIdentity)))));
        }


        //std::string userId;                 //�û�ID
        //unsigned int delay;                 //�ӳ�
        //unsigned int width;                 //��Ƶ�ֱ���-��
        //unsigned int height;                //��Ƶ�ֱ���-��
        //unsigned int receivedBitrate;       //���ϴ�ͳ�ƺ���ܵ��ֽ���(byte)
        //unsigned int receivedFrameRate;     //���ϴ�ͳ�ƺ���ܵ�֡��
        //VIDEO_STREAM_LEVEL rxStreamType;    //��Ƶ���ȼ�
        //QUALITY quality;                    //��Ƶ������

        for (int i = 0; i < m_vectAnchor.size(); i++) {
            std::string userId = STR2A(m_vectAnchor[i].userid);
            std::map<std::string, QHVC::INTERACT::RemoteVideoStats>::const_iterator it = mInteractStats.mapRemoteVideoStatus.find(userId);
            if (it != mInteractStats.mapRemoteVideoStatus.end()) {
                m_vectDebugInfo.push_back(InteractDebugInfo(_T("�û�ID"), STR2T(it->second.userId)));
                m_vectDebugInfo.push_back(InteractDebugInfo(_T("�ӳ�"), STR2T(Util::String::StrFormat("%d", it->second.delay))));
                m_vectDebugInfo.push_back(InteractDebugInfo(_T("��Ƶ�ֱ���-��"), STR2T(Util::String::StrFormat("%d", it->second.width))));
                m_vectDebugInfo.push_back(InteractDebugInfo(_T("��Ƶ�ֱ���-��"), STR2T(Util::String::StrFormat("%d", it->second.height))));
                m_vectDebugInfo.push_back(InteractDebugInfo(_T("���ϴ�ͳ�ƺ���ܵ��ֽ���(byte)"), STR2T(Util::String::StrFormat("%d", it->second.receivedBitrate))));
                m_vectDebugInfo.push_back(InteractDebugInfo(_T("���ϴ�ͳ�ƺ���ܵ�֡��"), STR2T(Util::String::StrFormat("%d", it->second.receivedFrameRate))));
                m_vectDebugInfo.push_back(InteractDebugInfo(_T("��Ƶ������"), GetQualityName(it->second.quality).c_str()));
            }
        }
    }
    else {
        m_vectDebugInfo.push_back(InteractDebugInfo(_T("���͵�����"), GetQualityName((QHVC::INTERACT::QUALITY)mInteractStats.txQuality)));
        m_vectDebugInfo.push_back(InteractDebugInfo(_T("���յ�����"), GetQualityName((QHVC::INTERACT::QUALITY)mInteractStats.rxQuality)));
    }

    CListUI* pRoomInfoList = static_cast<CListUI*>(FindControl(_T("list_room")));
    if (pRoomInfoList) {
        if (pRoomInfoList->GetCount() > m_vectDebugInfo.size()) {
            pRoomInfoList->RemoveAll();
        }
        while (pRoomInfoList->GetCount() < m_vectDebugInfo.size()) {
            {
                CListTextElementUI* pListElement = new CListTextElementUI;
                pRoomInfoList->Add(pListElement);
            }
        }

        pRoomInfoList->Invalidate();
    }
}

bool CInteractInfoPanel::IsAudioMode()
{
    if (mInteractRoomModel.talkType == InteractConstant::TALK_TYPE_AUDIO){
        return true;
    }

    return false;
}

tstring CInteractInfoPanel::GetQualityName(QHVC::INTERACT::QUALITY quality)
{
    tstring qualityName = _T("δ֪");
    switch (quality)
    {
        case QHVC::INTERACT::QUALITY_UNKOWN:
            qualityName = _T("δ֪");
            break;
        case QHVC::INTERACT::QUALITY_EXCELLENT:
            qualityName = _T("�ǳ���");
            break;
        case QHVC::INTERACT::QUALITY_GOOD:
            qualityName = _T("��");
            break;
        case QHVC::INTERACT::QUALITY_POOR:
            qualityName = _T("һ��");
            break;
        case QHVC::INTERACT::QUALITY_BAD:
            qualityName = _T("��");
            break;
        case QHVC::INTERACT::QUALITY_VBAD:
            qualityName = _T("��");
            break;
        case QHVC::INTERACT::QUALITY_DOWN:
            qualityName = _T("�ǳ���");
            break;
        case QHVC::INTERACT::QUALITY_UNSUPPORTED:
            qualityName = _T("������");
            break;
        default:
            break;
    }

    qualityName.append(_T("("));
    qualityName.append(STR2T(Util::String::StrFormat("%d", quality)));
    qualityName.append(_T(")"));

    return qualityName;
}

void CInteractInfoPanel::EnableJoin(bool enable)
{
    CButtonUI* pbtn = static_cast<CButtonUI*>(FindControl(_T("btn_join")));
    if (pbtn) {
        pbtn->SetEnabled(enable);
    }
}

void CInteractInfoPanel::UpdateDeviceInfo()
{
     InitDevicInfo();
}
