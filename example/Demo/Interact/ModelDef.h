#ifndef LIVECLOUD_MODEL_DEF
#define LIVECLOUD_MODEL_DEF

#pragma once

#include <string>
#include <vector>
#include "InteractConfig.h"

#define INTERACT_TITLE _T("360��Ƶ��-����ֱ��")

namespace InteractConstant {

    /**
    * ��������
    */
    enum ROOM_TYPE
    {
        /**
        * ��������-����&����
        */
        ROOM_TYPE_ANCHOR_AND_ANCHOR = 1,

        /**
        * ��������-����&�α�
        */
        ROOM_TYPE_ANCHOR_AND_GUEST = 2,

        /**
        * ��������-��ſ
        */
        ROOM_TYPE_HOME_PARTY = 3,
    };


    enum ROOM_LIFE_TYPE
    {
        /**
        * ����������������-�󶨵�����
        */
        ROOM_LIFE_TYPE_BIND_ANCHOR = 1,
        /**
        * ����������������-�󶨵�����
        */
        ROOM_LIFE_TYPE_BIND_ROOM = 2,
    };

    enum TALK_TYPE
    {
        /**
        * ������ʽ-����Ƶ
        */
        TALK_TYPE_ALL = 0,
        /**
        * ������ʽ-����Ƶ
        */
        TALK_TYPE_AUDIO = 1,
        /**
        * ������ʽ-����Ƶ
        */
        TALK_TYPE_VIDEO = 2,
    };

    enum USER_IDENTITY
    {
        /**
        * �û������Ϣ-����
        */
        USER_IDENTITY_ANCHOR = 1,
        /**
        * �û������Ϣ-�α�
        */
        USER_IDENTITY_GUEST = 2,
        /**
        * �û������Ϣ-����
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

    int txQuality;                       //���͵�����
    int rxQuality;                       //���յ�����

    InteractStats()
    {
        txQuality = 0;
        rxQuality = 0;
    }
} InteractStats;

#endif // LIVECLOUD_MODEL_DEF
