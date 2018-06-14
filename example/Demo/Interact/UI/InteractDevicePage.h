#ifndef _INTERACT_DEVICE_PAGE_H_
#define _INTERACT_DEVICE_PAGE_H_

#pragma once

#include "../../UI/UI.h"

class CInteractDevicePage : public CDuiChildWindowImpl < CInteractDevicePage >
{
public:
    CInteractDevicePage();
    ~CInteractDevicePage();

public:
    DUI_DECLARE_WND_CLASS(_T("InteractDevicePage"));
    DUI_DECLARE_SKIN_FILE(_T("InteractDevicePage.xml"));
    DUI_DECLARE_MESSAGE_MAP();

    DUI_WIN_BEGIN_MSG_MAP(CInteractDevicePage, CDuiChildWindowImpl<CInteractDevicePage>)
        DUI_WIN_MESSAGE_HANDLER(WM_CREATE, OnCreate)
        DUI_WIN_MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
    DUI_WIN_END_MSG_MAP()

public:
    LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

public:
    virtual void OnClick(TNotifyUI& msg);
};

#endif
