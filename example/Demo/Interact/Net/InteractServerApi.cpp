#include "stdafx.h"
#include "InteractServerApi.h"
#include "../../CommonLib/md5/md5.h"

#include "../InteractGlobalManager.h"
#include <io.h>

static std::string  HOST = "http://livedemo.vcloud.360.cn/api";

static std::string  HOST_DEBUG = "http://k0106v.add.lfyc.qihoo.net:9908/api";
static std::string  HOST_RELEASE = "http://livedemo.vcloud.360.cn/api";


bool SortUserModel(const InteractUserModel &v1, const InteractUserModel &v2)//注意：本函数的参数的类型一定要与vector中元素的类型一致
{
    return _tcsicmp(v1.createTime.c_str(), v2.createTime.c_str()) < 0;
}

InteractServerApi::InteractServerApi()
{
    char path[512] = { 0 };
    GetModuleFileNameA(NULL, path, 511);
    std::string module_path = path;
    std::string debug_path = module_path.substr(0, module_path.rfind("\\") + 1);
    debug_path.append("debug.ini");

    if (_access(debug_path.c_str(), 0) == 0) {
        HOST = HOST_DEBUG;
    }
}


InteractServerApi::~InteractServerApi()
{
}

void InteractServerApi::OnAfterRequestSend(FCHttpRequest& rTask)
{

}

