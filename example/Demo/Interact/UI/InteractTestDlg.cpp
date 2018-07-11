#include "stdafx.h"
#include "InteractTestDlg.h"
#include "../InteractGlobalManager.h"
#include "../DebugSetting.h"
#include "Control/VideoWnd.h"
#include <io.h>

#include "../../opencv/include/opencv2/highgui/highgui.hpp"
#include "../../opencv/include/opencv2/imgproc/imgproc.hpp"

#ifdef DEBUG
#pragma comment(lib, "opencv/lib/opencv_core2413d.lib")
#pragma comment(lib, "opencv/lib/opencv_highgui2413d.lib")
#pragma comment(lib, "opencv/lib/opencv_imgproc2413d.lib")
#else
#pragma comment(lib, "opencv/lib/opencv_core2413.lib")
#pragma comment(lib, "opencv/lib/opencv_highgui2413.lib")
#pragma comment(lib, "opencv/lib/opencv_imgproc2413.lib")
#endif

#define SOUNDID1 100
#define SOUNDID2 101


DUI_BEGIN_MESSAGE_MAP(CInteractTestDlg, CDuiDialogImpl<CInteractTestDlg>)
DUI_ON_MSGTYPE(DUI_MSGTYPE_CLICK, OnClick)
DUI_END_MESSAGE_MAP()

class QHVCInteractLocalVideoRenderCallback : public QHVC::INTERACT::IQHVCInteractLocalVideoRenderCallback {
private:
    CInteractTestDlg* m_pDialog;
public:
    QHVCInteractLocalVideoRenderCallback(CInteractTestDlg* pDialog) {
        m_pDialog = pDialog;
    }

    virtual bool OnCaptureVideoFrame(QHVC::INTERACT::VideoFrame& videoFrame)
    {
        cv::Mat yuvImg;
        yuvImg.create(videoFrame.height * 3 / 2, videoFrame.width, CV_8UC1);
        memcpy(yuvImg.data, videoFrame.yBuffer, videoFrame.yStride * videoFrame.height);
        memcpy(yuvImg.data + videoFrame.yStride * videoFrame.height, videoFrame.uBuffer, videoFrame.uStride * videoFrame.height / 2);
        memcpy(yuvImg.data + videoFrame.yStride * videoFrame.height + videoFrame.uStride * videoFrame.height / 2, videoFrame.vBuffer, videoFrame.vStride * videoFrame.height / 2);

        //在MyWindow的窗中中显示存储在img中的图片

        cv::Mat rgbImg;
        cv::cvtColor(yuvImg, rgbImg, CV_YUV2BGR_I420);


        cv::imshow("MyWindow", rgbImg);

        return true;
    }
};

class QHVCInteractRemoteVideoRenderCallback : public QHVC::INTERACT::IQHVCInteractRemoteVideoRenderCallback {
private:
    CInteractTestDlg* m_pDialog;
public:
    QHVCInteractRemoteVideoRenderCallback(CInteractTestDlg* pDialog) {
        m_pDialog = pDialog;
    }

    virtual bool OnRenderVideoFrame(const char* userId, QHVC::INTERACT::VideoFrame& videoFrame)
    {
        cv::Mat yuvImg;
        yuvImg.create(videoFrame.height * 3 / 2, videoFrame.yStride, CV_8UC1);
        //for (int i = 0; i < videoFrame.height - 100; i++) {
        //    void* sourceY = (unsigned char*)videoFrame.yBuffer + videoFrame.yStride * i;
        //    void* sourceU = (unsigned char*)videoFrame.uBuffer + videoFrame.uStride * i;
        //    void* sourceV = (unsigned char*)videoFrame.uBuffer + videoFrame.vStride * i;

        //    void* destY = (unsigned char*)yuvImg.data + videoFrame.width * i;
        //    void* destU = (unsigned char*)yuvImg.data + videoFrame.width * videoFrame.height + videoFrame.width / 2 * i;
        //    void* destV = (unsigned char*)yuvImg.data + videoFrame.width * videoFrame.height * 5 / 4 + videoFrame.width / 2 * i;

        //    memcpy(destY, sourceY, videoFrame.width);
        //    memcpy(destU, sourceU, videoFrame.width / 2);
        //    memcpy(destV, sourceV, videoFrame.width / 2);

        //}

        memcpy(yuvImg.data, videoFrame.yBuffer, videoFrame.yStride * videoFrame.height);
        memcpy(yuvImg.data + videoFrame.yStride * videoFrame.height, videoFrame.uBuffer, videoFrame.uStride * videoFrame.height / 2);
        memcpy(yuvImg.data + videoFrame.yStride * videoFrame.height + videoFrame.uStride * videoFrame.height / 2, videoFrame.vBuffer, videoFrame.vStride * videoFrame.height / 2);

        //在MyWindow的窗中中显示存储在img中的图片

        cv::Mat rgbImg;
        cv::cvtColor(yuvImg, rgbImg, CV_YUV2BGR_I420);


        cv::imshow("MyWindow", rgbImg);

        return true;
    }
};

