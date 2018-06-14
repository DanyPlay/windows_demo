#include "stdafx.h"
#include "VideoWnd.h"


CVideoWnd::CVideoWnd()
{
    m_pWindowUI = NULL;
}

CVideoWnd::~CVideoWnd()
{
}

LRESULT CVideoWnd::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    HDC hdc;
    if (uMsg == WM_PAINT) {
        PAINTSTRUCT ps;
        hdc = BeginPaint(GetHWND(), &ps); //使无效区域变得有效，并填充ps结构
        // TODO: Add any drawing code here...

        int iSaveDC = ::SaveDC(hdc);

        //绘制一个蓝色椭圆，ps.rcPaint保存了客户区矩形
        HBRUSH hbrush = ::CreateSolidBrush(RGB(50, 50, 50));
        ::SelectObject(hdc, hbrush);
        ::FillRect(hdc, &ps.rcPaint, hbrush);
        ::DeleteObject(hbrush);

        ::RestoreDC(hdc, iSaveDC);

        EndPaint(GetHWND(), &ps);

        return 0;
    }  if (uMsg == WM_SIZE || uMsg == WM_MOVE) {
        OnMove();
    }

    return CWindowWnd::HandleMessage(uMsg, wParam, lParam);
}

void CVideoWnd::OnFinalMessage(HWND hWnd)
{
    CWindowWnd::OnFinalMessage(hWnd);
}

void CVideoWnd::CreateEx(HWND hParent, CWindowUI* pWindowUI)
{
    HWND hWnd = Create(hParent, NULL, WS_CHILDWINDOW | WS_VISIBLE | WS_EX_LTRREADING | WS_EX_RIGHTSCROLLBAR | WS_CLIPCHILDREN, 0L);
    ShowWindow();

    m_wndLayer.Create(GetHWND(), NULL, WS_POPUP | WS_EX_LTRREADING | WS_EX_RIGHTSCROLLBAR, 0);

    m_pWindowUI = pWindowUI;

    if (pWindowUI != NULL) {
        pWindowUI->Attach(GetHWND());
    }
}

void CVideoWnd::DestroyEx()
{
    if (m_pWindowUI != NULL) {
        m_pWindowUI->Detach();
        m_pWindowUI = NULL;
    }

    DestroyWindow(GetHWND());
}

CWindowUI* CVideoWnd::GetCWindowUI()
{
    return m_pWindowUI;
}

void CVideoWnd::OnMove()
{
    RECT rcPaint;

    GetClientRect(GetHWND(), &rcPaint);
    POINT pt;
    pt.x = rcPaint.left;
    pt.y = rcPaint.top;
    int width = rcPaint.right - rcPaint.left;
    int height = rcPaint.bottom - rcPaint.top;
    ClientToScreen(GetHWND(), &pt);

    rcPaint.left = pt.x;
    rcPaint.top = pt.y;
    rcPaint.right = pt.x + width;
    rcPaint.bottom = pt.y + height;

    SetWindowPos(m_wndLayer.GetHWND(), NULL, rcPaint.left, rcPaint.top, rcPaint.right - rcPaint.left, rcPaint.bottom - rcPaint.top, 0);
}


CControlUI* CVideoWnd::FindSubControl(LPCTSTR pstrName) const
{
    return m_wndLayer.FindControl(pstrName);
}

void CVideoWnd::SetNotifyWnd(HWND hwnd)
{
    m_wndLayer.SetNotifyWnd(hwnd);
}

void CVideoWnd::SetTag(int tag)
{
    if (!IsWindowVisible(m_wndLayer.GetHWND())) {
        m_wndLayer.ShowWindow();
    }

    m_wndLayer.SetTag(tag);
}