void InteractServerApi::OnAfterRequestFinish(FCHttpRequest& rTask)
{
    int taskId = rTask.GetTaskID();

    std::string m_receive;
    m_receive.clear();
    rTask.PopReceived(m_receive);

    int apiType = rTask.GetRequestHeader().m_tag;
    InteractServerApiRet callbackRet;
    Json::Value jsonData;

    switch (apiType)
    {
        case Interact_ApiType_UserLogin: {
            InteractUserModel userModel;
            ParseCallbackRet(m_receive, callbackRet, jsonData);
            if (callbackRet.errCode == 0) {
                if (!jsonData["uSign"].empty()) {
                    std::string uSignAll = jsonData["uSign"].asCString();
                    std::string appKey = uSignAll.substr(0, uSignAll.find(":"));
                    std::string uSign = uSignAll.substr(uSignAll.find(":") + 1, uSignAll.size());

                    InteractGlobalManager::GetInstance().SetAppKey(STR2T(appKey));
                    InteractGlobalManager::GetInstance().SetUsign(STR2T(uSign));
                }

                if (!jsonData["imContext"].empty()) {
                    InteractIMModel imModel;
                    bool ret = ParseInteractIMModel(jsonData["imContext"], imModel);
                    if (ret) {
                        InteractGlobalManager::GetInstance().SetInteractImModel(imModel);
                    }
                }


                bool ret = ParseInteractUserModel(jsonData, userModel);
                if (!ret) {
                    callbackRet.errCode = -1;
                    callbackRet.errMessage = _T("parse error");
                }
            }

            OnServerApiUserLoginResult(callbackRet, userModel);


            break;
        }
        case Interact_ApiType_GetRoomList: {
            std::vector<InteractRoomModel> interactRoomModelList;
            ParseCallbackRet(m_receive, callbackRet, jsonData);
            if (callbackRet.errCode == 0) {
                if (!jsonData.empty() && jsonData.isArray()){
                    for (int i = 0; i < (int)jsonData.size(); i++) {
                        InteractRoomModel interactRoomModel;
                        ParseInteractRoomModel(jsonData[i], interactRoomModel);
                        interactRoomModelList.push_back(interactRoomModel);
                    }
                }
            }

            OnServerApiGetRoomListResult(callbackRet, interactRoomModelList);
            break;
        }
        case Interact_ApiType_GetRoomInfo: {
            InteractRoomModel interactRoomModel;
            ParseCallbackRet(m_receive, callbackRet, jsonData);
            if (callbackRet.errCode == 0) {
                bool ret = ParseInteractRoomModel(jsonData, interactRoomModel);
                if (!ret) {
                    callbackRet.errCode = -1;
                    callbackRet.errMessage = _T("parse error");
                }
            }

            OnServerApiGetRoomInfoResult(callbackRet, interactRoomModel);


            break;
        }
        case Interact_ApiType_CreateRoom: {
            InteractRoomModel interactRoomModel;
            ParseCallbackRet(m_receive, callbackRet, jsonData);
            if (callbackRet.errCode == 0) {
                bool ret = ParseInteractRoomModel(jsonData, interactRoomModel);
                if (!ret) {
                    callbackRet.errCode = -1;
                    callbackRet.errMessage = _T("parse error");
                }
            }

            OnServerApiCreateRoomResult(callbackRet, interactRoomModel);

            break;
        }
        case Interact_ApiType_JoinRoom: {
            InteractRoomModel interactRoomModel;
            ParseCallbackRet(m_receive, callbackRet, jsonData);
            if (callbackRet.errCode == 0) {
                bool ret = ParseInteractRoomModel(jsonData, interactRoomModel);
                if (!ret) {
                    callbackRet.errCode = -1;
                    callbackRet.errMessage = _T("parse error");
                }
            }

            OnServerApiJoinRoomResult(callbackRet, interactRoomModel);
            break;
        }
        case Interact_ApiType_UserLeaveRoom: {
            InteractRoomModel interactRoomModel;
            ParseCallbackRet(m_receive, callbackRet, jsonData);
            if (callbackRet.errCode == 0) {
                bool ret = ParseInteractRoomModel(jsonData, interactRoomModel);
                if (!ret) {
                    callbackRet.errCode = -1;
                    callbackRet.errMessage = _T("parse error");
                }
            }
            OnServerApiUserLeaveRoomResult(callbackRet, interactRoomModel);
            break;
        }
        case Interact_ApiType_GetRoomUserList: {
            std::vector<InteractUserModel>  userModelList;
            ParseCallbackRet(m_receive, callbackRet, jsonData);
            if (callbackRet.errCode == 0) {
                if (!jsonData.empty() && jsonData.isArray()){
                    for (int i = 0; i < jsonData.size(); i++) {
                        InteractUserModel userModel;
                        bool ret = ParseInteractUserModel(jsonData[i], userModel);
                        if (ret) {
                            userModelList.push_back(userModel);
                        }
                    }
                }
            }
            std::sort(userModelList.begin(), userModelList.end(), SortUserModel);

            OnServerApiGetRoomUserListResult(callbackRet, userModelList);
            break;
        }
        case Interact_ApiType_ChangeUserdentity: {
            InteractRoomModel interactRoomModel;
            ParseCallbackRet(m_receive, callbackRet, jsonData);
            if (callbackRet.errCode == 0) {
                bool ret = ParseInteractRoomModel(jsonData, interactRoomModel);
                if (!ret) {
                    callbackRet.errCode = -1;
                    callbackRet.errMessage = _T("parse error");
                }
            }

            OnServerApiChangeUserdentityResult(callbackRet, interactRoomModel);

            break;
        }
        case Interact_ApiType_DismissRoom: {
            ParseCallbackRet(m_receive, callbackRet, jsonData);
            OnServerApiDismissRoomResult(callbackRet);
            break;
        }
        case Interact_ApiType_UserHeart: {
            ParseCallbackRet(m_receive, callbackRet, jsonData);
            OnServerApiUserHeartResult(callbackRet);
            break;
        }
        default:

            break;
    };
}

std::map<std::string, std::string > InteractServerApi::GetServerCommonParams() {

    std::map<std::string, std::string> params;;
    params["channelId"] = STR_T2U(InteractGlobalManager::GetInstance().GetChannelId());
    params["deviceId"] = STR_T2U(InteractGlobalManager::GetInstance().GetDeviceId());
    params["ts"] = Util::String::StrFormat("%d", GetTickCount());
    params["sessionId"] = STR_T2U(InteractGlobalManager::GetInstance().CreateSessionId());
    params["ostype"] = Util::String::StrFormat("win%d", Util::OS::GetOSVersion());
    params["modelName"] = "interact";;
    params["appVersion"] = STR_T2U(Util::File::GetFileVersion(_Module.GetModuleInstance()));

    return params;
}


