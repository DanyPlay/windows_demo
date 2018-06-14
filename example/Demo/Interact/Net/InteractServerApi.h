#pragma once

#include "../../CommonLib/http/http_request_manager.h"
#include "../../CommonLib/json/json.h"
#include "../ModelDef.h"
#include "../../CommonLib/Thread/ThreadLock.h"
#include <vector>


enum ApiType
{
    Interact_ApiType_UserLogin,
    Interact_ApiType_GetRoomList,
    Interact_ApiType_GetRoomInfo,
    Interact_ApiType_CreateRoom,
    Interact_ApiType_JoinRoom,
    Interact_ApiType_UserLeaveRoom,
    Interact_ApiType_GetRoomUserList,
    Interact_ApiType_ChangeUserdentity,
    Interact_ApiType_DismissRoom,
    Interact_ApiType_UserHeart,

    Interact_ApiType_Unknown = 1000

};

class InteractServerApiRet {
public:
    int errCode;
    tstring errMessage;

    InteractServerApiRet() {
        errCode = 0;
    }
};

class InteractServerApiCallBack {
public:
    virtual void OnServerApiUserLoginResult(const InteractServerApiRet& ret, const InteractUserModel& interactUserModel){}

    virtual void OnServerApiGetRoomListResult(const InteractServerApiRet& ret, const std::vector<InteractRoomModel>& interactRoomModelList){}

    virtual void OnServerApiGetRoomInfoResult(const InteractServerApiRet& ret, const InteractRoomModel& interactRoomModel){}

    virtual void OnServerApiCreateRoomResult(const InteractServerApiRet& ret, const InteractRoomModel& interactRoomModel){}

    virtual void OnServerApiJoinRoomResult(const InteractServerApiRet& ret, const InteractRoomModel& interactRoomModel){}

    virtual void OnServerApiUserLeaveRoomResult(const InteractServerApiRet& ret, const InteractRoomModel& interactRoomModel){}

    virtual void OnServerApiGetRoomUserListResult(const InteractServerApiRet& ret, const std::vector<InteractUserModel>& interactUserModelList){}

    virtual void OnServerApiChangeUserdentityResult(const InteractServerApiRet& ret, const InteractRoomModel& interactRoomModel){}

    virtual void OnServerApiDismissRoomResult(const InteractServerApiRet& ret){}

    virtual void OnServerApiUserHeartResult(const InteractServerApiRet& ret){}
};


class InteractServerApi : public FCHttpRequestManager, InteractServerApiCallBack
{
public:
    InteractServerApi();
    ~InteractServerApi();

    static InteractServerApi& GetInstance() {
        static InteractServerApi instance;
        return instance;
    }


public:
    void UserLogin();
    void GetRoomList(const tstring& userId, int roomType);
    void GetRoomInfo(const tstring& userId, const tstring& roomId);
    void CreateRoom(const tstring& userId, const tstring& roomName, int roomType, int talkType, int roomLifeType, int maxNum);
    void JoinRoom(const tstring& userId, const tstring& roomId, int userIdentity);
    void UserLeaveRoom(const tstring& userId, const tstring& roomId);
    void GetRoomUserList(const tstring& userId, const tstring& roomId, std::vector<int> userIdentitys);
    void ChangeUserdentity(const tstring& userId, const tstring& roomId, int userIdentity);
    void DismissRoom(const tstring& userId, const tstring& roomId);
    void UserHeart(const tstring& userId, const tstring& roomId);

    void AddApiCallback(InteractServerApiCallBack* pCallback);
    void RemoveApiCallback(InteractServerApiCallBack* pCallback);
private:
    virtual void OnServerApiUserLoginResult(const InteractServerApiRet& ret, const InteractUserModel& interactUserModel);

    virtual void OnServerApiGetRoomListResult(const InteractServerApiRet& ret, const std::vector<InteractRoomModel>& interactRoomModelList);

    virtual void OnServerApiGetRoomInfoResult(const InteractServerApiRet& ret, const InteractRoomModel& interactRoomModel);

    virtual void OnServerApiCreateRoomResult(const InteractServerApiRet& ret, const InteractRoomModel& interactRoomModel);

    virtual void OnServerApiJoinRoomResult(const InteractServerApiRet& ret, const InteractRoomModel& interactRoomModel);

    virtual void OnServerApiUserLeaveRoomResult(const InteractServerApiRet& ret, const InteractRoomModel& interactRoomModel);

    virtual void OnServerApiGetRoomUserListResult(const InteractServerApiRet& ret, const std::vector<InteractUserModel>& interactUserModelList);

    virtual void OnServerApiChangeUserdentityResult(const InteractServerApiRet& ret, const InteractRoomModel& interactRoomModel);

    virtual void OnServerApiDismissRoomResult(const InteractServerApiRet& ret);

    virtual void OnServerApiUserHeartResult(const InteractServerApiRet& ret);
private:
    virtual void OnAfterRequestSend(FCHttpRequest& rTask);
    virtual void OnAfterRequestFinish(FCHttpRequest& rTask);

    void CommonHttpGet(const std::string& url, int apiType, std::map<std::string, std::string> getParams);
    void CommonHttpPost(const std::string& url, int apiType, std::map<std::string, std::string> postParams);


    std::map<std::string, std::string> GetServerCommonParams();

    std::string GetSign(const std::map<std::string, std::string>& params);

    static bool ParseInteractUserModel(const Json::Value& jsonValue, InteractUserModel& userModel);
    static bool ParseInteractRoomModel(const Json::Value& jsonValue, InteractRoomModel& roomModel);
    static bool ParseInteractIMModel(const Json::Value& jsonValue, InteractIMModel& imModel);

    std::string URLEncode(std::string url);

    void ParseCallbackRet(const std::string& receive, InteractServerApiRet& ret, Json::Value& jsonData);

private:
    std::vector<InteractServerApiCallBack*> mCallbacks;

    ThreadSection mThreadSection;
};

