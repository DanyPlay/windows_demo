#include "stdafx.h"
#include "InteractSettingPage.h"
#include "../ModelDef.h"
#include "../InteractSetting.h"

int CInteractSettingPage::m_dataProfileModes[] =
{
    QHVC::INTERACT::VIDEO_PROFILE::VIDEO_PROFILE_240P_3, // 240x240   15   140
    QHVC::INTERACT::VIDEO_PROFILE::VIDEO_PROFILE_240P_4, // 424x240   15   220
    QHVC::INTERACT::VIDEO_PROFILE::VIDEO_PROFILE_360P_3, // 360x360   15   260
    QHVC::INTERACT::VIDEO_PROFILE::VIDEO_PROFILE_360P_6, // 360x360   30   400
    QHVC::INTERACT::VIDEO_PROFILE::VIDEO_PROFILE_360P,   // 640x360   15   400
    QHVC::INTERACT::VIDEO_PROFILE::VIDEO_PROFILE_360P_4, // 640x360   30   600 （声网） // 640x360   15   600 （即构）
    QHVC::INTERACT::VIDEO_PROFILE::VIDEO_PROFILE_360P_9, // 640x360   15   800
    QHVC::INTERACT::VIDEO_PROFILE::VIDEO_PROFILE_480P_3, // 480x480   15   400
    QHVC::INTERACT::VIDEO_PROFILE::VIDEO_PROFILE_480P_6, // 480x480   30   600
    QHVC::INTERACT::VIDEO_PROFILE::VIDEO_PROFILE_480P_8, // 848x480   15   610
    QHVC::INTERACT::VIDEO_PROFILE::VIDEO_PROFILE_480P_9, // 848x480   30   930
    QHVC::INTERACT::VIDEO_PROFILE::VIDEO_PROFILE_720P,   // 1280x720  15   1130
    QHVC::INTERACT::VIDEO_PROFILE::VIDEO_PROFILE_720P_3, // 1280x720  30   1710
    0                                                    // 自定义
};

LPCTSTR CInteractSettingPage::m_dataProfileResolutions[] =
{
    _T("240x240"),
    _T("424x240"),
    _T("360x360"),
    _T("360x360"),
    _T("640x360"),
    _T("640x360"),
    _T("640x360"),
    _T("480x480"),
    _T("480x480"),
    _T("848x480"),
    _T("848x480"),
    _T("1280x720"),
    _T("1280x720")
};

int CInteractSettingPage::m_dataProfileFrameRates[] =
{
    15,
    15,
    15,
    30,
    15,
    15,
    15,
    15,
    30,
    15,
    30,
    15,
    30
};

int CInteractSettingPage::m_dataProfileBitrates[] =
{
    140,
    220,
    260,
    400,
    400,
    600,
    800,
    400,
    600,
    610,
    930,
    1130,
    1710
};

int CInteractSettingPage::m_dataFrameRates[] =
{
    15,
    20,
    25,
    30
};

DUI_BEGIN_MESSAGE_MAP(CInteractSettingPage, CDuiDialogImpl<CInteractSettingPage>)
DUI_ON_MSGTYPE(DUI_MSGTYPE_CLICK, OnClick)
DUI_ON_MSGTYPE(DUI_MSGTYPE_ITEMSELECT, OnItemSelect)
DUI_END_MESSAGE_MAP()

CInteractSettingPage::CInteractSettingPage()
: m_pCmbAnchorProfileMode(NULL)
, m_pEdtAnchorResolution(NULL)
, m_pCmbAnchorFrameRate(NULL)
, m_pEdtAnchorBitrate(NULL)
, m_pCmbGuestProfileMode(NULL)
, m_pEdtGuestResolution(NULL)
, m_pCmbGuestFrameRate(NULL)
, m_pEdtGuestBitrate(NULL)
{

}

CInteractSettingPage::~CInteractSettingPage()
{

}