void InteractServerApi::CommonHttpPost(const std::string& url, int apiType, std::map<std::string, std::string> postParams) {

    HTTP_REQUEST_HEADER   m_header(HTTP_REQUEST_HEADER::VERB_TYPE_POST);

    std::map<std::string, std::string>::const_iterator  it = postParams.begin();
    while (it != postParams.end())
    {
        m_header.AddMultipartFormData(it->first, it->second);
        it++;
    }
    m_header.EndMultipartFormData();


    m_header.m_header += "Authorization:" + GetSign(postParams);
    m_header.m_url = url;
    m_header.m_tag = apiType;

    int taskId = AddRequest(m_header);
}

void InteractServerApi::CommonHttpGet(const std::string& url, int apiType, std::map<std::string, std::string> getParams)
{
    HTTP_REQUEST_HEADER   m_header(HTTP_REQUEST_HEADER::VERB_TYPE_GET);

    std::string getUrl = url;
    std::map<std::string, std::string>::const_iterator  it = getParams.begin();
    while (it != getParams.end())
    {
        int idx = getUrl.find("?");
        if (idx > 0) {
            getUrl.append("&");
        }
        else {
            getUrl.append("?");
        }

        getUrl.append(it->first);
        getUrl.append("=");
        getUrl.append(it->second);
        it++;
    }


    m_header.m_header += "Authorization:" + GetSign(getParams);
    m_header.m_url = getUrl;
    m_header.m_tag = apiType;
    int taskId = AddRequest(m_header);
}

void InteractServerApi::UserLogin()
{
    std::string url = HOST + "/userLogin";
    std::map<std::string, std::string> params;


    CommonHttpPost(url, Interact_ApiType_UserLogin, GetServerCommonParams());
}

void InteractServerApi::GetRoomList(const tstring& userId, int roomType) {
    std::string  url = HOST + "/getRoomList";

    std::map<std::string, std::string> params = GetServerCommonParams();
    params["userId"] = STR_T2U(userId);
    params["roomType"] = Util::String::StrFormat("%d", roomType);


    CommonHttpPost(url, Interact_ApiType_GetRoomList, params);
}

std::string InteractServerApi::GetSign(const std::map<std::string, std::string>& params)
{
    std::list<std::string> paramList;

    std::map<std::string, std::string>::const_iterator  it = params.begin();
    while (it != params.end())
    {
        std::string info = it->first;
        info.append("__");
        info.append(it->second);
        paramList.push_back(info);
        it++;
    }

    paramList.sort();

    std::list<std::string>::iterator listIt = paramList.begin();

    std::string allParams;
    while (listIt != paramList.end())
    {
        allParams.append(listIt->c_str());
        listIt++;
    }


    std::string sign = CMD5Checksum::GetMD5((BYTE*)allParams.c_str(), allParams.size());
    return sign;
}

bool InteractServerApi::ParseInteractUserModel(const Json::Value& jsonValue, InteractUserModel& userModel)
{
    if (!jsonValue.empty()) {
        if (!jsonValue["userId"].empty()) {
            userModel.userid = STR2T(jsonValue["userId"].asCString());
        }
        if (!jsonValue["nickname"].empty()) {
            userModel.nickname = STR2T(jsonValue["nickname"].asString());
        }
        if (!jsonValue["portraint"].empty()){
            userModel.portraint = STR2T(jsonValue["portraint"].asString());
        }
        if (!jsonValue["identity"].empty()) {
            userModel.identity = atoi(jsonValue["identity"].asCString());
        }
        if (!jsonValue["createTime"].empty()) {
            userModel.createTime = STR2T(jsonValue["createTime"].asCString());
        }

        return true;
    }

    return false;
}

