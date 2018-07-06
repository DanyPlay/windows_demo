#ifndef __RENDER_WND__
#define __RENDER_WND__

#pragma once

#include "../../../UI/UI.h"
#include "VideoLayer.h"
class CVideoWnd : public CWindowWnd
{
public:
    CVideoWnd();
    ~CVideoWnd();
public:
    void CreateEx(HWND hParent, CWindowUI* pWindowUI);
    void DestroyEx();
    CWindowUI* GetCWindowUI();

    void OnMove();

public:
    CControlUI* FindSubControl(LPCTSTR pstrName) const;
    void SetNotifyWnd(HWND hwnd);
    void SetTag(int tag);
    void Update();

public:
    virtual LPCTSTR GetWindowClassName() const { return _T("VideoWnd"); }

protected:
    virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
    virtual void OnFinalMessage(HWND hWnd);

private:
    CWindowUI* m_pWindowUI;

    CVideoLayer m_wndLayer;
};

#endif
