#ifndef _INTERACT_SETTING_PAGE_H_
#define _INTERACT_SETTING_PAGE_H_

#pragma once

#include "../../UI/UI.h"

class CInteractSettingPage : public CDuiChildWindowImpl < CInteractSettingPage >
{
public:
    CInteractSettingPage();
    ~CInteractSettingPage();

public:
    DUI_DECLARE_WND_CLASS(_T("InteractSettingPage"));
    DUI_DECLARE_SKIN_FILE(_T("InteractSettingPage.xml"));
    DUI_DECLARE_MESSAGE_MAP();

    DUI_WIN_BEGIN_MSG_MAP(CInteractSettingPage, CDuiChildWindowImpl<CInteractSettingPage>)
        DUI_WIN_MESSAGE_HANDLER(WM_CREATE, OnCreate)
        DUI_WIN_MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
    DUI_WIN_END_MSG_MAP()

public:
    LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

private:
    void InitView();
    void InitView(CComboUI* pComboUI, int** data, unsigned int length);
    void InitData();

public:
    virtual void OnClick(TNotifyUI& msg);
    virtual void OnItemSelect(TNotifyUI& msg);

private:
    void OnAnchorProfileModeSelect();
    void OnGuestProfileModeSelect();

    void OnResetClick();
    bool OnSaveClick();

private:
    tstring Int2Str(int value);
    int Str2Int(const tstring& value);

    int Find(int* data, unsigned int length, int value, int default_);
    tstring& MakeLower(tstring &s);

private:
    static int m_dataProfileModes[];
    static LPCTSTR m_dataProfileResolutions[];
    static int m_dataProfileFrameRates[];
    static int m_dataProfileBitrates[];

    static int m_dataFrameRates[];

private:
    CComboUI* m_pCmbAnchorProfileMode;
    CEditUI* m_pEdtAnchorResolution;
    CComboUI* m_pCmbAnchorFrameRate;
    CEditUI* m_pEdtAnchorBitrate;

    CComboUI* m_pCmbGuestProfileMode;
    CEditUI* m_pEdtGuestResolution;
    CComboUI* m_pCmbGuestFrameRate;
    CEditUI* m_pEdtGuestBitrate;
};

#endif