class QHVCInteractAudioFrameCallback : public QHVC::INTERACT::IQHVCInteractAudioFrameCallback {
private:
    CInteractTestDlg* m_pDialog;
public:
    QHVCInteractAudioFrameCallback(CInteractTestDlg* pDialog) {
        m_pDialog = pDialog;
    }

    virtual bool OnRecordAudioFrame(QHVC::INTERACT::AudioFrame& audioFrame)
    {
        return true;
    }

    virtual bool OnPlaybackAudioFrame(QHVC::INTERACT::AudioFrame& audioFrame)
    {
        return true;
    }
};

CInteractTestDlg::CInteractTestDlg()
{
    m_bAllVideoMute = false;
    m_bAllAudioMute = false;
    m_bAudioVolumeIndication = false;
    m_bDualStreamMode = false;
    m_bRomoteStreamLow = false;
    m_bEnableAudio = false;
    m_bEnableVideo = true;

    m_pUserRenderWnd = NULL;
    m_pQHVCInteractLocalVideoRenderCallback = NULL;
    m_pQHVCInteractRemoteVideoRenderCallback = NULL;
    m_pQHVCInteractAudioFrameCallback = NULL;

    m_bEffect1Loaded = false;
    m_bEffect2Loaded = false;

    m_nEffectVolume = 100;
    m_nEffect1Volume = 100;
}

CInteractTestDlg::~CInteractTestDlg()
{
    if (m_pUserRenderWnd) {
        delete m_pUserRenderWnd;
        m_pUserRenderWnd = NULL;
    }

    if (m_pQHVCInteractLocalVideoRenderCallback) {
        delete m_pQHVCInteractLocalVideoRenderCallback;
        m_pQHVCInteractLocalVideoRenderCallback = NULL;
    }

    if (m_pQHVCInteractRemoteVideoRenderCallback) {
        delete m_pQHVCInteractRemoteVideoRenderCallback;
        m_pQHVCInteractRemoteVideoRenderCallback = NULL;
    }

    if (m_pQHVCInteractAudioFrameCallback) {
        delete m_pQHVCInteractAudioFrameCallback;
        m_pQHVCInteractAudioFrameCallback = NULL;
    }
}

LRESULT CInteractTestDlg::OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    m_pQHVCInteractLocalVideoRenderCallback = new QHVCInteractLocalVideoRenderCallback(this);
    m_pQHVCInteractRemoteVideoRenderCallback = new QHVCInteractRemoteVideoRenderCallback(this);
    m_pQHVCInteractAudioFrameCallback = new QHVCInteractAudioFrameCallback(this);

    //QHVC::INTERACT::QHVCInteract::SetLocalVideoRenderCallback(m_pQHVCInteractLocalVideoRenderCallback);
    QHVC::INTERACT::QHVCInteract::SetRemoteVideoRenderCallback(m_pQHVCInteractRemoteVideoRenderCallback);
    QHVC::INTERACT::QHVCInteract::SetAudioFrameCallback(m_pQHVCInteractAudioFrameCallback);

    char path[512] = { 0 };
    GetModuleFileNameA(NULL, path, 511);
    std::string module_path = path;
    m_strEffectPath1 = module_path.substr(0, module_path.rfind("\\") + 1);
    m_strEffectPath1.append("effect1.wav");

    m_strEffectPath2 = module_path.substr(0, module_path.rfind("\\") + 1);
    m_strEffectPath2.append("effect2.wav");

    return 0;
}

