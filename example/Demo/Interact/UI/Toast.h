#pragma once
#include "../../UI/UI.h"

class CDuiToast : public CDuiDialogImpl < CDuiToast >
{
public:
    CDuiToast();
    CDuiToast(HWND hParentWnd, LPCTSTR lpText, int nDuration);

public:
    DUI_DECLARE_SKIN_FILE(_T("Toast.xml"));

    DUI_WIN_BEGIN_MSG_MAP(CDuiMessageBox, CDuiDialogImpl<CDuiToast>)
        DUI_WIN_MESSAGE_HANDLER(WM_CREATE, OnCreate)
        DUI_WIN_MESSAGE_HANDLER(WM_TIMER, OnTimer)
        DUI_WIN_MESSAGE_HANDLER(WM_NCDESTROY, OnNcDestroy)
    DUI_WIN_END_MSG_MAP()

    LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT OnTimer(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT OnNcDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

private:
    CDuiString m_strText;
    HWND m_hParentWnd;
    int m_nDuration;

};

void DuiShowToast(HWND hParentWnd, LPCTSTR lpText, int nDuration);