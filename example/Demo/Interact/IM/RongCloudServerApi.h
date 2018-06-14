#pragma once

#include "../../CommonLib/http/http_request_manager.h"
#include "../../CommonLib/json/json.h"
#include "../ModelDef.h"
#include "../../CommonLib/Thread/ThreadLock.h"
#include <vector>



class RongCloudServerApiCallBack {
public:
    virtual void OnGetTokenSuccess(const std::string& userId, const std::string& token){}

    virtual void OnIMServerFailed(int errCode, const std::string& errMessage){}
};


class RongCloudServerApi : public FCHttpRequestManager
{
public:
    RongCloudServerApi();
    ~RongCloudServerApi();

public:
    void GetToken(const std::string& userId, const std::string& nickName,   const std::string& portrait);

    void SetAppInfo(const std::string& appKey, const std::string& appSecret);
  
    void SetApiCallback(RongCloudServerApiCallBack* pCallback);
private:
    virtual void OnAfterRequestSend(FCHttpRequest& rTask);
    virtual void OnAfterRequestFinish(FCHttpRequest& rTask);

private:
    RongCloudServerApiCallBack* mpCallback;

    ThreadSection mThreadSection;

    std::string m_strAppKey;
    std::string m_strAppSecret;
};