bool InteractServerApi::ParseInteractRoomModel(const Json::Value& jsonValue, InteractRoomModel& roomModel)
{
    if (!jsonValue.empty()) {
        if (!jsonValue["roomId"].empty()) {
            roomModel.roomId = STR2T(jsonValue["roomId"].asCString());
        }
        if (!jsonValue["roomName"].empty()) {
            roomModel.roomName = STR2T(jsonValue["roomName"].asString());
        }
        if (!jsonValue["bindRoleId"].empty()){
            roomModel.bindRoleId = STR2T(jsonValue["bindRoleId"].asString());
        }
        if (!jsonValue["num"].empty()) {
            roomModel.onlineNum = atoi(jsonValue["num"].asCString());
        }
        if (!jsonValue["talkType"].empty()) {
            roomModel.talkType = atoi(jsonValue["talkType"].asCString());
        }
        if (!jsonValue["maxNum"].empty()) {
            roomModel.maxNum = atoi(jsonValue["maxNum"].asCString());
        }
        if (!jsonValue["roomType"].empty()) {
            roomModel.roomLifeType = atoi(jsonValue["roomType"].asCString());
        }
        if (!jsonValue["createTime"].empty()){
            roomModel.createTime = STR2T(jsonValue["createTime"].asString());
        }

        if (!jsonValue["identity"].empty()){
            int identity = atoi(jsonValue["identity"].asCString());
            if (identity == InteractConstant::USER_IDENTITY_ANCHOR || identity == InteractConstant::USER_IDENTITY_GUEST || identity == InteractConstant::USER_IDENTITY_AUDIENCE) {
                roomModel.userIdentity = (InteractConstant::USER_IDENTITY)identity;
            }
        }

        if (!jsonValue["list"].empty() && jsonValue["list"].isArray()){
            for (int i = 0; i < jsonValue["list"].size(); i++) {
                InteractUserModel userModel;
                ParseInteractUserModel(jsonValue["list"][i], userModel);
                roomModel.userList.push_back(userModel);
            }
        }
        return true;
    }

    return false;
}

bool InteractServerApi::ParseInteractIMModel(const Json::Value& jsonValue, InteractIMModel& imModel)
{
    if (!jsonValue.empty()) {
        if (!jsonValue["vendor"].empty()) {
            imModel.vendor = STR2T(jsonValue["vendor"].asCString());
        }
        if (!jsonValue["appKey"].empty()) {
            imModel.appKey = STR2T(jsonValue["appKey"].asString());
        }
        if (!jsonValue["appSecret"].empty()){
            imModel.appSecret = STR2T(jsonValue["appSecret"].asString());
        }

        return true;
    }

    return false;
}


void InteractServerApi::GetRoomInfo(const tstring& userId, const tstring& roomId)
{
    std::string  url = HOST + "/getRoomInfo";

    std::map<std::string, std::string> params = GetServerCommonParams();
    params["userId"] = STR_T2U(userId);
    params["roomId"] = STR_T2U(roomId);

    CommonHttpPost(url, Interact_ApiType_GetRoomInfo, params);
}

void InteractServerApi::CreateRoom(const tstring& userId, const tstring& roomName, int roomType, int talkType, int roomLifeType, int maxNum)
{
    std::string  url = HOST + "/createRoom";

    std::map<std::string, std::string> params = GetServerCommonParams();
    params["userId"] = STR_T2U(userId);
    params["roomName"] = STR_T2U(roomName);
    params["roomType"] = Util::String::StrFormat("%d", roomType);
    params["talkType"] = Util::String::StrFormat("%d", talkType);
    params["roomLifeType"] = Util::String::StrFormat("%d", roomLifeType);
    params["maxNum"] = Util::String::StrFormat("%d", maxNum);


    CommonHttpPost(url, Interact_ApiType_CreateRoom, params);
}

void InteractServerApi::JoinRoom(const tstring& userId, const tstring& roomId, int userIdentity)
{
    std::string  url = HOST + "/joinRoom";

    std::map<std::string, std::string> params = GetServerCommonParams();
    params["userId"] = STR_T2U(userId);
    params["roomId"] = STR_T2U(roomId);
    params["identity"] = Util::String::StrFormat("%d", userIdentity);


    CommonHttpPost(url, Interact_ApiType_JoinRoom, params);
}

void InteractServerApi::UserLeaveRoom(const tstring& userId, const tstring& roomId)
{
    std::string  url = HOST + "/userLeaveRoom";

    std::map<std::string, std::string> params = GetServerCommonParams();
    params["userId"] = STR_T2U(userId);
    params["roomId"] = STR_T2U(roomId);

    CommonHttpPost(url, Interact_ApiType_UserLeaveRoom, params);
}