LRESULT CInteractSettingPage::OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    m_pCmbAnchorProfileMode = static_cast<CComboUI*>(FindControl(_T("cmb_anchor_profile_mode")));
    m_pEdtAnchorResolution = static_cast<CEditUI*>(FindControl(_T("edt_anchor_resolution")));
    m_pCmbAnchorFrameRate = static_cast<CComboUI*>(FindControl(_T("cmb_anchor_frame_rate")));
    m_pEdtAnchorBitrate = static_cast<CEditUI*>(FindControl(_T("edt_anchor_bitrate")));

    m_pCmbGuestProfileMode = static_cast<CComboUI*>(FindControl(_T("cmb_guest_profile_mode")));
    m_pEdtGuestResolution = static_cast<CEditUI*>(FindControl(_T("edt_guest_resolution")));
    m_pCmbGuestFrameRate = static_cast<CComboUI*>(FindControl(_T("cmb_guest_frame_rate")));
    m_pEdtGuestBitrate = static_cast<CEditUI*>(FindControl(_T("edt_guest_bitrate")));

    InitView();
    InitData();

    return 0;
}

LRESULT CInteractSettingPage::OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    return 0;
}

void CInteractSettingPage::InitView()
{
    TCHAR szBuffer[32] = { 0 };

    //主播-模式
    for (unsigned int i = 0, length = ARRAYSIZE(m_dataProfileModes); i < length; i++)
    {
        if (m_dataProfileModes[i] != 0)
            _sntprintf_s(szBuffer, ARRAYSIZE(szBuffer), _TRUNCATE, _T("模式%d"), i);
        else
            _sntprintf_s(szBuffer, ARRAYSIZE(szBuffer), _TRUNCATE, _T("自定义"));

        CListLabelElementUI* pElementUI = new CListLabelElementUI();
        pElementUI->SetText(szBuffer);
        pElementUI->SetTag(m_dataProfileModes[i]);
        m_pCmbAnchorProfileMode->Add(pElementUI);
    }

    //嘉宾-模式
    for (unsigned int i = 0, length = ARRAYSIZE(m_dataProfileModes); i < length; i++)
    {
        if (m_dataProfileModes[i] != 0)
            _sntprintf_s(szBuffer, ARRAYSIZE(szBuffer), _TRUNCATE, _T("模式%d"), i);
        else
            _sntprintf_s(szBuffer, ARRAYSIZE(szBuffer), _TRUNCATE, _T("自定义"));

        CListLabelElementUI* pElementUI = new CListLabelElementUI();
        pElementUI->SetText(szBuffer);
        pElementUI->SetTag(m_dataProfileModes[i]);
        m_pCmbGuestProfileMode->Add(pElementUI);
    }
}

void CInteractSettingPage::InitView(CComboUI* pComboUI, int** data, unsigned int length)
{
    if (!pComboUI || !data || length <= 0)
        return;

    pComboUI->RemoveAll();

    TCHAR szBuffer[32] = { 0 };
    for (unsigned int i = 0; i < length; i++)
    {
        _sntprintf_s(szBuffer, ARRAYSIZE(szBuffer), _TRUNCATE, _T("%d"), data[i]);

        CListLabelElementUI* pElementUI = new CListLabelElementUI();
        pElementUI->SetText(szBuffer);
        pElementUI->SetTag((UINT_PTR)data[i]);
        pComboUI->Add(pElementUI);
    }
}

