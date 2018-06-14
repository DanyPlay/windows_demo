#include "stdafx.h"
#include "InteractCreateRoomPage.h"
#include "InteractAnchorDlg.h"
#include "../InteractGlobalManager.h"
#include "../Net/InteractServerApi.h"


DUI_BEGIN_MESSAGE_MAP(CInteractCreateRoomPage, CDuiDialogImpl<CInteractCreateRoomPage>)
DUI_ON_MSGTYPE(DUI_MSGTYPE_CLICK, OnClick)
DUI_ON_MSGTYPE(DUI_MSGTYPE_SELECTCHANGED, OnSelectChanged)
DUI_END_MESSAGE_MAP()

CInteractCreateRoomPage::CInteractCreateRoomPage()
: m_pEdtRoomName(NULL)
, m_pEdtRoomMax(NULL)
, m_pCmbRoomMode(NULL)
, m_pOptAudioOnly(NULL)
{
}

CInteractCreateRoomPage::~CInteractCreateRoomPage()
{

}

LRESULT CInteractCreateRoomPage::OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    m_pEdtRoomName = static_cast<CEditUI*>(FindControl(_T("edt_room_name")));
    m_pEdtRoomMax = static_cast<CEditUI*>(FindControl(_T("edt_room_max")));
    m_pOptAudioOnly = static_cast<COptionUI*>(FindControl(_T("opt_room_audio_only")));

    m_pCmbRoomMode = static_cast<CComboUI*>(FindControl(_T("cmb_room_mode")));
    for (int i = 0, size = ARRAYSIZE(ROOM_MODES); i < size; i++)
    {
        CListLabelElementUI* pElementUI = new CListLabelElementUI();
        pElementUI->SetText(ROOM_MODES[i]);
        m_pCmbRoomMode->Add(pElementUI);
    }
    m_pCmbRoomMode->SelectItem(InteractConstant::ROOM_TYPE::ROOM_TYPE_ANCHOR_AND_GUEST - 1);

    InteractServerApi::GetInstance().AddApiCallback(this);

    return 0;
}

LRESULT CInteractCreateRoomPage::OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    InteractServerApi::GetInstance().RemoveApiCallback(this);

    return 0;
}

void CInteractCreateRoomPage::OnClick(TNotifyUI& msg)
{
    CDuiString sCtrlName = msg.pSender->GetName();

    if (sCtrlName == _T("btn_create_room"))
    {
        OnCreateRoomClick();
        return;
    }

    Super::OnClick(msg);
}

void CInteractCreateRoomPage::OnCreateRoomClick()
{
    bool bAudioOnly = m_pOptAudioOnly->IsSelected();

    tstring strRoomName = m_pEdtRoomName->GetText();
    if (strRoomName.size() == 0) {
        DuiMessageBox(m_hWnd, _T("请输入房间名称！"), INTERACT_TITLE, MB_OK);
        return;
    }

    tstring strRoomMaxCount = m_pEdtRoomMax->GetText();
    if (strRoomMaxCount.size() == 0) {
        DuiMessageBox(m_hWnd, _T("请输入房间最大人数！"), INTERACT_TITLE, MB_OK);
        return;
    }
    int nDefMaxCount = bAudioOnly ? 99 : 6;
    int nRoomMaxCount = _ttoi(strRoomMaxCount.c_str());
    if (nRoomMaxCount <= 0 || nRoomMaxCount > nDefMaxCount) {
        DuiMessageBox(m_hWnd, _T("房间最大人数无效，请重新输入！"), INTERACT_TITLE, MB_OK);
        return;
    }

    int nRoomMode = m_pCmbRoomMode->GetCurSel() + 1;

    int nRoomLifeType = InteractConstant::ROOM_LIFE_TYPE::ROOM_LIFE_TYPE_BIND_ANCHOR;
    if (nRoomMode == InteractConstant::ROOM_TYPE::ROOM_TYPE_HOME_PARTY)
    {
        nRoomLifeType = InteractConstant::ROOM_LIFE_TYPE::ROOM_LIFE_TYPE_BIND_ROOM;
    }



    InteractServerApi::GetInstance().CreateRoom(
        InteractGlobalManager::GetInstance().GetInteractUserId(),
        strRoomName,
        nRoomMode,
        bAudioOnly ? InteractConstant::TALK_TYPE_AUDIO : InteractConstant::TALK_TYPE_ALL,
        nRoomLifeType,
        nRoomMaxCount
        );
}

void CInteractCreateRoomPage::OnServerApiCreateRoomResult(const InteractServerApiRet& ret, const InteractRoomModel& interactRoomModel)
{
    if (ret.errCode == 0) {
        mInteractRoomModel = interactRoomModel;
        PostMessage(WM_CREATE_ROOM_SUCCESS, NULL, NULL);
    }
    else {
        if (IsWindowVisible(m_hWnd) && IsWindowEnabled(m_hWnd)) {
            TCHAR szMsg[MAX_PATH] = { 0 };
            _sntprintf_s(szMsg, ARRAYSIZE(szMsg), _T("错误(%d)：%s"), ret.errCode, ret.errMessage.c_str());
            DuiMessageBox(m_hWnd, szMsg, INTERACT_TITLE, MB_OK);
        }
    }

}

LRESULT CInteractCreateRoomPage::OnHandleCreateRoomSuccess(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    InteractGlobalManager::GetInstance().ShowMainWindow(false);
    CInteractAnchorDlg dlg;
    dlg.SetInteractUserIdentity(InteractConstant::USER_IDENTITY_ANCHOR);
    dlg.SetInteractRoomModel(mInteractRoomModel);
    dlg.DoModal();
    InteractGlobalManager::GetInstance().ShowMainWindow(true);
    return 0;
}

void CInteractCreateRoomPage::OnSelectChanged(TNotifyUI& msg)
{
    CDuiString sCtrlName = msg.pSender->GetName();
    if (sCtrlName == _T("opt_room_all"))
    {
        COptionUI* pOptAll = static_cast<COptionUI*>(msg.pSender);
        if (pOptAll && pOptAll->IsSelected()) {
            if (m_pEdtRoomMax) {
                m_pEdtRoomMax->SetText(_T("6"));
            }
        }
    }
    else if (sCtrlName == _T("opt_room_audio_only"))
    {
        COptionUI* pOptAudioOnly = static_cast<COptionUI*>(msg.pSender);
        if (pOptAudioOnly && pOptAudioOnly->IsSelected()) {
            if (m_pEdtRoomMax) {
                m_pEdtRoomMax->SetText(_T("99"));
            }
        }
    }
}