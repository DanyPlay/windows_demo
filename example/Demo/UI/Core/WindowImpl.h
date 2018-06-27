#pragma once
#include "../Inlcude/UIlib.h"

template<class T>
class CDuiWindowImpl : public WindowImplBase
{
public:
    typedef  CDuiWindowImpl<T> Super;

    virtual UILIB_RESOURCETYPE GetResourceType() const;
    virtual CDuiString GetSkinFolder();
    virtual CDuiString GetZIPFileName() const;
    virtual void SetWindowText(LPCTSTR lpText);
    virtual void SetWindowText(HWND hWnd, LPCTSTR lpText);

public:
#define DUI_DECLARE_WND_CLASS(WndClassName) \
    virtual LPCTSTR GetWindowClassName(void) const { return WndClassName; }

#define DUI_DECLARE_SKIN_FILE(SkinFile) \
    virtual CDuiString GetSkinFile() { return SkinFile; }

    virtual UINT GetClassStyle() const { return UI_CLASSSTYLE_FRAME; }

public:
    virtual LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled);
    CControlUI* FindControl(LPCTSTR pstrName) const;
};

template<class T>
UILIB_RESOURCETYPE CDuiWindowImpl<T>::GetResourceType() const
{
    return UILIB_FILE;
}

template<class T>
CDuiString CDuiWindowImpl<T>::GetZIPFileName() const
{
    return _T("Res.zip");
}

template<typename T>
CDuiString CDuiWindowImpl<T>::GetSkinFolder()
{
    return _T("Res\\");
}

template<class T>
void CDuiWindowImpl<T>::SetWindowText(LPCTSTR lpText)
{
    CLabelUI* pLabelUI = static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("titlelb")));
    if (pLabelUI)
    {
        pLabelUI->SetText(lpText);
    }

    ::SetWindowText(m_hWnd, lpText);
}

template<class T>
void CDuiWindowImpl<T>::SetWindowText(HWND hWnd, LPCTSTR lpText)
{
    ::SetWindowText(hWnd, lpText);
}

template<class T>
LRESULT CDuiWindowImpl<T>::MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled)
{
    return 0;
}

template<class T>
CControlUI* CDuiWindowImpl<T>::FindControl(LPCTSTR pstrName) const
{
    return m_PaintManager.FindControl(pstrName);
}