void InteractServerApi::GetRoomUserList(const tstring& userId, const tstring& roomId, std::vector<int> userIdentitys)
{
    std::string  url = HOST + "/getRoomUserList";

    std::map<std::string, std::string> params = GetServerCommonParams();
    params["userId"] = STR_T2U(userId);
    params["roomId"] = STR_T2U(roomId);

    std::string indentiys;
    for (int i = 0; i < userIdentitys.size(); i++) {
        indentiys.append(Util::String::StrFormat("%d", userIdentitys[i]));
        if (i != userIdentitys.size() - 1) {
            indentiys.append(",");
        }
    }
    params["identity"] = indentiys;

    CommonHttpPost(url, Interact_ApiType_GetRoomUserList, params);
}

void InteractServerApi::ChangeUserdentity(const tstring& userId, const tstring& roomId, int userIdentity)
{
    std::string  url = HOST + "/changeUserIdentity";

    std::map<std::string, std::string> params = GetServerCommonParams();
    params["userId"] = STR_T2U(userId);
    params["roomId"] = STR_T2U(roomId);
    params["identity"] = Util::String::StrFormat("%d", userIdentity);;

    CommonHttpPost(url, Interact_ApiType_ChangeUserdentity, params);
}

void InteractServerApi::DismissRoom(const tstring& userId, const tstring& roomId)
{
    std::string  url = HOST + "/dismissRoom";

    std::map<std::string, std::string> params = GetServerCommonParams();
    params["userId"] = STR_T2U(userId);
    params["roomId"] = STR_T2U(roomId);

    CommonHttpPost(url, Interact_ApiType_DismissRoom, params);
}

void InteractServerApi::UserHeart(const tstring& userId, const tstring& roomId)
{
    std::string  url = HOST + "/userHeart";

    std::map<std::string, std::string> params = GetServerCommonParams();
    params["userId"] = STR_T2U(userId);
    params["roomId"] = STR_T2U(roomId);

    CommonHttpPost(url, Interact_ApiType_UserHeart, params);
}

void InteractServerApi::OnServerApiUserLoginResult(const InteractServerApiRet& ret, const InteractUserModel& interactUserModel)
{
    AutoThreadSection autolock(&mThreadSection);
    for (std::vector<InteractServerApiCallBack*>::iterator it = mCallbacks.begin(); it != mCallbacks.end(); ++it)
    {
        if (*it != NULL) {
            (*it)->OnServerApiUserLoginResult(ret, interactUserModel);
        }
    }
}

void InteractServerApi::OnServerApiGetRoomListResult(const InteractServerApiRet& ret, const std::vector<InteractRoomModel>& interactRoomModelList)
{
    AutoThreadSection autolock(&mThreadSection);
    for (std::vector<InteractServerApiCallBack*>::iterator it = mCallbacks.begin(); it != mCallbacks.end(); ++it)
    {
        if (*it != NULL) {
            (*it)->OnServerApiGetRoomListResult(ret, interactRoomModelList);
        }
    }
}

void InteractServerApi::OnServerApiGetRoomInfoResult(const InteractServerApiRet& ret, const InteractRoomModel& interactRoomModel)
{
    AutoThreadSection autolock(&mThreadSection);
    for (std::vector<InteractServerApiCallBack*>::iterator it = mCallbacks.begin(); it != mCallbacks.end(); ++it)
    {
        if (*it != NULL) {
            (*it)->OnServerApiGetRoomInfoResult(ret, interactRoomModel);
        }
    }
}

void InteractServerApi::OnServerApiCreateRoomResult(const InteractServerApiRet& ret, const InteractRoomModel& interactRoomModel)
{
    AutoThreadSection autolock(&mThreadSection);
    for (std::vector<InteractServerApiCallBack*>::iterator it = mCallbacks.begin(); it != mCallbacks.end(); ++it)
    {
        if (*it != NULL) {
            (*it)->OnServerApiCreateRoomResult(ret, interactRoomModel);
        }
    }
}

void InteractServerApi::OnServerApiJoinRoomResult(const InteractServerApiRet& ret, const InteractRoomModel& interactRoomModel)
{
    AutoThreadSection autolock(&mThreadSection);
    for (std::vector<InteractServerApiCallBack*>::iterator it = mCallbacks.begin(); it != mCallbacks.end(); ++it)
    {
        if (*it != NULL) {
            (*it)->OnServerApiJoinRoomResult(ret, interactRoomModel);
        }
    }
}

