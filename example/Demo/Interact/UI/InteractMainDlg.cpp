#include "stdafx.h"
#include "InteractMainDlg.h"
#include "../InteractGlobalManager.h"


DUI_BEGIN_MESSAGE_MAP(CInteractMainDlg, CDuiDialogImpl<CInteractMainDlg>)
DUI_ON_MSGTYPE(DUI_MSGTYPE_CLICK, OnClick)
DUI_ON_MSGTYPE(DUI_MSGTYPE_SELECTCHANGED, OnSelectChanged)
DUI_END_MESSAGE_MAP()

CInteractMainDlg::CInteractMainDlg()
: m_pTabContainer(NULL)
, m_pPageHome(NULL)
, m_pPageRoom(NULL)
, m_pPageCreateRoom(NULL)
, m_pPageDevice(NULL)
, m_pPageSetting(NULL)
{

}

CInteractMainDlg::~CInteractMainDlg()
{

}

LRESULT CInteractMainDlg::OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    SetWindowText(INTERACT_TITLE);

    m_pTabContainer = static_cast<CTabLayoutUI*>(m_PaintManager.FindControl(_T("tab_container")));

    InteractServerApi::GetInstance().AddApiCallback(this);

    InteractGlobalManager::GetInstance().SetMainWindow(GetHWND());

    return 0;
}

LRESULT CInteractMainDlg::OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    InteractServerApi::GetInstance().RemoveApiCallback(this);
    InteractGlobalManager::GetInstance().SetMainWindow(NULL);

    for (std::map<INT, CWindowWnd*>::iterator itor = m_mapPages.begin(); itor != m_mapPages.end(); ++itor)
    {
        CWindowWnd* pWindowWnd = itor->second;
        if (pWindowWnd)
        {
            if (IsWindow(pWindowWnd->GetHWND()))
                DestroyWindow(pWindowWnd->GetHWND());
            delete pWindowWnd;
        }
    }

    m_mapPages.clear();

    m_pPageHome = NULL;
    m_pPageRoom = NULL;
    m_pPageCreateRoom = NULL;
    m_pPageDevice = NULL;
    m_pPageSetting = NULL;

    PostQuitMessage(0);
    return 0;
}

CControlUI* CInteractMainDlg::CreateControl(LPCTSTR pstrClass)
{
    if (_tcscmp(pstrClass, _T("InteractHomePage")) == 0)
    {
        CWindowUI* pWindowUI = new CWindowUI();

        m_pPageHome = new CInteractHomePage();
        m_pPageHome->Create(m_hWnd, NULL);
        m_pPageHome->SetWindowUI(pWindowUI);
        m_mapPages.insert(std::make_pair(MAIN_TAB_HOME, m_pPageHome));

        pWindowUI->Attach(m_pPageHome->GetHWND());
        return pWindowUI;
    }
    else if (_tcscmp(pstrClass, _T("InteractRoomPage")) == 0)
    {
        CWindowUI* pWindowUI = new CWindowUI();

        m_pPageRoom = new CInteractRoomPage();
        m_pPageRoom->Create(m_hWnd, NULL);
        m_pPageRoom->SetWindowUI(pWindowUI);
        m_mapPages.insert(std::make_pair(MAIN_TAB_ROOM, m_pPageRoom));

        pWindowUI->Attach(m_pPageRoom->GetHWND());
        return pWindowUI;
    }
    else if (_tcscmp(pstrClass, _T("InteractCreateRoomPage")) == 0)
    {
        CWindowUI* pWindowUI = new CWindowUI();

        m_pPageCreateRoom = new CInteractCreateRoomPage();
        m_pPageCreateRoom->Create(m_hWnd, NULL);
        m_pPageCreateRoom->SetWindowUI(pWindowUI);
        m_mapPages.insert(std::make_pair(MAIN_TAB_CREATE_ROOM, m_pPageCreateRoom));

        pWindowUI->Attach(m_pPageCreateRoom->GetHWND());
        return pWindowUI;
    }
    else if (_tcscmp(pstrClass, _T("InteractDevicePage")) == 0)
    {
        CWindowUI* pWindowUI = new CWindowUI();

        m_pPageDevice = new CInteractDevicePage();
        m_pPageDevice->Create(m_hWnd, NULL);
        m_pPageDevice->SetWindowUI(pWindowUI);
        m_mapPages.insert(std::make_pair(MAIN_TAB_DEVICE, m_pPageDevice));

        pWindowUI->Attach(m_pPageDevice->GetHWND());
        return pWindowUI;
    }
    else if (_tcscmp(pstrClass, _T("InteractSettingPage")) == 0)
    {
        CWindowUI* pWindowUI = new CWindowUI();

        m_pPageSetting = new CInteractSettingPage();
        m_pPageSetting->Create(m_hWnd, NULL);
        m_pPageSetting->SetWindowUI(pWindowUI);
        m_mapPages.insert(std::make_pair(MAIN_TAB_SETTING, m_pPageSetting));

        pWindowUI->Attach(m_pPageSetting->GetHWND());
        return pWindowUI;
    }

    return NULL;
}

