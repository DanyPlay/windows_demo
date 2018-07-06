#include "stdafx.h"
#include "InteractHomePage.h"
#include "../InteractGlobalManager.h"
#include "../IM/InteractImManager.h"
#include "../Net/InteractServerApi.h"
#include "../../CommonLib/md5/md5.h"
#include "../InteractSetting.h"


DUI_BEGIN_MESSAGE_MAP(CInteractHomePage, CDuiDialogImpl<CInteractHomePage>)
DUI_ON_MSGTYPE(DUI_MSGTYPE_CLICK, OnClick)
DUI_END_MESSAGE_MAP()

CInteractHomePage::CInteractHomePage()
: m_pEdtBid(NULL)
, m_pEdtCid(NULL)
, m_pEdtAppKey(NULL)
, m_pEdtAppSecret(NULL)
, m_pEdtUid(NULL)
{

}

CInteractHomePage::~CInteractHomePage()
{

}

LRESULT CInteractHomePage::OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    m_pEdtBid = static_cast<CEditUI*>(FindControl(_T("edt_bid")));
    m_pEdtCid = static_cast<CEditUI*>(FindControl(_T("edt_cid")));
    m_pEdtAppKey = static_cast<CEditUI*>(FindControl(_T("edt_appkey")));
    m_pEdtAppSecret = static_cast<CEditUI*>(FindControl(_T("edt_appsecret")));
    m_pEdtUid = static_cast<CEditUI*>(FindControl(_T("edt_uid")));

    m_pEdtBid->SetText(InteractSetting::GetInstance().GetBid().c_str());
    m_pEdtCid->SetText(InteractSetting::GetInstance().GetCid().c_str());
    m_pEdtAppKey->SetText(InteractSetting::GetInstance().GetAppKey().c_str());
    m_pEdtAppSecret->SetText(InteractSetting::GetInstance().GetAppSecret().c_str());

    m_pLayoutLogin = static_cast<CContainerUI*>(FindControl(_T("layout_login")));

    InitData();

    InteractServerApi::GetInstance().AddApiCallback(this);

    return 0;
}

LRESULT CInteractHomePage::OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    InteractServerApi::GetInstance().RemoveApiCallback(this);

    return 0;
}

void CInteractHomePage::OnClick(TNotifyUI& msg)
{
    CDuiString sCtrlName = msg.pSender->GetName();
    if (sCtrlName == _T("btn_login"))
    {
        OnLoginClick();
        return;
    }

    Super::OnClick(msg);
}

void CInteractHomePage::InitData()
{
    InteractUserModel* userModel = InteractGlobalManager::GetInstance().GetInteractUserModel();
    if (!userModel)
    {
        return;
    }

    m_pEdtUid->SetText(userModel->userid.c_str());

    m_pLayoutLogin->SetVisible(!InteractGlobalManager::GetInstance().IsLogin());
}

void CInteractHomePage::OnLoginClick()
{
    CDuiString strBid = m_pEdtBid->GetText();
    if (strBid.IsEmpty())
    {
        DuiMessageBox(m_hWnd, _T("业务标识不能为空"), INTERACT_TITLE, MB_OK);
        return;
    }

    CDuiString strCid = m_pEdtCid->GetText();
    if (strCid.IsEmpty())
    {
        DuiMessageBox(m_hWnd, _T("渠道标识不能为空"), INTERACT_TITLE, MB_OK);
        return;
    }

    CDuiString strAppKey = m_pEdtAppKey->GetText();
    if (strAppKey.IsEmpty())
    {
        DuiMessageBox(m_hWnd, _T("AppKey不能为空"), INTERACT_TITLE, MB_OK);
        return;
    }

    CDuiString strAppSecret = m_pEdtAppSecret->GetText();
    if (strAppSecret.IsEmpty())
    {
        DuiMessageBox(m_hWnd, _T("AppSecret不能为空"), INTERACT_TITLE, MB_OK);
        return;
    }

    InteractSetting::GetInstance().SetBid((LPCTSTR)strBid);
    InteractSetting::GetInstance().SetCid((LPCTSTR)strCid);
    InteractSetting::GetInstance().SetAppKey((LPCTSTR)strAppKey);
    InteractSetting::GetInstance().SetAppSecret((LPCTSTR)strAppSecret);

    InteractGlobalManager::GetInstance().SetBusinessId(strBid.GetData());
    InteractGlobalManager::GetInstance().SetChannelId(strCid.GetData());

    InteractServerApi::GetInstance().UserLogin();
}

void CInteractHomePage::OnServerApiUserLoginResult(const InteractServerApiRet& ret, const InteractUserModel& interactUserModel)
{
    if (ret.errCode != 0)
    {
        if (IsWindowVisible(m_hWnd) && IsWindowEnabled(m_hWnd)) {
            TCHAR szMsg[MAX_PATH] = { 0 };
            _sntprintf_s(szMsg, ARRAYSIZE(szMsg), _T("错误(%d)：%s"), ret.errCode, ret.errMessage.c_str());
            DuiMessageBox(m_hWnd, szMsg, INTERACT_TITLE, MB_OK);
        }

        return;
    }

    //视频云SDK初始化
    {
        char szPath[MAX_PATH] = { 0 };
        ::SHGetSpecialFolderPathA(NULL, szPath, CSIDL_APPDATA, TRUE);
        ::PathAddBackslashA(szPath);
        ::PathAppendA(szPath, APP_DIR_ROOT);
        ::PathAddBackslashA(szPath);

        QHVC::QHVCSdk::SetLogLevel(QHVC::QHVCSdk::LOG_LEVEL_TRACE);

        QHVC::QHVCSdk::QHVCSdkConfig sdkConfig;
        _snprintf_s(sdkConfig.szMachineId, ARRAYSIZE(sdkConfig.szMachineId), _TRUNCATE, "%s", STR2A(InteractGlobalManager::GetInstance().GetDeviceId()).c_str());
        _snprintf_s(sdkConfig.szAppVersion, ARRAYSIZE(sdkConfig.szAppVersion), _TRUNCATE, "%s", STR2A(Util::File::GetFileVersion(_Module.GetModuleInstance())).c_str());
        _snprintf_s(sdkConfig.szUserId, ARRAYSIZE(sdkConfig.szUserId), _TRUNCATE, STR2A(interactUserModel.userid).c_str());
        _snprintf_s(sdkConfig.szCacheDir, ARRAYSIZE(sdkConfig.szCacheDir), _TRUNCATE, "%s", szPath);
        _snprintf_s(sdkConfig.szBusinessId, ARRAYSIZE(sdkConfig.szBusinessId), _TRUNCATE, STR2A((LPCTSTR)m_pEdtBid->GetText()).c_str());
        QHVC::QHVCSdk::Init(&sdkConfig);
    }

    InteractGlobalManager::GetInstance().SetAppKey(tstring(m_pEdtAppKey->GetText()));
    InteractGlobalManager::GetInstance().SetAppSecret(tstring(m_pEdtAppSecret->GetText()));
    InteractGlobalManager::GetInstance().SetInteractUserModel(interactUserModel);

    InitData();

    InteractImManager::GetInstance().Init();
    InteractImManager::GetInstance().Connect();
}

void CInteractHomePage::OnImConnectStatusChanged(ImConstant::CONNECT_STATUS connectStatus)
{

}
