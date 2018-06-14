#ifndef LIVECLOUD_INTERACT_IM_MANAGER_DEF
#define LIVECLOUD_INTERACT_IM_MANAGER_DEF
#pragma once
#include "../ModelDef.h"
#include "../../CommonLib/Thread/ThreadLock.h"
#include "RongCloudApi.h"
#include "RongCloudServerApi.h"
namespace ImConstant {
    enum CMD_TYPE
    {
        /**
        * 主播发送邀请观众连麦
        */
        CMD_ANCHOR_INVITE_GUEST = 10000,
        /**
        * 观众同意主播连麦邀请
        */
        CMD_GUEST_AGREE_INVITE = 10001,
        /**
        * 观众拒绝主播连麦邀请
        */
        CMD_GUEST_REFUSE_INVITE = 10002,
        /**
        * 观众发送连麦请求
        */
        CMD_GUEST_ASK_JOIN = 10003,
        /**
        * 主播同意连麦请求
        */
        CMD_ANCHOR_AGREE_JOIN = 10004,
        /**
        * 主播拒绝连麦请求
        */
        CMD_ANCHOR_REFUSE_JOIN = 10005,

        /**
        * 主播踢出某个嘉宾
        */
        CMD_ANCHOR_KICKOUT_GUEST = 10006,

        /**
        * 嘉宾进入，发送通知
        */
        CMD_GUEST_JOIN_NOTIFY = 20000,
        /**
        * 主播退出，发送通知
        */
        CMD_ANCHOR_QUIT_NOTIFY = 20001,
        /**
        * 嘉宾退出，发送通知
        */
        CMD_GUEST_QUIT_NOTIFY = 20002,
        /**
        * 嘉宾被踢出，发送通知
        */
        CMD_GUEST_KICKOUT_NOTIFY = 20003,
        /**
        * 有观众进入，发送通知，可以用来刷新界面人数
        */

        CMD_AUDIENT_JOIN_NOTIFY = 20004,
        /**
        * 有观众退出，发送通知，可以用来刷新界面人数
        */
        CMD_AUDIENT_QUIT_NOTIFY = 20005,

    };

    enum CONNECT_STATUS
    {
        CONNECT_STATUS_UNKNOW = -2,

        CONNECT_STATUS_NETWORK_UNAVAILABLE = -1,

        CONNECT_STATUS_CONNECTED = 0,

        CONNECT_STATUS_CONNECTING,

        CONNECT_STATUS_DISCONNECTED,

        CONNECT_STATUS_KICKED_OFFLINE_BY_OTHER_CLIENT,

        CONNECT_STATUS_TOKEN_INCORRECT,

        CONNECT_STATUS_SERVER_INVALID
    };

    enum  CONVERSATION_TYPE {
        CONVERSATION_TYPE_PRIVATE = 1,
        CONVERSATION_TYPE_CHATROOM = 4,
    };
}


struct CommandInfo {
    int cmd;
    tstring cmdTip;
    tstring target;
    tstring osType;
    tstring ver;
    tstring extra;
    tstring sentTime;
    tstring receivedTime;

    CommandInfo(int command, const tstring& cmdTip, const tstring& target) {
        this->cmd = command;
        this->cmdTip = cmdTip;
        this->target = target;
    }
};


/**
* 收到命令消息的回调函数
*/
class InteractImCallback {
public:
    /**
    * 连接状态发生改变
    * @param connectStatus 链接状态
    */
    virtual void OnImConnectStatusChanged(ImConstant::CONNECT_STATUS connectStatus) {};

    /**
    * 发送消息成功
    */
    virtual void OnImSendMessageSuccess() {};

    /**
    * 发送消息失败
    */
    virtual void OnImSendMessageFailed(int errCode, const std::string& errMessage){};

    /**
    * 加入房间成功
    */
    virtual void OnImJoinRoomSuccess(const std::string& roomId) {};

    /**
    * 加入房间失败
    */
    virtual void OnImJoinRoomFailed(int errCode, const std::string& errMessage){};

