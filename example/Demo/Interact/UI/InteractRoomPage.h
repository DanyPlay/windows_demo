#ifndef _INTERACT_ROOM_PAGE_H_
#define _INTERACT_ROOM_PAGE_H_

#pragma once

#include "../../UI/UI.h"
#include "../ModelDef.h"
#include "../Net/InteractServerApi.h"
#include "InteractUIConstant.h"


class CInteractRoomPage : public CDuiChildWindowImpl < CInteractRoomPage >, InteractServerApiCallBack
{
public:
    CInteractRoomPage();
    ~CInteractRoomPage();

public:
    DUI_DECLARE_WND_CLASS(_T("InteractRoomPage"));
    DUI_DECLARE_SKIN_FILE(_T("InteractRoomPage.xml"));
    DUI_DECLARE_MESSAGE_MAP();

    DUI_WIN_BEGIN_MSG_MAP(CInteractRoomPage, CDuiChildWindowImpl<CInteractRoomPage>)
        DUI_WIN_MESSAGE_HANDLER(WM_CREATE, OnCreate)
        DUI_WIN_MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
        DUI_WIN_MESSAGE_HANDLER(WM_SHOWWINDOW, OnShowWindow)
        DUI_WIN_MESSAGE_HANDLER(WM_BEGIN_JOIN_ROOM, OnHandleBeginJoinRoom)
    DUI_WIN_END_MSG_MAP()

public:
    LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT OnShowWindow(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT OnHandleBeginJoinRoom(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

public:
    virtual void OnClick(TNotifyUI& msg);
    virtual void OnItemSelect(TNotifyUI& msg);

private:
    void InitData();
    void OnSearchClick();
    void OnRefreshClick();

private:
    virtual void OnServerApiGetRoomListResult(const InteractServerApiRet& ret, const std::vector<InteractRoomModel>& interactRoomModelList);
    virtual void OnServerApiGetRoomInfoResult(const InteractServerApiRet& ret, const InteractRoomModel& interactRoomModel);
    virtual void OnServerApiCreateRoomResult(const InteractServerApiRet& ret, const InteractRoomModel& interactRoomModel);

private:
    CComboUI* m_pCmbMode;
    CEditUI* m_pEdtSearch;
    CButtonUI* m_pBtnSearch;
    CTileLayoutUI* m_pRoomList;
    CButtonUI* m_pBtnCreateRoom;

    std::vector<InteractRoomModel> m_vectInteractRoomModel;
    InteractRoomModel m_CurInteractRoomModel;
    bool m_bWantJoinRoom;
};

#endif