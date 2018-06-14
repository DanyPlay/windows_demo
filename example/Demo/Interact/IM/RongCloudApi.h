#ifndef LIVECLOUD_INTERACT_RONGCLOUD_API_DEF
#define LIVECLOUD_INTERACT_RONGCLOUD_API_DEF
#pragma once
class RongCloudApi
{
public:
    RongCloudApi();
    ~RongCloudApi();

    typedef void(__stdcall *MessageListenerCallback)(const char* json_str);

    typedef void(__stdcall *ConnectAckListenerCallback)(const char* json_str);

    typedef void(__stdcall *ExceptionListenerCallback)(const char* json_str);

    typedef void(__stdcall *PublishAckListenerCallback)(const char* json_str);

    //初始化
    typedef int(*InitClientFun)(const char *appId, const char* appName, const char* deviceId, const char* localPath, const char* databasePath);

    //设置设备信息
    typedef int(*SetDeviceInfoFun)(const char* manufacturer, const char* model, const char* osVersion, const char* network, const char* networkOperator);

    //连接服务器
    typedef int(*ConnectFun)(const char* token, ConnectAckListenerCallback callback);

    //注册消息类型
    typedef int(*RegisterMessageTypeFun)(const char* clazzName, const unsigned int operateBits);

    //设置消息监听
    typedef int(*SetMessageListenerFun)(MessageListenerCallback callback);

    //设置网络异常监听
    typedef int(*SetExceptionListenerFun)(ExceptionListenerCallback callback);

    // 断开连接
    typedef int(*DisconnectFun)(int);

    //发消息前存储消息
    typedef int(*SaveMessageFun)(const char* targetId, int categoryId, const char* clazzName, const char* senderId, const wchar_t* message, const char* push, const char* appData);

    // 发送消息
    typedef int(*SendMessageFun)(const char* targetId, int categoryId, int transferType, const char* clazzname, const wchar_t* message, const char* push, const char* appData, int messageId, PublishAckListenerCallback callback);

    // 加入房间
    typedef int(*JoinChatRoomFun)(const char* targetId, int conversationType, int messageCount, PublishAckListenerCallback callback);

    // 退出房间
    typedef int(*QuitChatRoomFun)(const char* targetId, int conversationType, PublishAckListenerCallback callback);



private:
    HINSTANCE m_hDll; //DLL句柄 
    InitClientFun m_InitClientFun;
    SetDeviceInfoFun m_SetDeviceInfoFun;
    ConnectFun m_ConnectFun;
    RegisterMessageTypeFun m_RegisterMessageTypeFun;
    SetMessageListenerFun m_SetMessageListenerFun;
    SetExceptionListenerFun m_SetExceptionListenerFun;
    DisconnectFun m_DisconnectFun;
    SaveMessageFun m_SaveMessageFun;
    SendMessageFun m_SendMessageFun;
    JoinChatRoomFun m_JoinChatRoomFun;
    QuitChatRoomFun m_QuitChatRoomFun;
public:
    bool Init();
    bool UnInit();

    //rcsdk functions the functions for the developers
    /**
    *  初始化全局实例
    *
    *  @param appid        应用id
    *  @param appName      应用名称
    *  @param deviceId     设备id
    *  @param localPath    缓存路径
    *  @param databasePath 数据库存储路径
    *
    *  @return client指针   NULL - 失败 非NULL - 成功
    */

    void InitClient(const char *appId, const char* appName, const char* deviceId, const char* localPath, const char* databasePath);

    /**
    *  建立服务器连接
    *
    *  @param token    登录token，由后台配置从网站得到
    *  @param callback 监听连接结果通过监听中的函数通知
    */

    void Connect(const char* token, ConnectAckListenerCallback callback);

    /**
    *  断开服务器连接，不会删除全局实例
    *
    *  @param state 断开时指定的状态 2 断开连接收推送 4 断开连接不收推送
    */
    void Disconnect(int state);

    /**
    *  设置设备信息
    *
    *  @param manufacturer 设备生产商
    *
    *  @param model 设备型号
    *
    *  @param osVersion 设备系统版本
    *
    *  @param network 设备当前使用的网络
    *
    *  @param networkOperator 网络运营商
    */
    void SetDeviceInfo(const char* manufacturer, const char* model, const char* osVersion, const char* network, const char* networkOperator);


    /**
    *  设置消息监听
    *
    *  @param callback 消息监听器
    */
    void SetMessageListener(MessageListenerCallback callback);

    /**
    *  设置异常监听
    *
    *  @param callback 异常监听器
    */
    void SetExceptionListener(ExceptionListenerCallback callback);

    /**
    *  注册信息类别
    *
    *  @param clazzName   信息类别
    *  @param operateBits 操作位 1(00000001) - 是否入库 2(00000010) - 是否标记为已读
    *
    *  @return 操作结果 true - 成功 false - 失败
    */
    bool RegisterMessageType(const char* clazzName, const unsigned int operateBits);

    /**
    *  备注：
    *  上层定义的 PRIVATE DISCUSSION 等值需要与此对应，因为暂时的CHATROOM和TEMPGROUP都未使用，
    *  使用时，这两个值需匹配上。
    *  PB中通道的定义如下
    *  enum ChannelType
    *  {
    *      PERSON = 1,
    *      PERSONS = 2,
    *      GROUP = 3,
    *      TEMPGROUP = 4,
    *      CUSTOMERSERVICE = 5
    *  };
    */

    /*
    ************************************************
    *  会话处理相关函数
    ************************************************
    */

    /**
    *  发送消息
    *
    *  @param targetId     交互方id(接收方id)
    *  @param categoryId   通道类型
    1 - PRIVATE             单人
    2 - DISCUSSION          讨论组
    3 - GROUP               群组
    4 - CHATROOM            聊天室
    5 - CUSTOMERSERVICE     客服
    6 - SYSTEM              系统消息
    7 - MC                  MC消息
    8 - MP                  MP消息
    *  @param transferType 传输类别
    1 - S 消息数据服务器不缓存，不保证信息到达接收方
    2 - N 消息数据服务器缓存，交互方会收到通知消息
    3 - P 消息数据服务器缓存，交互方不在线会收到PUSH消息
    *  @param clazzname    消息类别(RC:ImgMsg RC:VcMsg RC:NtfMsg RC:StMsg)
    *  @param message      消息内容 json内容
    *  @param messageId    消息id 消息在数据库中的Id
    *  @param callback     发送监听
    */

    void sendMessage(const char* targetId, int categoryId, int transferType, const char* clazzname, const wchar_t* message, const char* push, const char* appData, int messageId, PublishAckListenerCallback callback);
    /**
    *  信息入库
    *
    *  @param targetId   交互方id
    *  @param categoryId 通道类型
    *  @param clazzName  消息类别
    *  @param senderId   发送方id
    *  @param message    消息内容
    *
    *  @return 信息在库中的id -1 失败 >0 成功
    */

    long SaveMessage(const char* targetId, int categoryId, const char* clazzName, const char* senderId, const wchar_t* message, const char* push, const char* appData);

    
    void JoinChatRoom(const char* targetId, int conversationType, int messageCount, PublishAckListenerCallback callback);

    void QuitChatRoom(const char* targetId, int conversationType, PublishAckListenerCallback callback);
};

#endif // LIVECLOUD_INTERACT_RONGCLOUD_API_DEF

