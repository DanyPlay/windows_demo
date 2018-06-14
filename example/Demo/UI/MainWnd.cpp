#include "stdafx.h"
#include "MainWnd.h"
#include "../Interact/UI/InteractMainDlg.h"

DUI_BEGIN_MESSAGE_MAP(CMainWnd, CDuiWindowImpl<CMainWnd>)
DUI_ON_MSGTYPE(DUI_MSGTYPE_CLICK, OnClick)
DUI_END_MESSAGE_MAP()

LRESULT CMainWnd::OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    SetWindowText(APP_TITLE);
    return 0;
}

LRESULT CMainWnd::OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    DestroyWindow(m_hWnd);
    return 0;
}

LRESULT CMainWnd::OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    PostQuitMessage(0);
    return 0;
}

void CMainWnd::OnClick(TNotifyUI& msg)
{
    CDuiString sCtrlName = msg.pSender->GetName();
    if (sCtrlName == _T("btn_player"))
    {
        INT nRet = DuiMessageBox(m_hWnd, _T("�ı����ݣ�����һ���Ƚϳ������ݣ���������Ч����Σ����ǲ��������ٶ������"), APP_TITLE, MB_OK);
        return;
    }
    else if (sCtrlName == _T("btn_interact"))
    {
        ShowWindow(false);
        CInteractMainDlg interactMainDlg;
        interactMainDlg.DoModal(NULL);
        ShowWindow(true);
    }

    Super::OnClick(msg);
}
