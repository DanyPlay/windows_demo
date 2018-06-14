#include "stdafx.h"
#include "InteractRoomPage.h"
#include "InteractAnchorDlg.h"
#include "InteractUIConstant.h"
#include "../InteractGlobalManager.h"
#include "../Net/InteractServerApi.h"


DUI_BEGIN_MESSAGE_MAP(CInteractRoomPage, CDuiChildWindowImpl<CInteractRoomPage>)
DUI_ON_MSGTYPE(DUI_MSGTYPE_CLICK, OnClick)
DUI_ON_MSGTYPE(DUI_MSGTYPE_ITEMSELECT, OnItemSelect)
DUI_END_MESSAGE_MAP()

CInteractRoomPage::CInteractRoomPage()
: m_pEdtSearch(NULL)
, m_pBtnSearch(NULL)
, m_pRoomList(NULL)
, m_pBtnCreateRoom(NULL)
, m_bWantJoinRoom(false)
{

}

CInteractRoomPage::~CInteractRoomPage()
{

}

LRESULT CInteractRoomPage::OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    m_pCmbMode = static_cast<CComboUI*>(FindControl(_T("cmb_mode")));
    for (int i = 0, size = ARRAYSIZE(ROOM_MODES); i < size; i++)
    {
        CListLabelElementUI* pElementUI = new CListLabelElementUI();
        pElementUI->SetText(ROOM_MODES[i]);
        m_pCmbMode->Add(pElementUI);
    }
    m_pCmbMode->SelectItem(InteractConstant::ROOM_TYPE::ROOM_TYPE_ANCHOR_AND_GUEST - 1);

    m_pRoomList = static_cast<CTileLayoutUI*>(FindControl(_T("list_room")));

    m_pEdtSearch = static_cast<CEditUI*>(FindControl(_T("edt_search")));
    m_pBtnSearch = static_cast<CButtonUI*>(FindControl(_T("btn_search")));
    m_pBtnCreateRoom = static_cast<CButtonUI*>(FindControl(_T("btn_create_room")));

    InitData();

    InteractServerApi::GetInstance().AddApiCallback(this);

    return 0;
}


void CInteractRoomPage::InitData()
{
}

LRESULT CInteractRoomPage::OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    InteractServerApi::GetInstance().RemoveApiCallback(this);

    return 0;
}

void CInteractRoomPage::OnClick(TNotifyUI& msg)
{
    CDuiString sCtrlName = msg.pSender->GetName();
    if (sCtrlName == _T("btn_search"))
    {
        OnSearchClick();
        return;
    }
    else if (sCtrlName == _T("btn_refresh"))
    {
        OnRefreshClick();
        return;
    }
    else if (sCtrlName == _T("btn_room_item"))
    {
        int index = msg.pSender->GetTag();
        if (index >= 0 && index < m_vectInteractRoomModel.size()) {
            InteractGlobalManager::GetInstance().ShowMainWindow(false);
            CInteractAnchorDlg dlg;
            dlg.SetInteractUserIdentity(InteractConstant::USER_IDENTITY_AUDIENCE);
            dlg.SetInteractRoomModel(m_vectInteractRoomModel[index]);
            dlg.DoModal();
            InteractGlobalManager::GetInstance().ShowMainWindow(true);
        }
    }

    Super::OnClick(msg);
}


void CInteractRoomPage::OnItemSelect(TNotifyUI& msg)
{
    CDuiString sCtrlName = msg.pSender->GetName();

    if (sCtrlName == _T("cmb_mode"))
    {
        //TODO
    }
}

void CInteractRoomPage::OnSearchClick()
{
    CDuiString strKey = m_pEdtSearch->GetText();
    if (strKey.IsEmpty())
    {
        return;
    }
    m_bWantJoinRoom = true;
    InteractServerApi::GetInstance().GetRoomInfo(InteractGlobalManager::GetInstance().GetInteractUserId(), tstring(strKey));
    OnRefreshClick();
}

void CInteractRoomPage::OnRefreshClick()
{
    InteractServerApi::GetInstance().GetRoomList(InteractGlobalManager::GetInstance().GetInteractUserId(), m_pCmbMode->GetCurSel() + 1);
}