void CInteractSettingPage::InitData()
{
    InteractSetting& setting = InteractSetting::GetInstance();

    //主播-模式
    int nAnchorProfileMode = setting.GetProfileMode(InteractConstant::USER_IDENTITY_ANCHOR);
    int nAnchorProfileModeIndex = Find(m_dataProfileModes, ARRAYSIZE(m_dataProfileModes), nAnchorProfileMode, 0);
    m_pCmbAnchorProfileMode->SelectItem(nAnchorProfileModeIndex);

    //主播-分辨率
    if (nAnchorProfileMode == 0)
    {
        int nResolutionWidth = setting.GetResolutionWidth(InteractConstant::USER_IDENTITY_ANCHOR);
        int nResolutionHeight = setting.GetResolutionHeight(InteractConstant::USER_IDENTITY_ANCHOR);
        m_pEdtAnchorResolution->SetText(STR_A2T(Util::String::StrFormat("%dx%d", nResolutionWidth, nResolutionHeight)).c_str());
    }

    //主播-帧率
    if (nAnchorProfileMode == 0)
    {
        int nFrameRate = setting.GetFrameRate(InteractConstant::USER_IDENTITY_ANCHOR);
        int nFrameRateIndex = Find(m_dataFrameRates, ARRAYSIZE(m_dataFrameRates), nFrameRate, 0);
        m_pCmbAnchorFrameRate->SelectItem(nFrameRateIndex);
    }

    //主播-码率
    if (nAnchorProfileMode == 0)
    {
        int nBitrate = setting.GetBitrate(InteractConstant::USER_IDENTITY_ANCHOR);
        m_pEdtAnchorBitrate->SetText(Int2Str(nBitrate).c_str());
    }

    //嘉宾-模式
    int nGuestProfileMode = setting.GetProfileMode(InteractConstant::USER_IDENTITY_GUEST);
    int nGuestProfileModeIndex = Find(m_dataProfileModes, ARRAYSIZE(m_dataProfileModes), nGuestProfileMode, 0);
    m_pCmbGuestProfileMode->SelectItem(nGuestProfileModeIndex);

    //嘉宾-分辨率
    if (nGuestProfileMode == 0)
    {
        int nResolutionWidth = setting.GetResolutionWidth(InteractConstant::USER_IDENTITY_GUEST);
        int nResolutionHeight = setting.GetResolutionHeight(InteractConstant::USER_IDENTITY_GUEST);
        m_pEdtGuestResolution->SetText(STR_A2T(Util::String::StrFormat("%dx%d", nResolutionWidth, nResolutionHeight)).c_str());
    }

    //嘉宾-帧率
    if (nGuestProfileMode == 0)
    {
        int nFrameRate = setting.GetFrameRate(InteractConstant::USER_IDENTITY_GUEST);
        int nFrameRateIndex = Find(m_dataFrameRates, ARRAYSIZE(m_dataFrameRates), nFrameRate, 0);
        m_pCmbGuestFrameRate->SelectItem(nFrameRateIndex);
    }

    //嘉宾-码率
    if (nGuestProfileMode == 0)
    {
        int nBitrate = setting.GetBitrate(InteractConstant::USER_IDENTITY_GUEST);
        m_pEdtGuestBitrate->SetText(Int2Str(nBitrate).c_str());
    }
}

void CInteractSettingPage::OnClick(TNotifyUI& msg)
{
    CDuiString sCtrlName = msg.pSender->GetName();

    if (sCtrlName == _T("btn_reset"))
    {
        OnResetClick();
    }
    else if (sCtrlName == _T("btn_save"))
    {
        bool ret = OnSaveClick();
        if (!ret) {
            DuiMessageBox(m_hWnd, _T("参数错误！"), INTERACT_TITLE, MB_OK);
        }
    }

    Super::OnClick(msg);
}

void CInteractSettingPage::OnItemSelect(TNotifyUI& msg)
{
    CDuiString sCtrlName = msg.pSender->GetName();

    if (sCtrlName == _T("cmb_anchor_profile_mode"))
    {
        OnAnchorProfileModeSelect();
    }
    else if (sCtrlName == _T("cmb_guest_profile_mode"))
    {
        OnGuestProfileModeSelect();
    }
}