LRESULT CInteractTestDlg::OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    cv::destroyWindow("MyWindow");

    //QHVC::INTERACT::QHVCInteract::SetLocalVideoRenderCallback(NULL);
    QHVC::INTERACT::QHVCInteract::SetRemoteVideoRenderCallback(NULL);
    QHVC::INTERACT::QHVCInteract::SetAudioFrameCallback(NULL);

    return 0;
}

void CInteractTestDlg::OnClick(TNotifyUI& msg)
{
    CDuiString sCtrlName = msg.pSender->GetName();

    if (sCtrlName == _T("SetCloudControlRole"))
    {
        tstring role = DebugSetting::GetInstance().GetCloudRole();
        if (!role.empty()) {
            QHVC::INTERACT::QHVCInteract::SetCloudControlRole(STR2A(role).c_str());
        }
        SetTextInfo(_T("SetCloudControlRole:") + role);
    }
    else if (sCtrlName == _T("MuteAllRemoteVideoStreams"))
    {
        m_bAllVideoMute = !m_bAllVideoMute;
        SetTextInfo(_T("MuteAllRemoteVideoStreams"), m_bAllVideoMute);
        QHVC::INTERACT::QHVCInteract::MuteAllRemoteVideoStreams(m_bAllVideoMute);
    }
    else if (sCtrlName == _T("MuteAllRemoteAudioStreams"))
    {
        m_bAllAudioMute = !m_bAllAudioMute;
        SetTextInfo(_T("MuteAllRemoteAudioStreams"), m_bAllAudioMute);

        QHVC::INTERACT::QHVCInteract::MuteAllRemoteVideoStreams(m_bAllAudioMute);
    }
    else if (sCtrlName == _T("EnableAudioVolumeIndication"))
    {
        m_bAudioVolumeIndication = !m_bAudioVolumeIndication;
        int interval = m_bAudioVolumeIndication ? 1000 : 0;

        SetTextInfo(_T("EnableAudioVolumeIndication"), m_bAudioVolumeIndication);

        QHVC::INTERACT::QHVCInteract::EnableAudioVolumeIndication(interval, 10);
    }
    else if (sCtrlName == _T("EnableDualStreamMode"))
    {
        m_bDualStreamMode = !m_bDualStreamMode;

        SetTextInfo(_T("EnableDualStreamMode"), m_bDualStreamMode);

        QHVC::INTERACT::QHVCInteract::EnableDualStreamMode(m_bDualStreamMode);
    }
    else if (sCtrlName == _T("SetLowStreamVideoProfile"))
    {
        SetTextInfo(_T("SetLowStreamVideoProfile:"));
        QHVC::INTERACT::QHVCInteract::SetLowStreamVideoProfile(320, 180, 15, 200);
    }
    else if (sCtrlName == _T("SetRemoteVideoStream"))
    {
        m_bRomoteStreamLow = !m_bRomoteStreamLow;
        QHVC::INTERACT::VIDEO_STREAM_LEVEL level = QHVC::INTERACT::VIDEO_STREAM_LEVEL_HIGH;
        if (m_bRomoteStreamLow) {
            level = QHVC::INTERACT::VIDEO_STREAM_LEVEL_LOW;
        }
        else {
            level = QHVC::INTERACT::VIDEO_STREAM_LEVEL_HIGH;
        }

        SetTextInfo(_T("SetRemoteVideoStream"), m_bRomoteStreamLow);

        std::string userId = STR2A(InteractGlobalManager::GetInstance().GetInteractUserId());
        for (int i = 0; i < m_vectAnchorID.size(); i++) {
            if (strcmp(userId.c_str(), m_vectAnchorID[i].c_str()) != 0) {
                QHVC::INTERACT::QHVCInteract::SetRemoteVideoStream(m_vectAnchorID[i].c_str(), level);
            }
        }
    }
    else if (sCtrlName == _T("EnableAudio"))
    {
        m_bEnableAudio = true;
        SetTextInfo(_T("EnableAudio"), m_bEnableAudio);
        QHVC::INTERACT::QHVCInteract::EnableAudio();
    }
    else if (sCtrlName == _T("DisableAudio"))
    {
        m_bEnableAudio = false;
        SetTextInfo(_T("DisableAudio"), m_bEnableAudio);
        QHVC::INTERACT::QHVCInteract::DisableAudio();
    }
    else if (sCtrlName == _T("EnableVideo"))
    {
        m_bEnableVideo = true;
        SetTextInfo(_T("EnableVideo"), m_bEnableVideo);
        QHVC::INTERACT::QHVCInteract::EnableVideo();
    }
    else if (sCtrlName == _T("DisableVideo"))
    {
        m_bEnableVideo = false;
        SetTextInfo(_T("DisableVideo"), m_bEnableVideo);
        QHVC::INTERACT::QHVCInteract::DisableVideo();
    }
    else if (sCtrlName == _T("ClearVideoCompositingLayout"))
    {
        SetTextInfo(_T("ClearVideoCompositingLayout"));
        QHVC::INTERACT::QHVCInteract::ClearVideoCompositingLayout();
    }
    else if (sCtrlName == _T("GetEffectsVolume"))
    {
        int ret = QHVC::INTERACT::QHVCInteract::GetEffectsVolume();
        std::string  info;
        info = Util::String::StrFormat("GetEffectsVolume, ret=%d", ret);
        SetTextInfo(STR2T(info));
    }

    else if (sCtrlName == _T("SetEffectsVolume"))
    {
        if (m_nEffectVolume == 100) {
            m_nEffectVolume = 10;
        }
        else {
            m_nEffectVolume = 100;
        }
        int ret = QHVC::INTERACT::QHVCInteract::SetEffectsVolume(m_nEffectVolume);
        std::string  info;
        info = Util::String::StrFormat("SetEffectsVolume, volume=%d, ret=%d", m_nEffectVolume, ret);
        SetTextInfo(STR2T(info));
    }
    else if (sCtrlName == _T("SetVolumeOfEffect"))
    {
        if (m_nEffect1Volume == 100) {
            m_nEffect1Volume = 10;
        }
        else {
            m_nEffect1Volume = 100;
        }

        int ret = QHVC::INTERACT::QHVCInteract::SetVolumeOfEffect(SOUNDID1, m_nEffect1Volume);
        std::string  info;
        info = Util::String::StrFormat("SetVolumeOfEffect, volume=%d, ret=%d", m_nEffect1Volume, ret);
        SetTextInfo(STR2T(info));
    }
    else if (sCtrlName == _T("PlayEffect"))
    {
        if (!m_bEffect1Loaded) {
            if (_access(m_strEffectPath1.c_str(), 0) != 0) {
                std::string msg = "文件不存在：" + m_strEffectPath1;
                DuiMessageBox(m_hWnd, STR2T(msg).c_str(), INTERACT_TITLE, MB_YESNO);
                return;
            }

            int ret = QHVC::INTERACT::QHVCInteract::PlayEffect(SOUNDID1, m_strEffectPath1.c_str(), 100, 1.0, 0, 100.0, true);
            std::string  info;
            info = Util::String::StrFormat("PlayEffect, path = %s, ret=%d", m_strEffectPath1.c_str(), ret);
            SetTextInfo(STR2T(info));
            m_bEffect1Loaded = true;
            return;
        }

        if (!m_bEffect2Loaded) {
            if (_access(m_strEffectPath2.c_str(), 0) != 0) {
                std::string msg = "文件不存在：" + m_strEffectPath2;
                DuiMessageBox(m_hWnd, STR2T(msg).c_str(), INTERACT_TITLE, MB_YESNO);
                return;
            }

            int ret = QHVC::INTERACT::QHVCInteract::PlayEffect(SOUNDID2, m_strEffectPath2.c_str(), 100, 1.0, 0, 100.0, true);
            std::string  info;
            info = Util::String::StrFormat("PlayEffect, path = %s, ret=%d", m_strEffectPath2.c_str(),  ret);
            SetTextInfo(STR2T(info));
            m_bEffect2Loaded = true;
        }
    }
    else if (sCtrlName == _T("StopEffect"))
    {
        int ret = QHVC::INTERACT::QHVCInteract::StopEffect(SOUNDID1);
        std::string  info;
        info = Util::String::StrFormat("StopEffect, ret=%d", ret);
        SetTextInfo(STR2T(info));
        m_bEffect1Loaded = false;
    }
    else if (sCtrlName == _T("StopAllEffects"))
    {
        int ret = QHVC::INTERACT::QHVCInteract::StopAllEffects();
        std::string  info;
        info = Util::String::StrFormat("StopAllEffects, ret=%d", ret);
        SetTextInfo(STR2T(info));
        m_bEffect1Loaded = false;
        m_bEffect2Loaded = false;
    }
    else if (sCtrlName == _T("PreloadEffect"))
    {
        if (_access(m_strEffectPath1.c_str(), 0) != 0) {
            std::string msg = "文件不存在：" + m_strEffectPath1;
            DuiMessageBox(m_hWnd, STR2T(msg).c_str(), INTERACT_TITLE, MB_YESNO);
            return;
        }

        int ret = QHVC::INTERACT::QHVCInteract::PreloadEffect(SOUNDID1, m_strEffectPath1.c_str());
        std::string  info;
        info = Util::String::StrFormat("PreloadEffect, ret=%d", ret);
        SetTextInfo(STR2T(info));
    }
    else if (sCtrlName == _T("UnloadEffect"))
    {
        int ret = QHVC::INTERACT::QHVCInteract::UnloadEffect(SOUNDID1);
        std::string  info;
        info = Util::String::StrFormat("UnloadEffect, ret=%d", ret);
        SetTextInfo(STR2T(info));
        m_bEffect1Loaded = FALSE;
    }
    else if (sCtrlName == _T("PauseEffect"))
    {
        int ret = QHVC::INTERACT::QHVCInteract::PauseEffect(SOUNDID1);
        std::string  info;
        info = Util::String::StrFormat("PauseEffect, ret=%d", ret);
        SetTextInfo(STR2T(info));
    }
    else if (sCtrlName == _T("PauseAllEffects"))
    {
        int ret = QHVC::INTERACT::QHVCInteract::PauseAllEffects();
        std::string  info;
        info = Util::String::StrFormat("PauseAllEffects, ret=%d", ret);
        SetTextInfo(STR2T(info));
    }
    else if (sCtrlName == _T("ResumeEffect"))
    {
        int ret = QHVC::INTERACT::QHVCInteract::ResumeEffect(SOUNDID1);
        std::string  info;
        info = Util::String::StrFormat("ResumeEffect, ret=%d", ret);
        SetTextInfo(STR2T(info));
    }
    else if (sCtrlName == _T("ResumeAllEffects"))
    {
        int ret = QHVC::INTERACT::QHVCInteract::ResumeAllEffects();
        std::string  info;
        info = Util::String::StrFormat("ResumeAllEffects, ret=%d", ret);
        SetTextInfo(STR2T(info));
    }
    Super::OnClick(msg);
}

