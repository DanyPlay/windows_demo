#pragma once
#include "InteractUIConstant.h"
#include "../../UI/UI.h"
#include "../ModelDef.h"

class CInteractTestDlg : public CDuiDialogImpl < CInteractTestDlg >
{
public:
    CInteractTestDlg();
    ~CInteractTestDlg();

public:
    DUI_DECLARE_WND_CLASS(_T("TestWnd"));
    DUI_DECLARE_SKIN_FILE(_T("TestWnd.xml"));
    DUI_DECLARE_MESSAGE_MAP();

    DUI_WIN_BEGIN_MSG_MAP(CInteractTestDlg, CDuiDialogImpl<CInteractTestDlg>)
        DUI_WIN_MESSAGE_HANDLER(WM_CREATE, OnCreate)
        DUI_WIN_MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
        DUI_WIN_END_MSG_MAP()

public:
    void SetInteractRoomModel(const InteractRoomModel& interactRoomModel);
    void SetAnchorIDList(const std::vector<std::string >& vectAnchorID);

private:
    LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

private:
    virtual void OnClick(TNotifyUI& msg);
    void SetTextInfo(const tstring& info, bool bValue);
    void SetTextInfo(const tstring& info);
private:
    bool m_bAllVideoMute;
    bool m_bAllAudioMute;
    bool m_bDualStreamMode;
    bool m_bAudioVolumeIndication;
    bool m_bRomoteStreamLow;
    bool m_bEnableAudio;
    bool m_bEnableVideo;

    InteractRoomModel mInteractRoomModel;
    std::vector<std::string> m_vectAnchorID;
};