void CInteractSettingPage::OnAnchorProfileModeSelect()
{
    int nIndex = m_pCmbAnchorProfileMode->GetCurSel();

    if (nIndex <= ARRAYSIZE(m_dataProfileModes) - 2)
    {
        m_pEdtAnchorResolution->SetEnabled(false);
        m_pCmbAnchorFrameRate->SetEnabled(false);
        m_pEdtAnchorBitrate->SetEnabled(false);

        InitView(m_pCmbAnchorFrameRate, (int**)&m_dataProfileFrameRates, ARRAYSIZE(m_dataProfileFrameRates));

        m_pEdtAnchorResolution->SetText(m_dataProfileResolutions[nIndex]);
        m_pCmbAnchorFrameRate->SelectItem(nIndex);
        m_pEdtAnchorBitrate->SetText(Int2Str(m_dataProfileBitrates[nIndex]).c_str());
    }
    else
    {
        m_pEdtAnchorResolution->SetEnabled(true);
        m_pCmbAnchorFrameRate->SetEnabled(true);
        m_pEdtAnchorBitrate->SetEnabled(true);

        int nTmpFrameRateIndex = m_pCmbAnchorFrameRate->GetCurSel();
        if (nTmpFrameRateIndex >= 0)
        {
            int nTmpFrameRateValue = (int)m_pCmbAnchorFrameRate->GetItemAt(nTmpFrameRateIndex)->GetTag();
            InitView(m_pCmbAnchorFrameRate, (int**)&m_dataFrameRates, ARRAYSIZE(m_dataFrameRates));
            for (unsigned int i = 0, length = ARRAYSIZE(m_dataFrameRates); i < length; i++)
            {
                if (m_dataFrameRates[i] == nTmpFrameRateValue)
                {
                    m_pCmbAnchorFrameRate->SelectItem(i);
                    break;
                }
            }
        }
        else
        {
            InitView(m_pCmbAnchorFrameRate, (int**)&m_dataFrameRates, ARRAYSIZE(m_dataFrameRates));
        }
    }
}

void CInteractSettingPage::OnGuestProfileModeSelect()
{
    int nIndex = m_pCmbGuestProfileMode->GetCurSel();

    if (nIndex <= ARRAYSIZE(m_dataProfileModes) - 2)
    {
        m_pEdtGuestResolution->SetEnabled(false);
        m_pCmbGuestFrameRate->SetEnabled(false);
        m_pEdtGuestBitrate->SetEnabled(false);

        InitView(m_pCmbGuestFrameRate, (int**)&m_dataProfileFrameRates, ARRAYSIZE(m_dataProfileFrameRates));

        m_pEdtGuestResolution->SetText(m_dataProfileResolutions[nIndex]);
        m_pCmbGuestFrameRate->SelectItem(nIndex);
        m_pEdtGuestBitrate->SetText(Int2Str(m_dataProfileBitrates[nIndex]).c_str());
    }
    else
    {
        m_pEdtGuestResolution->SetEnabled(true);
        m_pCmbGuestFrameRate->SetEnabled(true);
        m_pEdtGuestBitrate->SetEnabled(true);

        int nTmpFrameRateIndex = m_pCmbGuestFrameRate->GetCurSel();
        if (nTmpFrameRateIndex >= 0)
        {
            int nTmpFrameRateValue = (int)m_pCmbGuestFrameRate->GetItemAt(nTmpFrameRateIndex)->GetTag();
            InitView(m_pCmbGuestFrameRate, (int**)&m_dataFrameRates, ARRAYSIZE(m_dataFrameRates));
            for (unsigned int i = 0, length = ARRAYSIZE(m_dataFrameRates); i < length; i++)
            {
                if (m_dataFrameRates[i] == nTmpFrameRateValue)
                {
                    m_pCmbGuestFrameRate->SelectItem(i);
                    break;
                }
            }
        }
        else
        {
            InitView(m_pCmbGuestFrameRate, (int**)&m_dataFrameRates, ARRAYSIZE(m_dataFrameRates));
        }
    }
}

void CInteractSettingPage::OnResetClick()
{
    m_pCmbAnchorProfileMode->SelectItem(6);
    m_pCmbGuestProfileMode->SelectItem(6);
}

