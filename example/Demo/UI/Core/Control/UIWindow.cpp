#include "stdafx.h"
#include "UIWindow.h"

namespace DuiLib
{
    CWindowUI::CWindowUI()
        : m_hWnd(NULL)
    {

    }

    CWindowUI::~CWindowUI()
    {

    }

    bool CWindowUI::Attach(HWND hWnd)
    {
        if (!::IsWindow(hWnd))
            return false;

        m_hWnd = hWnd;
        return true;
    }

    HWND CWindowUI::Detach()
    {
        HWND hWnd = m_hWnd;
        m_hWnd = NULL;
        return hWnd;
    }

    LPCTSTR CWindowUI::GetClass() const
    {
        return DUI_CTR_WINDOW;
    }

    LPVOID CWindowUI::GetInterface(LPCTSTR pstrName)
    {
        if (_tcscmp(pstrName, DUI_CTR_WINDOW) == 0)
            return static_cast<CWindowUI*>(this);
        return CControlUI::GetInterface(pstrName);
    }

    void CWindowUI::SetPos(RECT rc, bool bNeedInvalidate /*= true*/)
    {
        CControlUI::SetPos(rc, bNeedInvalidate);

        if (::IsWindow(m_hWnd))
        {
            RECT rect = GetPos();

            ::SetWindowPos(m_hWnd, NULL, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, SWP_NOZORDER | SWP_NOACTIVATE);
            if (bNeedInvalidate)
            {
                ::InvalidateRect(m_hWnd, &rect, FALSE);
            }
        }
    }

    void CWindowUI::Move(SIZE szOffset, bool bNeedInvalidate /*= true*/)
    {
        CControlUI::Move(szOffset, bNeedInvalidate);

        if (::IsWindow(m_hWnd))
        {
            RECT rect = GetPos();

            ::SetWindowPos(m_hWnd, NULL, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, SWP_NOZORDER | SWP_NOACTIVATE);
            if (bNeedInvalidate)
            {
                ::InvalidateRect(m_hWnd, &rect, FALSE);
            }
        }
    }

    void CWindowUI::SetVisible(bool bVisible /*= true*/)
    {
        CControlUI::SetVisible(bVisible);

        if (::IsWindow(m_hWnd))
        {
            ::ShowWindow(m_hWnd, bVisible ? SW_SHOW : SW_HIDE);
        }
    }

    void CWindowUI::SetInternVisible(bool bVisible /*= true*/)
    {
        CControlUI::SetInternVisible(bVisible);

        if (::IsWindow(m_hWnd))
        {
            ::ShowWindow(m_hWnd, bVisible ? SW_SHOW : SW_HIDE);
        }
    }

    void CWindowUI::SetEnabled(bool bEnable /*= true*/)
    {
        CControlUI::SetEnabled(bEnable);

        if (::IsWindow(m_hWnd))
        {
            ::EnableWindow(m_hWnd, bEnable ? TRUE : FALSE);
        }
    }

    void CWindowUI::SetFocus()
    {
        CControlUI::SetFocus();

        if (::IsWindow(m_hWnd))
        {
            ::SetFocus(m_hWnd);
        }
    }

    HWND CWindowUI::GetHWND()
    {
        return m_hWnd;
    }

}

