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

    //��ʼ��
    typedef int(*InitClientFun)(const char *appId, const char* appName, const char* deviceId, const char* localPath, const char* databasePath);

    //�����豸��Ϣ
    typedef int(*SetDeviceInfoFun)(const char* manufacturer, const char* model, const char* osVersion, const char* network, const char* networkOperator);

    //���ӷ�����
    typedef int(*ConnectFun)(const char* token, ConnectAckListenerCallback callback);

    //ע����Ϣ����
    typedef int(*RegisterMessageTypeFun)(const char* clazzName, const unsigned int operateBits);

    //������Ϣ����
    typedef int(*SetMessageListenerFun)(MessageListenerCallback callback);

    //���������쳣����
    typedef int(*SetExceptionListenerFun)(ExceptionListenerCallback callback);

    // �Ͽ�����
    typedef int(*DisconnectFun)(int);

    //����Ϣǰ�洢��Ϣ
    typedef int(*SaveMessageFun)(const char* targetId, int categoryId, const char* clazzName, const char* senderId, const wchar_t* message, const char* push, const char* appData);

    // ������Ϣ
    typedef int(*SendMessageFun)(const char* targetId, int categoryId, int transferType, const char* clazzname, const wchar_t* message, const char* push, const char* appData, int messageId, PublishAckListenerCallback callback);

    // ���뷿��
    typedef int(*JoinChatRoomFun)(const char* targetId, int conversationType, int messageCount, PublishAckListenerCallback callback);

    // �˳�����
    typedef int(*QuitChatRoomFun)(const char* targetId, int conversationType, PublishAckListenerCallback callback);



private:
    HINSTANCE m_hDll; //DLL��� 
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
    *  ��ʼ��ȫ��ʵ��
    *
    *  @param appid        Ӧ��id
    *  @param appName      Ӧ������
    *  @param deviceId     �豸id
    *  @param localPath    ����·��
    *  @param databasePath ���ݿ�洢·��
    *
    *  @return clientָ��   NULL - ʧ�� ��NULL - �ɹ�
    */

    void InitClient(const char *appId, const char* appName, const char* deviceId, const char* localPath, const char* databasePath);

    /**
    *  ��������������
    *
    *  @param token    ��¼token���ɺ�̨���ô���վ�õ�
    *  @param callback �������ӽ��ͨ�������еĺ���֪ͨ
    */

    void Connect(const char* token, ConnectAckListenerCallback callback);

    /**
    *  �Ͽ����������ӣ�����ɾ��ȫ��ʵ��
    *
    *  @param state �Ͽ�ʱָ����״̬ 2 �Ͽ����������� 4 �Ͽ����Ӳ�������
    */
    void Disconnect(int state);

    /**
    *  �����豸��Ϣ
    *
    *  @param manufacturer �豸������
    *
    *  @param model �豸�ͺ�
    *
    *  @param osVersion �豸ϵͳ�汾
    *
    *  @param network �豸��ǰʹ�õ�����
    *
    *  @param networkOperator ������Ӫ��
    */
    void SetDeviceInfo(const char* manufacturer, const char* model, const char* osVersion, const char* network, const char* networkOperator);


    /**
    *  ������Ϣ����
    *
    *  @param callback ��Ϣ������
    */
    void SetMessageListener(MessageListenerCallback callback);

    /**
    *  �����쳣����
    *
    *  @param callback �쳣������
    */
    void SetExceptionListener(ExceptionListenerCallback callback);

    /**
    *  ע����Ϣ���
    *
    *  @param clazzName   ��Ϣ���
    *  @param operateBits ����λ 1(00000001) - �Ƿ���� 2(00000010) - �Ƿ���Ϊ�Ѷ�
    *
    *  @return ������� true - �ɹ� false - ʧ��
    */
    bool RegisterMessageType(const char* clazzName, const unsigned int operateBits);

    /**
    *  ��ע��
    *  �ϲ㶨��� PRIVATE DISCUSSION ��ֵ��Ҫ��˶�Ӧ����Ϊ��ʱ��CHATROOM��TEMPGROUP��δʹ�ã�
    *  ʹ��ʱ��������ֵ��ƥ���ϡ�
    *  PB��ͨ���Ķ�������
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
    *  �Ự������غ���
    ************************************************
    */

    /**
    *  ������Ϣ
    *
    *  @param targetId     ������id(���շ�id)
    *  @param categoryId   ͨ������
    1 - PRIVATE             ����
    2 - DISCUSSION          ������
    3 - GROUP               Ⱥ��
    4 - CHATROOM            ������
    5 - CUSTOMERSERVICE     �ͷ�
    6 - SYSTEM              ϵͳ��Ϣ
    7 - MC                  MC��Ϣ
    8 - MP                  MP��Ϣ
    *  @param transferType �������
    1 - S ��Ϣ���ݷ����������棬����֤��Ϣ������շ�
    2 - N ��Ϣ���ݷ��������棬���������յ�֪ͨ��Ϣ
    3 - P ��Ϣ���ݷ��������棬�����������߻��յ�PUSH��Ϣ
    *  @param clazzname    ��Ϣ���(RC:ImgMsg RC:VcMsg RC:NtfMsg RC:StMsg)
    *  @param message      ��Ϣ���� json����
    *  @param messageId    ��Ϣid ��Ϣ�����ݿ��е�Id
    *  @param callback     ���ͼ���
    */

    void sendMessage(const char* targetId, int categoryId, int transferType, const char* clazzname, const wchar_t* message, const char* push, const char* appData, int messageId, PublishAckListenerCallback callback);
    /**
    *  ��Ϣ���
    *
    *  @param targetId   ������id
    *  @param categoryId ͨ������
    *  @param clazzName  ��Ϣ���
    *  @param senderId   ���ͷ�id
    *  @param message    ��Ϣ����
    *
    *  @return ��Ϣ�ڿ��е�id -1 ʧ�� >0 �ɹ�
    */

    long SaveMessage(const char* targetId, int categoryId, const char* clazzName, const char* senderId, const wchar_t* message, const char* push, const char* appData);

    
    void JoinChatRoom(const char* targetId, int conversationType, int messageCount, PublishAckListenerCallback callback);

    void QuitChatRoom(const char* targetId, int conversationType, PublishAckListenerCallback callback);
};

#endif // LIVECLOUD_INTERACT_RONGCLOUD_API_DEF