bool CInteractSettingPage::OnSaveClick()
{
    //TODO 参数校验

    InteractSetting& setting = InteractSetting::GetInstance();
    tstring strAchorResolution = m_pEdtAnchorResolution->GetText();
    MakeLower(strAchorResolution);
    int idx = strAchorResolution.find(_T("x"));
    if (idx <= 0) {
        return false;
    }
    int nAnhorResolutionWidth = Str2Int(strAchorResolution.substr(0, idx));
    int nAnhorResolutionHeight = Str2Int(strAchorResolution.substr(idx + 1, -1));
    if (nAnhorResolutionWidth <= 0 || nAnhorResolutionHeight <= 0) {
        return false;
    }

    int nAnchorBitrate = Str2Int((LPCTSTR)m_pEdtAnchorBitrate->GetText());
    if (nAnchorBitrate <= 0) {
        return false;
    }

    tstring strGuestResolution = m_pEdtGuestResolution->GetText();
    MakeLower(strGuestResolution);
    idx = strGuestResolution.find(_T("x"));
    if (idx <= 0) {
        return false;
    }

    int nGuestResolutionWidth = Str2Int(strGuestResolution.substr(0, idx));
    int nGuestResolutionHeight = Str2Int(strGuestResolution.substr(idx + 1, -1));
    if (nGuestResolutionWidth <= 0 || nGuestResolutionHeight <= 0) {
        return false;
    }

    int nGuestBitrate = Str2Int((LPCTSTR)m_pEdtGuestBitrate->GetText());
    if (nGuestBitrate <= 0) {
        return false;
    }

    //主播
    setting.SetProfileMode(InteractConstant::USER_IDENTITY_ANCHOR, m_dataProfileModes[m_pCmbAnchorProfileMode->GetCurSel()]);

    setting.SetResolutionWidth(InteractConstant::USER_IDENTITY_ANCHOR, nAnhorResolutionWidth);
    setting.SetResolutionHeight(InteractConstant::USER_IDENTITY_ANCHOR, nAnhorResolutionHeight);
    setting.SetFrameRate(InteractConstant::USER_IDENTITY_ANCHOR, m_dataProfileFrameRates[m_pCmbAnchorFrameRate->GetCurSel()]);
    setting.SetBitrate(InteractConstant::USER_IDENTITY_ANCHOR, nAnchorBitrate);

    //嘉宾
    setting.SetProfileMode(InteractConstant::USER_IDENTITY_GUEST, m_dataProfileModes[m_pCmbGuestProfileMode->GetCurSel()]);
    setting.SetResolutionWidth(InteractConstant::USER_IDENTITY_GUEST, nGuestResolutionWidth);
    setting.SetResolutionHeight(InteractConstant::USER_IDENTITY_GUEST, nGuestResolutionHeight);
    setting.SetFrameRate(InteractConstant::USER_IDENTITY_GUEST, m_dataProfileFrameRates[m_pCmbGuestFrameRate->GetCurSel()]);
    setting.SetBitrate(InteractConstant::USER_IDENTITY_GUEST, nGuestBitrate);

    return true;
}

tstring CInteractSettingPage::Int2Str(int value)
{
    TCHAR buffer[32] = { 0 };
    _sntprintf_s(buffer, ARRAYSIZE(buffer), _TRUNCATE, _T("%d"), value);

    return buffer;
}

int CInteractSettingPage::Str2Int(const tstring& value)
{
    return _ttoi(value.c_str());
}

int CInteractSettingPage::Find(int* data, unsigned int length, int value, int default_)
{
    if (!data || length <= 0)
        return default_;

    for (unsigned int i = 0; i < length; i++)
    {
        if (data[i] == value)
        {
            return (int)i;
        }
    }

    return default_;
}

tstring& CInteractSettingPage::MakeLower(tstring &s)
{
    if (s.empty())
    {
        return s;
    }

    TCHAR* pBuf = new TCHAR[s.length() + 1];
    _tcsncpy_s(pBuf, s.length() + 1, s.c_str(), _TRUNCATE);
#if _MSC_VER < 1500
    _tcslwr(pBuf);
#else
    _tcslwr_s(pBuf, s.length() + 1);
#endif
    s = pBuf;
    delete[] pBuf;

    return s;
}
