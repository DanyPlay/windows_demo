#include "stdafx.h"
#include "InteractDevicePage.h"

DUI_BEGIN_MESSAGE_MAP(CInteractDevicePage, CDuiDialogImpl<CInteractDevicePage>)
DUI_ON_MSGTYPE(DUI_MSGTYPE_CLICK, OnClick)
DUI_END_MESSAGE_MAP()

CInteractDevicePage::CInteractDevicePage()
{

}

CInteractDevicePage::~CInteractDevicePage()
{

}

LRESULT CInteractDevicePage::OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    return 0;
}

LRESULT CInteractDevicePage::OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    return 0;
}

void CInteractDevicePage::OnClick(TNotifyUI& msg)
{
    Super::OnClick(msg);
}
