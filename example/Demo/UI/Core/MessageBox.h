#pragma once
#include "DialogImpl.h"

class CDuiMessageBox : public CDuiDialogImpl < CDuiMessageBox >
{
public:
    CDuiMessageBox();
    CDuiMessageBox(LPCTSTR lpText, LPCTSTR lpCaption, UINT uType);

public:
    DUI_DECLARE_SKIN_FILE(_T("MessageBox.xml"));

    DUI_WIN_BEGIN_MSG_MAP(CDuiMessageBox, CDuiDialogImpl<CDuiMessageBox>)
        DUI_WIN_MESSAGE_HANDLER(WM_CREATE, OnCreate)
    DUI_WIN_END_MSG_MAP()

    LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

public:
    virtual void OnClick(TNotifyUI& msg);

public:
    void SetOkBtnText(LPCTSTR lpText);
    void SetCancelBtnText(LPCTSTR lpText);

private:
    CDuiString m_strText;
    CDuiString m_strCaption;
    CDuiString m_strOk;
    CDuiString m_strCancel;
    UINT m_uType;
};

INT DuiMessageBox(HWND hParentWnd, LPCTSTR lpText, LPCTSTR lpCaption, UINT uType);