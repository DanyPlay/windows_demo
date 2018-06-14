#pragma once
#include "UI.h"

class CMainWnd : public CDuiWindowImpl < CMainWnd >
{
public:
    CMainWnd() {};
    virtual ~CMainWnd() {};

public:
    DUI_DECLARE_WND_CLASS(_T("MainWnd"));
    DUI_DECLARE_SKIN_FILE(_T("MainWnd.xml"));
    DUI_DECLARE_MESSAGE_MAP();

    DUI_WIN_BEGIN_MSG_MAP(CMainWnd, CDuiWindowImpl<CMainWnd>)
        DUI_WIN_MESSAGE_HANDLER(WM_CREATE, OnCreate)
        DUI_WIN_MESSAGE_HANDLER(WM_CLOSE, OnClose)
        DUI_WIN_MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
    DUI_WIN_END_MSG_MAP()

    LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

public:
    virtual void OnClick(TNotifyUI& msg);

protected:

};