    /**
    * 退出房间成功
    */
    virtual void OnImQuitRoomSuccess(const std::string& roomId) {};

    /**
    * 退出房间失败
    */
    virtual void OnImQuitRoomFailed(int errCode, const std::string& errMessage){};

    /**
    * 收到已经知道的消息
    * @param userFrom 用户信息
    * @param command 命令结构
    */
    virtual void OnImReceiveCommand(const InteractUserModel& userFrom, const CommandInfo& command){};

    /**
    * 收到已经知道的消息
    * @param userFrom 用户信息
    * @param str 命令字符
    */

    virtual void OnImReceiveOtherCommand(const InteractUserModel& userFrom, const std::string& str){};
};


class InteractImManager : public InteractImCallback, RongCloudServerApiCallBack
{
public:
    enum
    {
        WM_IM_EVENT = (WM_APP + 0x300),
    };

    enum
    {
        IM_TYPE_RECEIVE_MESSAGE = 1000,
        IM_TYPE_EXCEPTION_CALLBACK,
        IM_TYPE_CONNECT_CALLBACK,
        IM_TYPE_JOIN_CHATROOM_CALLBACK,
        IM_TYPE_QUIT_CHATROOM_CALLBACK,
        IM_TYPE_SEND_MESSAGE_CALLBACK,
    };

public:
    InteractImManager();
    ~InteractImManager();

    static InteractImManager& GetInstance() {
        static InteractImManager instance;
        return instance;
    }
public:
    std::string getCommandNote(int command);

    void Init();
    void UnInit();

    bool Connect();
    void CheckConnect();
    void DisConnect();

    void JoinChatRoom(const tstring& roomId);
    void QuitChatRoom();

    void SendCommand(const tstring& userId, int cmd, const tstring& cmdTip);
    void SendNotify(int cmd, const tstring& cmdTip);

    void AddApiCallback(InteractImCallback* pCallback);
    void RemoveApiCallback(InteractImCallback* pCallback);

    ImConstant::CONNECT_STATUS GetConnectStatus();

    void OnIMEvent(int nType, char* json_str);

private:
    virtual void OnImConnectStatusChanged(ImConstant::CONNECT_STATUS connectStatus);

    virtual void OnImSendMessageSuccess();
    virtual void OnImSendMessageFailed(int errCode, const std::string& errMessage);

    virtual void OnImJoinRoomSuccess(const std::string& roomId);
    virtual void OnImJoinRoomFailed(int errCode, const std::string& errMessage);

    virtual void OnImQuitRoomSuccess(const std::string& roomId);
    virtual void OnImQuitRoomFailed(int errCode, const std::string& errMessage);

    virtual void OnImReceiveCommand(const InteractUserModel& userFrom, const CommandInfo& command);
    virtual void OnImReceiveOtherCommand(const InteractUserModel& userFrom, const std::string& str);

private:
    virtual void OnGetTokenSuccess(const std::string& userId, const std::string& token);
    virtual void OnIMServerFailed(int errCode, const std::string& errMessage);
private:
    void SendCommandMessage(const tstring& targetId, ImConstant::CONVERSATION_TYPE conversationType, int cmd, const tstring& cmdTip);
private:
    static void __stdcall message_callback(const char* json_str);
    static void __stdcall connect_callback(const char* json_str);
    static void __stdcall exception_callback(const char* json_str);
    static LRESULT __stdcall im_message_proc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);



private:
    void CreateNotifyWnd();
    void PostIMMessage(WPARAM wParam, LPARAM lParam);
private:
    std::vector<InteractImCallback*> mCallbacks;

    ThreadSection mThreadSection;

    ImConstant::CONNECT_STATUS m_nConnectStatus;

    RongCloudServerApi m_RongCloudServerApi;
    RongCloudApi m_RongCloudApi;

    std::string m_strCurrentRoomId;

    HWND m_hWnd;
};

#endif // LIVECLOUD_INTERACT_IM_MANAGER_DEF

