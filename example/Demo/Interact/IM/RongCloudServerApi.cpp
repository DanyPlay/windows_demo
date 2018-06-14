#include "stdafx.h"
#include "RongCloudServerApi.h"
#include "../../CommonLib/md5/md5.h"
#include "../../CommonLib/SHA1/sha1.hpp"
#include <time.h>


RongCloudServerApi::RongCloudServerApi()
{
}


RongCloudServerApi::~RongCloudServerApi()
{
}

void RongCloudServerApi::OnAfterRequestSend(FCHttpRequest& rTask)
{

}

void RongCloudServerApi::OnAfterRequestFinish(FCHttpRequest& rTask)
{
    int taskId = rTask.GetTaskID();

    std::string m_receive;
    m_receive.clear();
    rTask.PopReceived(m_receive);

    int apiType = rTask.GetRequestHeader().m_tag;

    Json::Reader reader;
    Json::Value root;
    if (reader.parse((const char*)m_receive.c_str(), root)) {

        if (!root["token"].empty()) {
            std::string userId = root["userId"].asString();
            std::string token = root["token"].asString();
            if (mpCallback != NULL) {
                mpCallback->OnGetTokenSuccess(userId, token);
            }

        }
        else {
            if (mpCallback != NULL) {
                mpCallback->OnIMServerFailed(-1, m_receive);
            }
        }
    }
}

void RongCloudServerApi::GetToken(const std::string& userId, const std::string& nickName, const std::string& portrait)
{
    std::string url = "http://api.cn.ronghub.com/user/getToken.json";
    std::map<std::string, std::string> params;

    HTTP_REQUEST_HEADER   m_header(HTTP_REQUEST_HEADER::VERB_TYPE_POST);

    std::string body = "userId=" + userId + "&name=" + nickName + "&portraitUri=" + portrait;

 
    m_header.SetPostData(body);

    time_t now;
    time(&now);

    srand((unsigned)time(NULL));
    std::string nonce = Util::String::StrFormat("%d", rand() * 0xffffff);
    std::string time_stamp = Util::String::StrFormat("%d", now);
    std::string signature = SHA1::GetSHA1(m_strAppSecret + nonce + time_stamp);


    m_header.m_header += "App-Key:" + m_strAppKey + "\r\n";
    m_header.m_header += "Nonce:" + nonce + "\r\n";;
    m_header.m_header += "Timestamp:" + time_stamp + "\r\n";;
    m_header.m_header += "Signature:" + signature + "\r\n";;
    m_header.m_header += "Content-Type:" + std::string("application/x-www-form-urlencoded");
    m_header.m_url = url;

    int taskId = AddRequest(m_header);
}

void RongCloudServerApi::SetAppInfo(const std::string& appKey, const std::string& appSecret)
{
    m_strAppKey = appKey;
    m_strAppSecret = appSecret;
}

void RongCloudServerApi::SetApiCallback(RongCloudServerApiCallBack* pCallback)
{
    AutoThreadSection autolock(&mThreadSection);
    mpCallback = pCallback;
   
}