void InteractServerApi::OnServerApiUserLeaveRoomResult(const InteractServerApiRet& ret, const InteractRoomModel& interactRoomModel)
{
    AutoThreadSection autolock(&mThreadSection);
    for (std::vector<InteractServerApiCallBack*>::iterator it = mCallbacks.begin(); it != mCallbacks.end(); ++it)
    {
        if (*it != NULL) {
            (*it)->OnServerApiUserLeaveRoomResult(ret, interactRoomModel);
        }
    }
}

void InteractServerApi::OnServerApiGetRoomUserListResult(const InteractServerApiRet& ret, const std::vector<InteractUserModel>& interactUserModelList)
{
    AutoThreadSection autolock(&mThreadSection);
    for (std::vector<InteractServerApiCallBack*>::iterator it = mCallbacks.begin(); it != mCallbacks.end(); ++it)
    {
        if (*it != NULL) {
            (*it)->OnServerApiGetRoomUserListResult(ret, interactUserModelList);
        }
    }
}

void InteractServerApi::OnServerApiChangeUserdentityResult(const InteractServerApiRet& ret, const InteractRoomModel& interactRoomModel)
{
    AutoThreadSection autolock(&mThreadSection);
    for (std::vector<InteractServerApiCallBack*>::iterator it = mCallbacks.begin(); it != mCallbacks.end(); ++it)
    {
        if (*it != NULL) {
            (*it)->OnServerApiChangeUserdentityResult(ret, interactRoomModel);
        }
    }
}

void InteractServerApi::OnServerApiDismissRoomResult(const InteractServerApiRet& ret)
{
    AutoThreadSection autolock(&mThreadSection);
    for (std::vector<InteractServerApiCallBack*>::iterator it = mCallbacks.begin(); it != mCallbacks.end(); ++it)
    {
        if (*it != NULL) {
            (*it)->OnServerApiDismissRoomResult(ret);
        }
    }
}

void InteractServerApi::OnServerApiUserHeartResult(const InteractServerApiRet& ret)
{
    AutoThreadSection autolock(&mThreadSection);
    for (std::vector<InteractServerApiCallBack*>::iterator it = mCallbacks.begin(); it != mCallbacks.end(); ++it)
    {
        if (*it != NULL) {
            (*it)->OnServerApiUserHeartResult(ret);
        }
    }
}

void InteractServerApi::AddApiCallback(InteractServerApiCallBack* pCallback)
{
    AutoThreadSection autolock(&mThreadSection);
    if (pCallback != NULL) {
        std::vector<InteractServerApiCallBack*>::iterator itFind = std::find(mCallbacks.begin(), mCallbacks.end(), pCallback);
        if (itFind == mCallbacks.end())
        {
            mCallbacks.push_back(pCallback);
        }
    }
}

void InteractServerApi::RemoveApiCallback(InteractServerApiCallBack* pCallback)
{
    AutoThreadSection autolock(&mThreadSection);
    if (pCallback != NULL) {
        for (std::vector<InteractServerApiCallBack*>::iterator it = mCallbacks.begin(); it != mCallbacks.end(); ++it)
        {
            if (*it == pCallback) {
                mCallbacks.erase(it);
                break;
            }
        }
    }
}

std::string InteractServerApi::URLEncode(std::string url)
{
    char ret[1024] = { 0 };

    Util::Encode::URLEncode(url.c_str(), url.size(), ret, 1024);

    return ret;
}

void InteractServerApi::ParseCallbackRet(const std::string& receive, InteractServerApiRet& callbackRet, Json::Value& jsonData)
{
    Json::Reader reader;
    Json::Value root;

    if (reader.parse((const char*)receive.c_str(), root)) {
        if (!root["errno"].empty() && root["errno"].asInt() == 0) {
            jsonData = root["data"];
        }
        else {
            callbackRet.errCode = !root["errno"].empty() ? root["errno"].asInt() : 0;
            callbackRet.errMessage = !root["errmsg"].empty() ? STR2T(root["errmsg"].asString()) : _T("");
        }
    }
    else {
        callbackRet.errCode = -1;
        callbackRet.errMessage = _T("unknow!");
    }
}

