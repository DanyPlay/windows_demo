#ifndef _CHILD_WINDOW_IMPL_H_
#define _CHILD_WINDOW_IMPL_H_

#pragma once

#include "../Inlcude/UIlib.h"

template<class T>
class CDuiChildWindowImpl : public CDuiWindowImpl < T >
{
public:
    CDuiChildWindowImpl();
    CDuiChildWindowImpl(CWindowUI* pWindowUI);
    ~CDuiChildWindowImpl();

    virtual UINT GetClassStyle() const { return UI_CLASSSTYLE_CHILD; }

public:
    HWND Create(HWND hwndParent, LPCTSTR pstrName);
    HWND Create(HWND hwndParent, LPCTSTR pstrName, DWORD dwStyle, DWORD dwExStyle, const RECT rc, HMENU hMenu = NULL);
    HWND Create(HWND hwndParent, LPCTSTR pstrName, DWORD dwStyle, DWORD dwExStyle, int x = CW_USEDEFAULT, int y = CW_USEDEFAULT, int cx = CW_USEDEFAULT, int cy = CW_USEDEFAULT, HMENU hMenu = NULL);

public:
    void SetWindowUI(CWindowUI* pWindowUI) { this->m_pWindowUI = pWindowUI; };
    CWindowUI* GetWindowUI() { return m_pWindowUI; };

private:
    CWindowUI* m_pWindowUI;
};

template<class T>
CDuiChildWindowImpl<T>::CDuiChildWindowImpl()
    : m_pWindowUI(NULL)
{

}

template<class T>
CDuiChildWindowImpl<T>::CDuiChildWindowImpl(CWindowUI* pWindowUI)
    : m_pWindowUI(pWindowUI)
{

}

template<class T>
CDuiChildWindowImpl<T>::~CDuiChildWindowImpl()
{

}

template<class T>
HWND CDuiChildWindowImpl<T>::Create(HWND hwndParent, LPCTSTR pstrName)
{
    return Create(hwndParent, pstrName, WS_VISIBLE | WS_CHILD | WS_CLIPSIBLINGS | WS_CLIPCHILDREN, 0L);
}

template<class T>
HWND CDuiChildWindowImpl<T>::Create(HWND hwndParent, LPCTSTR pstrName, DWORD dwStyle, DWORD dwExStyle, const RECT rc, HMENU hMenu /*= NULL*/)
{
    return Super::Create(hwndParent, pstrName, dwStyle, dwExStyle, rc, hMenu);
}

template<class T>
HWND CDuiChildWindowImpl<T>::Create(HWND hwndParent, LPCTSTR pstrName, DWORD dwStyle, DWORD dwExStyle, int x /*= CW_USEDEFAULT*/, int y /*= CW_USEDEFAULT*/, int cx /*= CW_USEDEFAULT*/, int cy /*= CW_USEDEFAULT*/, HMENU hMenu /*= NULL*/)
{
    return Super::Create(hwndParent, pstrName, dwStyle, dwExStyle, x, y, cx, cy, hMenu);
}

#endif