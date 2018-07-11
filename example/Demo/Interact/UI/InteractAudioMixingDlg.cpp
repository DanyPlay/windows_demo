#include "stdafx.h"
#include "InteractAudioMixingDlg.h"
#include "../ModelDef.h"


CInteractAudioMixingDlg::CInteractAudioMixingDlg()
    : m_pEdtFilePath(NULL)
    , m_pEdtCycle(NULL)
    , m_pOptMix(NULL)
    , m_pOptLookback(NULL)
{

}

CInteractAudioMixingDlg::~CInteractAudioMixingDlg()
{

}

LRESULT CInteractAudioMixingDlg::OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    m_pEdtFilePath = static_cast<CEditUI*>(FindControl(_T("edt_file_path")));
    m_pEdtFilePath->SetText(STR2T(m_oConfig.filePath.c_str()).c_str());

    m_pEdtCycle = static_cast<CEditUI*>(FindControl(_T("edt_cycle")));
    //m_pEdtCycle->SetNumberOnly(true);
    {
        TCHAR szBuf[64] = { 0 };
        _sntprintf(szBuf, ARRAYSIZE(szBuf), _T("%d"), m_oConfig.cycle);
        m_pEdtCycle->SetText(szBuf);
    }

    m_pOptMix = static_cast<COptionUI*>(FindControl(_T("opt_mix")));
    m_pOptMix->Selected(!m_oConfig.replace, false);

    m_pOptLookback = static_cast<COptionUI*>(FindControl(_T("opt_lookback")));
    m_pOptLookback->Selected(!m_oConfig.loopback, false);

    return 0;
}

void CInteractAudioMixingDlg::OnClick(TNotifyUI& msg)
{
    CDuiString sCtrlName = msg.pSender->GetName();

    if (sCtrlName == _T("btn_ok"))
    {
        DoOk();
    }
    else if (sCtrlName == _T("btn_cancel") || sCtrlName == _T("closebtn"))
    {
        PostMessage(WM_CLOSE, IDCANCEL, 0);
    }
    else if (sCtrlName == _T("btn_browser"))
    {
        DoBrowserFile();
    }
}

void CInteractAudioMixingDlg::DoBrowserFile()
{
    TCHAR lpFile[MAX_PATH + 1] = { 0 };

    OPENFILENAME ofn;
    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.lpstrFile = lpFile;
    ofn.hwndOwner = m_hWnd;
    ofn.nMaxFile = MAX_PATH;
    ofn.lpstrFilter = TEXT("All Formats (*.mp3;*.wma;*.wmv;*.mid;*.aac)\0*.mp3;*.wma;*.wav;*.mid;*.aac\0");
    ofn.nFilterIndex = 1;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

    TCHAR curDirectory[MAX_PATH + 1];
    GetCurrentDirectory(MAX_PATH, curDirectory);

    BOOL bOpenFile = GetOpenFileName(&ofn);
    SetCurrentDirectory(curDirectory);

    if (bOpenFile)
        m_pEdtFilePath->SetText(lpFile);
}

void CInteractAudioMixingDlg::DoOk()
{
    CDuiString strFilePath = m_pEdtFilePath->GetText();
    if (strFilePath.IsEmpty())
    {
        DuiMessageBox(m_hWnd, _T("文件路径不能为空"), INTERACT_TITLE, MB_OK);
        return;
    }

    CDuiString strCycle = m_pEdtCycle->GetText();
    if (strCycle.IsEmpty())
    {
        DuiMessageBox(m_hWnd, _T("循环次数不能为空"), INTERACT_TITLE, MB_OK);
        return;
    }

    m_oConfig.filePath = STR2A(strFilePath.GetData());
    m_oConfig.cycle = _ttoi(strCycle.GetData());
    m_oConfig.replace = !m_pOptMix->IsSelected();
    m_oConfig.loopback = !m_pOptLookback->IsSelected();

    PostMessage(WM_CLOSE, IDOK, NULL);
}
