#pragma once
#include "../../../UI/UI.h"

class CVideoLayer : public CDuiChildWindowImpl < CVideoLayer >
{
public:
    CVideoLayer();
    ~CVideoLayer();

public:
    DUI_DECLARE_WND_CLASS(_T("VideoLayer"));
    DUI_DECLARE_SKIN_FILE(_T("VideoLayer.xml"));
    DUI_DECLARE_MESSAGE_MAP();

    DUI_WIN_BEGIN_MSG_MAP(CVideoLayer, CDuiChildWindowImpl<CVideoLayer>)
        DUI_WIN_MESSAGE_HANDLER(WM_CREATE, OnCreate)
        DUI_WIN_MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
    DUI_WIN_END_MSG_MAP()

private:
    LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

public:
    virtual void OnClick(TNotifyUI& msg);
    void SetNotifyWnd(HWND hwnd);
    void SetTag(int tag);
private:
    HWND m_hNotifyWnd;
    int m_nTag;
};