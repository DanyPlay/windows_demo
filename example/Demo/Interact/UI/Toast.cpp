#include "stdafx.h"
#include "Toast.h"
#include "../../Constant.h"


CDuiToast::CDuiToast()
{
    m_strText = _T("");
    m_hParentWnd = NULL;
    m_nDuration = 10;
}

CDuiToast::CDuiToast(HWND hParentWnd, LPCTSTR lpText, int nDuration)
{
    m_hParentWnd = hParentWnd;
    m_strText = lpText != NULL ? lpText : _T("");
    m_nDuration = nDuration;
}

LRESULT CDuiToast::OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{

    CLabelUI* pTextLabel = static_cast<CLabelUI*>(FindControl(_T("txt_toast")));
    if (pTextLabel)
    {
        pTextLabel->SetText(m_strText);
        SIZE szTxt = { 200, 54 };
        szTxt = pTextLabel->EstimateSize(szTxt);
        szTxt.cy = 54;
        if (szTxt.cx < 200) {
            szTxt.cx = 200;
        }
        pTextLabel->SetMultiLine(true);

        RECT rcParent;
        GetWindowRect(m_hParentWnd, &rcParent);

        POINT pt;
        pt.x = rcParent.left + (rcParent.right - rcParent.left - szTxt.cx) / 2;
        pt.y = rcParent.top + (rcParent.bottom - rcParent.top - szTxt.cy) / 2;

        ::SetWindowPos(GetHWND(), HWND_TOPMOST, pt.x, pt.y, szTxt.cx, szTxt.cy, SWP_SHOWWINDOW);
    }

    ::InvalidateRect(GetHWND(), NULL, TRUE);
    ::UpdateWindow(GetHWND());

    SetTimer(GetHWND(), 9999, m_nDuration * 1000, NULL);

    return 0;
}

LRESULT CDuiToast::OnTimer(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    if (wParam == 9999) {
        Close();
    }
    bHandled = false;

    return 0;
}

LRESULT CDuiToast::OnNcDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    delete this;
    return 0;
}

static CDuiToast* g_pDuiToast = NULL;

void DuiShowToast(HWND hParentWnd, LPCTSTR lpText, int nDuration)
{
    if (g_pDuiToast != NULL) {
        if (::IsWindow(g_pDuiToast->GetHWND())) {
            g_pDuiToast->Close();
        }
    }

    g_pDuiToast = new CDuiToast(hParentWnd, lpText, nDuration);
    if (g_pDuiToast) {
        g_pDuiToast->Create(hParentWnd, NULL, WS_POPUP | WS_VISIBLE | WS_EX_LTRREADING | WS_EX_RIGHTSCROLLBAR, 0);
    }
}