#pragma once
#include "InteractUIConstant.h"
#include "../../UI/UI.h"
#include "../ModelDef.h"
#include "../Net/InteractServerApi.h"

class CInteractCreateRoomPage : public CDuiChildWindowImpl < CInteractCreateRoomPage >, InteractServerApiCallBack
{
public:
    CInteractCreateRoomPage();
    ~CInteractCreateRoomPage();

public:
    DUI_DECLARE_WND_CLASS(_T("InteractCreateRoomPage"));
    DUI_DECLARE_SKIN_FILE(_T("InteractCreateRoomPage.xml"));
    DUI_DECLARE_MESSAGE_MAP();

    DUI_WIN_BEGIN_MSG_MAP(CInteractCreateRoomPage, CDuiChildWindowImpl<CInteractCreateRoomPage>)
        DUI_WIN_MESSAGE_HANDLER(WM_CREATE, OnCreate)
        DUI_WIN_MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
        DUI_WIN_MESSAGE_HANDLER(WM_CREATE_ROOM_SUCCESS, OnHandleCreateRoomSuccess)
    DUI_WIN_END_MSG_MAP()

private:
    LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT OnHandleCreateRoomSuccess(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

private:
    virtual void OnClick(TNotifyUI& msg);
    virtual void OnSelectChanged(TNotifyUI& msg);

    void OnCreateRoomClick();

private:
    virtual void OnServerApiCreateRoomResult(const InteractServerApiRet& ret, const InteractRoomModel& interactRoomModel);

private:
    CEditUI* m_pEdtRoomName;
    CEditUI* m_pEdtRoomMax;
    CComboUI* m_pCmbRoomMode;
    COptionUI* m_pOptAudioOnly;

private:
    InteractRoomModel mInteractRoomModel;
};