void CInteractRoomPage::OnServerApiGetRoomListResult(const InteractServerApiRet& ret, const std::vector<InteractRoomModel>& interactRoomModelList)
{
    m_vectInteractRoomModel.clear();
    for (int i = 0; i < interactRoomModelList.size(); i++) {
        InteractRoomModel interactRoomModel = interactRoomModelList[i];
        m_vectInteractRoomModel.push_back(interactRoomModel);
    }

    CDialogBuilder builder;
    CContainerUI* pRoomItem = static_cast<CContainerUI*>(builder.Create(_T("roomItem.xml"), (UINT)0));

    m_pRoomList->RemoveAll();
    for (int i = 0; i < m_vectInteractRoomModel.size(); i++)
    {
        if (pRoomItem == NULL) pRoomItem = static_cast<CContainerUI*>(builder.Create());
        if (pRoomItem != NULL) {

            CLabelUI* pLabelTitle = static_cast<CLabelUI*>(pRoomItem->FindSubControl(_T("txt_title")));
            if (pLabelTitle != NULL) {
                pLabelTitle->SetText(m_vectInteractRoomModel[i].roomName.c_str());
            }

            CLabelUI* pLabelType = static_cast<CLabelUI*>(pRoomItem->FindSubControl(_T("txt_type")));
            if (pLabelType != NULL) {
                if (m_vectInteractRoomModel[i].roomLifeType == InteractConstant::ROOM_TYPE_ANCHOR_AND_ANCHOR) {
                    pLabelType->SetText(ROOM_MODES[0]);
                }
                else if (m_vectInteractRoomModel[i].roomLifeType == InteractConstant::ROOM_TYPE_ANCHOR_AND_GUEST) {
                    pLabelType->SetText(ROOM_MODES[1]);
                }
                else if (m_vectInteractRoomModel[i].roomLifeType == InteractConstant::ROOM_TYPE_HOME_PARTY) {
                    pLabelType->SetText(ROOM_MODES[2]);
                }
            }

            CLabelUI* pLabelId = static_cast<CLabelUI*>(pRoomItem->FindSubControl(_T("txt_id")));
            if (pLabelId != NULL) {
                pLabelId->SetText(m_vectInteractRoomModel[i].roomId.c_str());
            }

            CLabelUI* pLabelNum = static_cast<CLabelUI*>(pRoomItem->FindSubControl(_T("txt_num")));
            if (pLabelNum != NULL) {
                pLabelNum->SetText(STR2T(Util::String::StrFormat("%d", m_vectInteractRoomModel[i].onlineNum)).c_str());
            }

            CButtonUI* pBtnRoomItem = static_cast<CButtonUI*>(pRoomItem->FindSubControl(_T("btn_room_item")));
            if (pBtnRoomItem != NULL) {
                pBtnRoomItem->SetTag(i);
            }

            m_pRoomList->Add(pRoomItem);
            pRoomItem = NULL;
        }
    }
}

void CInteractRoomPage::OnServerApiGetRoomInfoResult(const InteractServerApiRet& ret, const InteractRoomModel& interactRoomModel)
{
    if (ret.errCode == 0) {
        if (m_bWantJoinRoom) {
            m_bWantJoinRoom = false;
            m_CurInteractRoomModel = interactRoomModel;
            PostMessage(WM_BEGIN_JOIN_ROOM);
        }
    }
    else {
        if (IsWindowVisible(m_hWnd) && IsWindowEnabled(m_hWnd)) {
            TCHAR szMsg[MAX_PATH] = { 0 };
            _sntprintf_s(szMsg, ARRAYSIZE(szMsg), _T("´íÎó(%d)£º%s"), ret.errCode, ret.errMessage.c_str());
            DuiMessageBox(m_hWnd, szMsg, INTERACT_TITLE, MB_OK);
        }
    }
}

void CInteractRoomPage::OnServerApiCreateRoomResult(const InteractServerApiRet& ret, const InteractRoomModel& interactRoomModel)
{
    OnRefreshClick();
}

LRESULT CInteractRoomPage::OnShowWindow(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    if (wParam == TRUE) {
        OnRefreshClick();
    }

    return 0;
}

LRESULT CInteractRoomPage::OnHandleBeginJoinRoom(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    InteractGlobalManager::GetInstance().ShowMainWindow(false);
    CInteractAnchorDlg dlg;
    dlg.SetInteractUserIdentity(InteractConstant::USER_IDENTITY_AUDIENCE);
    dlg.SetInteractRoomModel(m_CurInteractRoomModel);
    dlg.DoModal();
    InteractGlobalManager::GetInstance().ShowMainWindow(true);
    return 0;
}
