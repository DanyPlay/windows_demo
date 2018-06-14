#ifndef _INTERACT_MAIN_DLG_H_
#define _INTERACT_MAIN_DLG_H_

#pragma once

#include "../../UI/UI.h"
#include "../ModelDef.h"
#include "InteractUIConstant.h"
#include "InteractHomePage.h"
#include "InteractRoomPage.h"
#include "InteractCreateRoomPage.h"
#include "InteractDevicePage.h"
#include "InteractSettingPage.h"
#include "../Net/InteractServerApi.h"


class CInteractMainDlg : public CDuiDialogImpl < CInteractMainDlg > , InteractServerApiCallBack
{
public:
    CInteractMainDlg();
    virtual ~CInteractMainDlg();

public:
    DUI_DECLARE_WND_CLASS(_T("InteractMainDlg"));
    DUI_DECLARE_SKIN_FILE(_T("InteractMainDlg.xml"));
    DUI_DECLARE_MESSAGE_MAP();

    DUI_WIN_BEGIN_MSG_MAP(CInteractMainDlg, CDuiDialogImpl<CInteractMainDlg>)
        DUI_WIN_MESSAGE_HANDLER(WM_CREATE, OnCreate)
        DUI_WIN_MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
        DUI_WIN_MESSAGE_HANDLER(WM_PAGE_SELECT_CHANGED, OnPageSelectChanged)
    DUI_WIN_END_MSG_MAP()

public:
    LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

    LRESULT OnPageSelectChanged(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

public:
    virtual CControlUI* CreateControl(LPCTSTR pstrClass);

    virtual void OnClick(TNotifyUI& msg);
    virtual void OnSelectChanged(TNotifyUI& msg);

private:
    virtual void OnServerApiUserLoginResult(const InteractServerApiRet& ret, const InteractUserModel& interactUserModel);

private:
    CTabLayoutUI* m_pTabContainer;

    std::map<INT, CWindowWnd*> m_mapPages;
    CInteractHomePage* m_pPageHome;
    CInteractRoomPage* m_pPageRoom;
    CInteractCreateRoomPage* m_pPageCreateRoom;
    CInteractDevicePage* m_pPageDevice;
    CInteractSettingPage* m_pPageSetting;
};

#endif