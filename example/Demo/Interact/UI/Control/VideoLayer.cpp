#include "stdafx.h"
#include "VideoLayer.h"
#include "../InteractUIConstant.h"

DUI_BEGIN_MESSAGE_MAP(CVideoLayer, CDuiChildWindowImpl<CVideoLayer>)
DUI_ON_MSGTYPE(DUI_MSGTYPE_CLICK, OnClick)
DUI_END_MESSAGE_MAP()

CVideoLayer::CVideoLayer()
:m_hNotifyWnd(NULL)
, m_nTag(0)
{
}

CVideoLayer::~CVideoLayer()
{
}

LRESULT CVideoLayer::OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    return 0;
}

LRESULT CVideoLayer::OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    return 0;
}

void CVideoLayer::OnClick(TNotifyUI& msg)
{
    CDuiString sCtrlName = msg.pSender->GetName();
    if (sCtrlName == _T("btn_more")) {
        ::PostMessage(m_hNotifyWnd, WM_TOOLBAR_SHOW, (WPARAM)m_nTag, NULL);
    }
    else if (sCtrlName == _T("btn_kickout")) {
        ::PostMessage(m_hNotifyWnd, WM_TOOLBAR_KICKOUT_GUEST, (WPARAM)m_nTag, NULL);
    }
    else if (sCtrlName == _T("chk_video")) {
        CCheckBoxUI *pCheck = static_cast<CCheckBoxUI*>(msg.pSender);
        ::PostMessage(m_hNotifyWnd, WM_TOOLBAR_MUTE_VIDEO, (WPARAM)m_nTag, (LPARAM)(pCheck ? !pCheck->IsSelected() : false));
    }
    else if (sCtrlName == _T("chk_audio")) {
        CCheckBoxUI *pCheck = static_cast<CCheckBoxUI*>(msg.pSender);
        ::PostMessage(m_hNotifyWnd, WM_TOOLBAR_MUTE_AUDIO, (WPARAM)m_nTag, (LPARAM)(pCheck ? !pCheck->IsSelected() : false));
    }
    else if (sCtrlName == _T("chk_micro")) {
        CCheckBoxUI *pCheck = static_cast<CCheckBoxUI*>(msg.pSender);
        ::PostMessage(m_hNotifyWnd, WM_TOOLBAR_MUTE_MICRO, (WPARAM)m_nTag, (LPARAM)(pCheck ? !pCheck->IsSelected() : false));
    }
    else if (sCtrlName == _T("chk_screen")) {
        CCheckBoxUI *pCheck = static_cast<CCheckBoxUI*>(msg.pSender);
        ::PostMessage(m_hNotifyWnd, WM_TOOLBAR_SCREEN, (WPARAM)m_nTag, (LPARAM)(pCheck ? !pCheck->IsSelected() : false));
    }
    else if (sCtrlName == _T("chk_audio_mixing")) {
        CCheckBoxUI *pCheck = static_cast<CCheckBoxUI*>(msg.pSender);
        ::PostMessage(m_hNotifyWnd, WM_TOOLBAR_AUDIO_MIXING, (WPARAM)m_nTag, (LPARAM)(pCheck ? !pCheck->IsSelected() : false));
    }

    Super::OnClick(msg);
}

void CVideoLayer::SetNotifyWnd(HWND hwnd)
{
    m_hNotifyWnd = hwnd;
}

void CVideoLayer::SetTag(int tag)
{
    m_nTag = tag;
}
