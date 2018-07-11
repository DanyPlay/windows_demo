#ifndef _INTERACT_AUDIO_MIXING_H_
#define _INTERACT_AUDIO_MIXING_H_

#pragma once

#include "../../UI/UI.h"

class CInteractAudioMixingDlg : public CDuiDialogImpl < CInteractAudioMixingDlg >
{
public:
    typedef struct Config
    {
        std::string filePath;
        bool loopback;
        bool replace;
        int cycle;

        Config()
        {
            loopback = false;
            replace = false;
            cycle = 1;
        }
    } Config;

public:
    CInteractAudioMixingDlg();
    virtual ~CInteractAudioMixingDlg();

public:
    DUI_DECLARE_SKIN_FILE(_T("InteractAudioMixingDlg.xml"));

    DUI_WIN_BEGIN_MSG_MAP(CInteractAudioMixingDlg, CDuiDialogImpl<CInteractAudioMixingDlg>)
        DUI_WIN_MESSAGE_HANDLER(WM_CREATE, OnCreate)
    DUI_WIN_END_MSG_MAP()

    LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

public:
    virtual void OnClick(TNotifyUI& msg);

private:
    void DoBrowserFile();
    void DoOk();

public:
    const Config* GetConfig() { return &m_oConfig; }

private:
    Config m_oConfig;

    CEditUI* m_pEdtFilePath;
    CEditUI* m_pEdtCycle;
    COptionUI* m_pOptMix;
    COptionUI* m_pOptLookback;
};

#endif