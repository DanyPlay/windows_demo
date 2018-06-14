#pragma once
#include "../../UI/UI.h"
#include "../ModelDef.h"

typedef struct InteractDeviceInfo
{
    std::string strDeviceId;     //设备ID
    std::string strDeviceName;   //设备名称
} InteractDeviceInfo;

struct InteractDebugInfo
{
    tstring key;
    tstring value;
    InteractDebugInfo(const tstring& key, const tstring& value) {
        this->key = key;
        this->value = value;
    }
};

class CInteractInfoPanel : public CDuiChildWindowImpl < CInteractInfoPanel >, IListCallbackUI
{
public:
    CInteractInfoPanel();
    ~CInteractInfoPanel();

public:
    DUI_DECLARE_WND_CLASS(_T("InteractInfoPanel"));
    DUI_DECLARE_SKIN_FILE(_T("InteractInfoPanel.xml"));
    DUI_DECLARE_MESSAGE_MAP();
    
    DUI_WIN_BEGIN_MSG_MAP(CInteractInfoPanel, CDuiChildWindowImpl<CInteractInfoPanel>)
        DUI_WIN_MESSAGE_HANDLER(WM_CREATE, OnCreate)
        DUI_WIN_MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
    DUI_WIN_END_MSG_MAP()

public:
    void SetAnchorList(const std::vector<InteractUserModel>& vectAnchor);
    void SetInteractRoomModel(const InteractRoomModel& interactRoomModel);
    void SetInteractStats(const InteractStats& mInteractStats);
    void EnableJoin(bool enable);
    void UpdateDeviceInfo();

private:
    LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

private:
    virtual void OnClick(TNotifyUI& msg);
    virtual void OnItemLink(TNotifyUI& msg);
    virtual void OnSelectChanged(TNotifyUI& msg);
    virtual void OnItemSelect(TNotifyUI& msg);
    virtual void OnValueChange(TNotifyUI& msg);
private:
    virtual LPCTSTR GetItemText(CControlUI* pList, int iItem, int iSubItem);
private:
    void InitDevicInfo();
    void FillComboList(const std::string& comboName, std::vector<InteractDeviceInfo> vectDeviceInfo, const std::string& deviceId);
    bool IsAudioMode();
    tstring GetQualityName(QHVC::INTERACT::QUALITY quality);

    void UpdateVideoControl();
    void UpdateAudioControl();
    void UpdateMicroControl();
private:
    std::vector<InteractDeviceInfo> m_vectVideoDevicInfo;
    std::vector<InteractDeviceInfo> m_vectAudioDevicInfo;
    std::vector<InteractDeviceInfo> m_vectMicroDevicInfo;

    std::vector<InteractUserModel> m_vectAnchor;
    InteractRoomModel mInteractRoomModel;
    bool m_bMuteLocalVideoStream;

    std::vector<InteractDebugInfo> m_vectDebugInfo;
    InteractConstant::USER_IDENTITY mUserIdentity;
    bool m_bShowNetworkInfo;
    DWORD m_dwUpdateLogTickCount;
    bool m_bInitControlStatus;
};