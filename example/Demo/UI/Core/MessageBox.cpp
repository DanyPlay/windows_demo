#include "stdafx.h"
#include "MessageBox.h"
#include "../../Constant.h"


CDuiMessageBox::CDuiMessageBox()
{
    m_strCaption = APP_TITLE;
    m_strText = _T("");
    m_strOk = _T("确定");
    m_strCancel = _T("取消");
    m_uType = MB_OKCANCEL;
}

CDuiMessageBox::CDuiMessageBox(LPCTSTR lpText, LPCTSTR lpCaption, UINT uType)
{
    m_strText = lpText != NULL ? lpText : _T("");
    m_strCaption = lpCaption != NULL ? lpCaption : APP_TITLE;
    m_strOk = _T("确定");
    m_strCancel = _T("取消");
    m_uType = uType;
}

LRESULT CDuiMessageBox::OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    CLabelUI* pTitleLabel = static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("lb_title")));
    if (pTitleLabel)
    {
        pTitleLabel->SetText(m_strCaption);
    }
    SetWindowText(m_strCaption);

    CLabelUI* pTextLabel = static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("lb_text")));
    if (pTextLabel)
    {
        pTextLabel->SetText(m_strText);
        pTextLabel->SetMultiLine(true);
    }

    CButtonUI* pOkBtn = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("btn_ok")));
    if (pOkBtn)
    {
        pOkBtn->SetText(m_strOk);
    }

    CButtonUI* pCancelBtn = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("btn_cancel")));
    if (pCancelBtn)
    {
        pCancelBtn->SetText(m_strCancel);

        BOOL bShow = FALSE;
        UINT uType = m_uType & 0x0000000F;
        if (uType == MB_OKCANCEL || uType == MB_RETRYCANCEL || uType == MB_YESNO)
        {
            bShow = TRUE;
        }
        pCancelBtn->SetVisible(bShow ? true : false);
    }

    return 0;
}

void CDuiMessageBox::OnClick(TNotifyUI& msg)
{
    CDuiString sCtrlName = msg.pSender->GetName();
    if (sCtrlName == _T("closebtn") || sCtrlName == _T("btn_cancel"))
    {
        UINT nResultID = IDCANCEL;
        UINT uType = m_uType & 0x0000000FL;
        if (uType == MB_OKCANCEL)
        {
            nResultID = IDCANCEL;
        }
        else if (uType == MB_YESNO)
        {
            nResultID = IDNO;
        }
        PostMessage(WM_CLOSE, nResultID, 0);
        return;
    }
    else if (sCtrlName == _T("btn_ok"))
    {
        UINT nResultID = IDOK;
        UINT uType = m_uType & 0x0000000FL;
        if (uType == MB_OKCANCEL || uType == MB_OK)
        {
            nResultID = IDOK;
        }
        else if (uType == MB_YESNOCANCEL || uType == MB_YESNO)
        {
            nResultID = IDYES;
        }
        PostMessage(WM_CLOSE, nResultID, 0);
        return;
    }

    Super::OnClick(msg);
}

void CDuiMessageBox::SetOkBtnText(LPCTSTR lpText)
{
    if (lpText)
    {
        m_strOk = lpText;
    }
}

void CDuiMessageBox::SetCancelBtnText(LPCTSTR lpText)
{
    if (lpText)
    {
        m_strCancel = lpText;
    }
}

INT DuiMessageBox(HWND hWnd, LPCTSTR lpText, LPCTSTR lpCaption, UINT uType)
{
    CDuiMessageBox dlg(lpText, lpCaption, uType);
    return dlg.DoModal(hWnd);
}
