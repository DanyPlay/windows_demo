#ifndef LIVECLOUD_MODEL_DEF
#define LIVECLOUD_MODEL_DEF

#pragma once

#include <string>
#include <vector>
#include "InteractConfig.h"

#define INTERACT_TITLE _T("360视频云-互动直播")

namespace InteractConstant {

    /**
    * 房间类型
    */
    enum ROOM_TYPE
    {
        /**
        * 房间类型-主播&主播
        */
        ROOM_TYPE_ANCHOR_AND_ANCHOR = 1,

        /**
        * 房间类型-主播&嘉宾
        */
        ROOM_TYPE_ANCHOR_AND_GUEST = 2,

        /**
        * 房间类型-轰趴
        */
        ROOM_TYPE_HOME_PARTY = 3,
    };


    enum ROOM_LIFE_TYPE
    {
        /**
        * 房间生命周期类型-绑定到主播
        */
        ROOM_LIFE_TYPE_BIND_ANCHOR = 1,
        /**
        * 房间生命周期类型-绑定到房间
        */
        ROOM_LIFE_TYPE_BIND_ROOM = 2,
    };

    enum TALK_TYPE
    {
        /**
        * 互动方式-音视频
        */
        TALK_TYPE_ALL = 0,
        /**
        * 互动方式-仅音频
        */
        TALK_TYPE_AUDIO = 1,
        /**
        * 互动方式-仅视频
        */
        TALK_TYPE_VIDEO = 2,
    };

    enum USER_IDENTITY
    {
        /**
        * 用户身份信息-主播
        */
        USER_IDENTITY_ANCHOR = 1,
        /**
        * 用户身份信息-嘉宾
        */
        USER_IDENTITY_GUEST = 2,
        /**
        * 用户身份信息-观众
        */
        USER_IDENTITY_AUDIENCE = 0,
    };


}


struct InteractUserModel
{
    tstring userid;
    tstring nickname;
    tstring portraint;
    int identity;
    tstring createTime;

    InteractUserModel()
    {
        identity = 0;
    }

    InteractUserModel(const InteractUserModel& other)
    {
        this->userid = other.userid;
        this->nickname = other.nickname;
        this->portraint = other.portraint;
        this->identity = other.identity;
        this->createTime = other.createTime;
    }

    InteractUserModel& operator =(const InteractUserModel& other)
    {
        this->userid = other.userid;
        this->nickname = other.nickname;
        this->portraint = other.portraint;
        this->identity = other.identity;
        this->createTime = other.createTime;

        return *this;
    }
};


struct InteractRoomModel
{
    tstring roomName;
    tstring roomId;

    int onlineNum;
    tstring bindRoleId;
    int talkType;
    int maxNum;
    int roomLifeType;
    tstring createTime;
    InteractConstant::USER_IDENTITY userIdentity;


    std::vector<InteractUserModel> userList;

    InteractRoomModel() {
        onlineNum = 0;
        talkType = 0;
        maxNum = 0;
        roomLifeType = 0;
        userIdentity = InteractConstant::USER_IDENTITY_AUDIENCE;
    }

};

struct InteractIMModel
{
    tstring vendor;
    tstring appKey;
    tstring appSecret;
};

typedef struct InteractStats
{
    QHVC::INTERACT::RtcStats rtcStatus;
    QHVC::INTERACT::LocalVideoStats localVideoStatus;
    std::map<std::string, QHVC::INTERACT::RemoteVideoStats> mapRemoteVideoStatus;

    int txQuality;                       //发送的质量
    int rxQuality;                       //接收的质量

    InteractStats()
    {
        txQuality = 0;
        rxQuality = 0;
    }
} InteractStats;

#endif // LIVECLOUD_MODEL_DEF