LRESULT CInteractMainDlg::OnPageSelectChanged(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    int srcPageIndex = HIWORD(wParam);
    int dstPageIndex = LOWORD(wParam);
    if (srcPageIndex < 0 || srcPageIndex > MAIN_TAB_MAX || dstPageIndex < 0 || dstPageIndex > MAIN_TAB_MAX)
    {
        return 0;
    }

    //未创建页面
    if (m_mapPages.find(dstPageIndex) == m_mapPages.end())
    {
        return 0;
    }

    //除首页外，其他页面都要求登录
    if (dstPageIndex != 0 && !InteractGlobalManager::GetInstance().IsLogin())
    {
        CCheckBoxUI* pCheckBoxUI = static_cast<CCheckBoxUI*>(FindControl(_T("opt_home")));
        if (pCheckBoxUI)
        {
            pCheckBoxUI->SetCheck(true);
        }
        return 0;
    }

    m_pTabContainer->SelectItem(dstPageIndex);

    return 0;
}

void CInteractMainDlg::OnClick(TNotifyUI& msg)
{
    Super::OnClick(msg);
}

void CInteractMainDlg::OnSelectChanged(TNotifyUI& msg)
{
    CDuiString sCtrlName = msg.pSender->GetName();

    int dstPageIndex = MAIN_TAB_HOME;
    if (sCtrlName == _T("opt_home"))
    {
        dstPageIndex = MAIN_TAB_HOME;
    }
    else if (sCtrlName == _T("opt_room"))
    {
        dstPageIndex = MAIN_TAB_ROOM;
    }
    else if (sCtrlName == _T("opt_create_room"))
    {
        dstPageIndex = MAIN_TAB_CREATE_ROOM;
    }
    else if (sCtrlName == _T("opt_device"))
    {
        dstPageIndex = MAIN_TAB_DEVICE;
    }
    else if (sCtrlName == _T("opt_setting"))
    {
        dstPageIndex = MAIN_TAB_SETTING;
    }

    PostMessage(WM_PAGE_SELECT_CHANGED, MAKEWPARAM(dstPageIndex, m_pTabContainer->GetCurSel()), 0);
}

void CInteractMainDlg::OnServerApiUserLoginResult(const InteractServerApiRet& ret, const InteractUserModel& interactUserModel)
{
    CLabelUI* pUserIdI = static_cast<CLabelUI*>(FindControl(_T("txt_userid")));
    if (pUserIdI)
    {
        tstring useridInfo = _T("UserId:");
        useridInfo.append(InteractGlobalManager::GetInstance().GetInteractUserId());
        pUserIdI->SetText(useridInfo.c_str());
    }
}
