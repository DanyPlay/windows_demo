#pragma once
#include "WindowImpl.h"

template<class T>
class CDuiDialogImpl : public CDuiWindowImpl < T >
{
public:
    DUI_DECLARE_WND_CLASS(_T("Dialog"));

    virtual UINT GetClassStyle() const { return UI_CLASSSTYLE_DIALOG; }

private:
    int ShowModal() { return Super::ShowModal(); };

public:
    int DoModal(HWND hWndParent = ::GetActiveWindow());
};

template<class T>
int CDuiDialogImpl<T>::DoModal(HWND hWndParent /*= ::GetActiveWindow()*/)
{
    Create(hWndParent, NULL, UI_WNDSTYLE_DIALOG, 0L);
    CenterWindow();
    return ShowModal();
}