void CInteractTestDlg::SetAnchorIDList(const std::vector<std::string >& vectAnchorID)
{
    m_vectAnchorID = vectAnchorID;
}

void CInteractTestDlg::SetInteractRoomModel(const InteractRoomModel& interactRoomModel)
{
    mInteractRoomModel = interactRoomModel;
}

void CInteractTestDlg::SetTextInfo(const tstring& info, bool bValue)
{
    tstring log = info;
    if (bValue) {
        log.append(_T(":true"));
    }
    else {
        log.append(_T(":false"));
    }

    CLabelUI* pLabelInfo = static_cast<CLabelUI*>(FindControl(_T("lb_text_info")));
    if (pLabelInfo) {
        pLabelInfo->SetText(log.c_str());
    }
}

void CInteractTestDlg::SetTextInfo(const tstring& info)
{
    CLabelUI* pLabelInfo = static_cast<CLabelUI*>(FindControl(_T("lb_text_info")));
    if (pLabelInfo) {
        pLabelInfo->SetText(info.c_str());
    }
}

CControlUI* CInteractTestDlg::CreateControl(LPCTSTR pstrClass)
{
    if (_tcsicmp(pstrClass, DUI_CTR_WINDOW) == 0)
    {
        CWindowUI* pWindowUI = new CWindowUI();

        m_pUserRenderWnd = new CVideoWnd();
        m_pUserRenderWnd->CreateEx(m_hWnd, pWindowUI);
        m_pUserRenderWnd->SetNotifyWnd(m_hWnd);


        //创建一个名字为MyWindow的窗口

        cv::namedWindow("MyWindow", CV_WINDOW_NORMAL);
        cv::resizeWindow("MyWindow", 250, 250);


        HWND hWnd = (HWND)cvGetWindowHandle("MyWindow");
        HWND hParent = ::GetParent(hWnd);

        ::SetParent(hWnd, m_pUserRenderWnd->GetHWND());
        ::ShowWindow(hParent, SW_HIDE);

        return pWindowUI;
    }


    return NULL;
}
