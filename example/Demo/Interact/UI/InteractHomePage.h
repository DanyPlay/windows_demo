#pragma once
#include "../../UI/UI.h"
#include "../ModelDef.h"
#include "../Net/InteractServerApi.h"
#include "../IM/InteractImManager.h"

class CInteractHomePage : public CDuiChildWindowImpl < CInteractHomePage >, InteractServerApiCallBack, InteractImCallback
{
public:
    CInteractHomePage();
    ~CInteractHomePage();

public:
    DUI_DECLARE_WND_CLASS(_T("InteractHomePage"));
    DUI_DECLARE_SKIN_FILE(_T("InteractHomePage.xml"));
    DUI_DECLARE_MESSAGE_MAP();

    DUI_WIN_BEGIN_MSG_MAP(CInteractHomePage, CDuiChildWindowImpl<CInteractHomePage>)
        DUI_WIN_MESSAGE_HANDLER(WM_CREATE, OnCreate)
        DUI_WIN_MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
    DUI_WIN_END_MSG_MAP()

public:
    LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

public:
    virtual void OnClick(TNotifyUI& msg);

private:
    void InitData();
    void OnLoginClick();
    void OnRoomTypeClick(InteractConstant::ROOM_TYPE roomType);

private:
    virtual void OnServerApiUserLoginResult(const InteractServerApiRet& ret, const InteractUserModel& interactUserModel);

    virtual void OnImConnectStatusChanged(ImConstant::CONNECT_STATUS connectStatus);

private:
    CEditUI* m_pEdtBid;
    CEditUI* m_pEdtCid;
    CEditUI* m_pEdtAppKey;
    CEditUI* m_pEdtAppSecret;
    CEditUI* m_pEdtUid;
    CControlUI* m_pLayoutLogin;
};