#ifndef __UIWND_H__
#define __UIWND_H__

#pragma once

#include "../../Inlcude/UIlib.h"

#define  DUI_CTR_WINDOW         _T("Window")

namespace DuiLib
{
    class CWindowUI : public CControlUI
    {
    public:
        CWindowUI();
        ~CWindowUI();

    public:
        bool Attach(HWND hWnd);
        HWND Detach();
        HWND GetHWND();
    public:
        LPCTSTR GetClass() const;
        LPVOID GetInterface(LPCTSTR pstrName);

        virtual void SetPos(RECT rc, bool bNeedInvalidate = true);
        virtual void Move(SIZE szOffset, bool bNeedInvalidate = true);

        virtual void SetVisible(bool bVisible = true);
        virtual void SetInternVisible(bool bVisible = true);
        virtual void SetEnabled(bool bEnable = true);
        virtual void SetFocus();

    private:
        HWND m_hWnd;
    };
}

#endif