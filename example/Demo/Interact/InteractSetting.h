#ifndef _INTERACT_SETTING_H_
#define _INTERACT_SETTING_H_

#pragma once

#include "ModelDef.h"

class InteractSetting
{
protected:
    InteractSetting();
    ~InteractSetting();

public:
    static InteractSetting& GetInstance();

private:
    int Read(LPCTSTR lpAppName, LPCTSTR lpKeyName, int nDefault);
    bool Read(LPCTSTR lpAppName, LPCTSTR lpKeyName, bool bDefault);
    tstring Read(LPCTSTR lpAppName, LPCTSTR lpKeyName, LPCTSTR lpDefault);

    void Write(LPCTSTR lpAppName, LPCTSTR lpKeyName, int nValue);
    void Write(LPCTSTR lpAppName, LPCTSTR lpKeyName, bool bValue);
    void Write(LPCTSTR lpAppName, LPCTSTR lpKeyName, LPCTSTR lpValue);

private:
    tstring GetUserIdentityTag(InteractConstant::USER_IDENTITY enIdentity);

public:
    int GetProfileMode(InteractConstant::USER_IDENTITY enIdentity, int nDefault = QHVC::INTERACT::VIDEO_PROFILE::VIDEO_PROFILE_360P_9);
    void SetProfileMode(InteractConstant::USER_IDENTITY enIdentity, int nProfileMode);

    int GetResolutionWidth(InteractConstant::USER_IDENTITY enIdentity, int nDefaultWidth = 640);
    void SetResolutionWidth(InteractConstant::USER_IDENTITY enIdentity, int nResolutionWidth);

    int GetResolutionHeight(InteractConstant::USER_IDENTITY enIdentity, int nDefaultWidth = 640);
    void SetResolutionHeight(InteractConstant::USER_IDENTITY enIdentity, int nResolutionWidth);

    int GetFrameRate(InteractConstant::USER_IDENTITY enIdentity, int nDefault = 15);
    void SetFrameRate(InteractConstant::USER_IDENTITY enIdentity, int nFrameRate);

    int GetBitrate(InteractConstant::USER_IDENTITY enIdentity, int nDefault = 800);
    void SetBitrate(InteractConstant::USER_IDENTITY enIdentity, int nBitrate);

public:
    tstring GetBid();
    void SetBid(const tstring& strBid);

    tstring GetCid();
    void SetCid(const tstring& strCid);

    tstring GetAppKey();
    void SetAppKey(const tstring& strAppKey);

    tstring GetAppSecret();
    void SetAppSecret(const tstring& strAppSecret);

private:
    tstring m_strFilePath;
};

#endif
