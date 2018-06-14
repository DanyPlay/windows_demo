#include "stdafx.h"
#include "InteractImManager.h"
#include <objbase.h>
#include "../InteractGlobalManager.h"


InteractImManager::InteractImManager()
{
    m_nConnectStatus = ImConstant::CONNECT_STATUS_UNKNOW;
}


InteractImManager::~InteractImManager()
{
}

//消息监听
void InteractImManager::message_callback(const char* json_str)
{
    InteractImManager::GetInstance().PostIMMessage((WPARAM)InteractImManager::IM_TYPE_RECEIVE_MESSAGE, (LPARAM)_strdup(json_str));
}

//异常监听
void InteractImManager::exception_callback(const char* json_str)
{
    InteractImManager::GetInstance().PostIMMessage((WPARAM)InteractImManager::IM_TYPE_EXCEPTION_CALLBACK, (LPARAM)_strdup(json_str));
}

//连接服务器
void InteractImManager::connect_callback(const char* json_str)
{
    InteractImManager::GetInstance().PostIMMessage((WPARAM)InteractImManager::IM_TYPE_CONNECT_CALLBACK, (LPARAM)_strdup(json_str));
}

std::string InteractImManager::getCommandNote(int command) {
    std::string  note = "未知";

    switch (command) {
        case ImConstant::CMD_ANCHOR_INVITE_GUEST:
            note = "cmd:anchor_invite_guest";
            break;
        case ImConstant::CMD_GUEST_AGREE_INVITE:
            note = "cmd:guess_agree_invite";
            break;
        case ImConstant::CMD_GUEST_REFUSE_INVITE:
            note = "cmd:guess_refuse_invite";
            break;
        case ImConstant::CMD_GUEST_ASK_JOIN:
            note = "cmd:guest_ask_join";
            break;
        case ImConstant::CMD_ANCHOR_AGREE_JOIN:
            note = "cmd:anchor_agree_join";
            break;
        case ImConstant::CMD_ANCHOR_REFUSE_JOIN:
            note = "cmd:anchor_refuse_join";
            break;
        case ImConstant::CMD_ANCHOR_KICKOUT_GUEST:
            note = "cmd:anchor_kickout_guset";
            break;
        case ImConstant::CMD_GUEST_JOIN_NOTIFY:
            note = "cmd:guest_join_notify";
            break;
        case ImConstant::CMD_ANCHOR_QUIT_NOTIFY:
            note = "cmd:anchor_quit_notify";
            break;
        case ImConstant::CMD_GUEST_QUIT_NOTIFY:
            note = "cmd:guest_quit_notify";
            break;
        case ImConstant::CMD_GUEST_KICKOUT_NOTIFY:
            note = "cmd:guest_kickout_notify";
            break;
        case ImConstant::CMD_AUDIENT_JOIN_NOTIFY:
            note = "cmd:audient_join_notify";
            break;
        case ImConstant::CMD_AUDIENT_QUIT_NOTIFY:
            note = "cmd:audient_leave_notify";
            break;
    }

    return note;
}

void InteractImManager::Init()
{
    CreateNotifyWnd();

    InteractIMModel& imModel = InteractGlobalManager::GetInstance().GetInteractIMModel();

    m_RongCloudServerApi.SetAppInfo(STR2A(imModel.appKey), STR2A(imModel.appSecret));
    m_RongCloudServerApi.SetApiCallback(this);

    char szPath[MAX_PATH] = { 0 };
    ::SHGetSpecialFolderPathA(NULL, szPath, CSIDL_APPDATA, TRUE);
    ::PathAddBackslashA(szPath);
    ::PathAppendA(szPath, DIR_INTERACT);
    ::PathAddBackslashA(szPath);
    ::PathAppendA(szPath, "im");
    ::PathAddBackslashA(szPath);


    Util::File::ForceCreateDir(STR_A2T(szPath).c_str());


    m_RongCloudApi.Init();
    m_RongCloudApi.InitClient(STR2A(imModel.appKey).c_str(), "socket_test", "deviceId", szPath, szPath);
    m_RongCloudApi.SetDeviceInfo("Apple", "iPhone 6 Simulator", "8.1.1", "WIFI", "");
    m_RongCloudApi.RegisterMessageType("RC:TxtMsg", 3);
    m_RongCloudApi.RegisterMessageType("RC:CmdMsg", 0);
    m_RongCloudApi.SetMessageListener(message_callback);
    m_RongCloudApi.SetExceptionListener(exception_callback);
}

void InteractImManager::UnInit()
{
    m_RongCloudServerApi.SetApiCallback(NULL);
    ::DestroyWindow(m_hWnd);
}

bool InteractImManager::Connect()
{
    m_nConnectStatus = ImConstant::CONNECT_STATUS_CONNECTING;
    InteractUserModel* userModel = InteractGlobalManager::GetInstance().GetInteractUserModel();

    m_RongCloudServerApi.GetToken(STR2A(userModel->userid), "haha", "haha");

    return true;
}

void InteractImManager::CheckConnect() {
    if (m_nConnectStatus != ImConstant::CONNECT_STATUS_CONNECTED && m_nConnectStatus != ImConstant::CONNECT_STATUS_CONNECTING) {
        Connect();
    }
}

void InteractImManager::DisConnect()
{
    m_RongCloudApi.Disconnect(4);
}

void InteractImManager::JoinChatRoom(const tstring& roomId)
{
    m_strCurrentRoomId = STR2A(roomId);
    m_RongCloudApi.JoinChatRoom(m_strCurrentRoomId.c_str(), 4, -1, [](const char* json_str)
    {
        InteractImManager::GetInstance().PostIMMessage((WPARAM)InteractImManager::IM_TYPE_JOIN_CHATROOM_CALLBACK, (LPARAM)_strdup(json_str));
    });
}
void InteractImManager::QuitChatRoom()
{
    m_RongCloudApi.QuitChatRoom(m_strCurrentRoomId.c_str(), 4, [](const char* json_str)
    {
        InteractImManager::GetInstance().PostIMMessage((WPARAM)InteractImManager::IM_TYPE_QUIT_CHATROOM_CALLBACK, (LPARAM)_strdup(json_str));
    });

    m_strCurrentRoomId = "";
}

void InteractImManager::SendCommand(const tstring& userId, int cmd, const tstring& cmdTip)
{
    SendCommandMessage(userId, ImConstant::CONVERSATION_TYPE_PRIVATE, cmd, cmdTip);
}

void InteractImManager::SendNotify(int cmd, const tstring& cmdTip)
{
    SendCommandMessage(STR2T(m_strCurrentRoomId), ImConstant::CONVERSATION_TYPE_CHATROOM, cmd, cmdTip);
}

void InteractImManager::AddApiCallback(InteractImCallback* pCallback)
{
    AutoThreadSection autolock(&mThreadSection);
    if (pCallback != NULL) {
        mCallbacks.push_back(pCallback);
    }
}

void InteractImManager::RemoveApiCallback(InteractImCallback* pCallback)
{
    AutoThreadSection autolock(&mThreadSection);
    if (pCallback != NULL) {
        for (std::vector<InteractImCallback*>::iterator it = mCallbacks.begin(); it != mCallbacks.end(); ++it)
        {
            if (*it == pCallback) {
                mCallbacks.erase(it);
                break;
            }
        }
    }
}

void InteractImManager::OnImConnectStatusChanged(ImConstant::CONNECT_STATUS connectStatus)
{
    AutoThreadSection autolock(&mThreadSection);
    m_nConnectStatus = connectStatus;
    for (std::vector<InteractImCallback*>::iterator it = mCallbacks.begin(); it != mCallbacks.end(); ++it)
    {
        if (*it != NULL) {
            (*it)->OnImConnectStatusChanged(connectStatus);
        }
    }
}

void InteractImManager::OnImSendMessageSuccess()
{
    AutoThreadSection autolock(&mThreadSection);
    for (std::vector<InteractImCallback*>::iterator it = mCallbacks.begin(); it != mCallbacks.end(); ++it)
    {
        if (*it != NULL) {
            (*it)->OnImSendMessageSuccess();
        }
    }
}

void InteractImManager::OnImSendMessageFailed(int errCode, const std::string& errMessage)
{
    AutoThreadSection autolock(&mThreadSection);
    for (std::vector<InteractImCallback*>::iterator it = mCallbacks.begin(); it != mCallbacks.end(); ++it)
    {
        if (*it != NULL) {
            (*it)->OnImSendMessageFailed(errCode, errMessage);
        }
    }
}


void InteractImManager::OnImJoinRoomSuccess(const std::string& roomId)
{
    AutoThreadSection autolock(&mThreadSection);
    for (std::vector<InteractImCallback*>::iterator it = mCallbacks.begin(); it != mCallbacks.end(); ++it)
    {
        if (*it != NULL) {
            (*it)->OnImJoinRoomSuccess(roomId);
        }
    }
}

void InteractImManager::OnImJoinRoomFailed(int errCode, const std::string& errMessage)
{
    AutoThreadSection autolock(&mThreadSection);
    for (std::vector<InteractImCallback*>::iterator it = mCallbacks.begin(); it != mCallbacks.end(); ++it)
    {
        if (*it != NULL) {
            (*it)->OnImJoinRoomFailed(errCode, errMessage);
        }
    }
}

void InteractImManager::OnImQuitRoomSuccess(const std::string& roomId)
{
    AutoThreadSection autolock(&mThreadSection);
    for (std::vector<InteractImCallback*>::iterator it = mCallbacks.begin(); it != mCallbacks.end(); ++it)
    {
        if (*it != NULL) {
            (*it)->OnImQuitRoomSuccess(roomId);
        }
    }
}

void InteractImManager::OnImQuitRoomFailed(int errCode, const std::string& errMessage)
{
    AutoThreadSection autolock(&mThreadSection);
    for (std::vector<InteractImCallback*>::iterator it = mCallbacks.begin(); it != mCallbacks.end(); ++it)
    {
        if (*it != NULL) {
            (*it)->OnImQuitRoomFailed(errCode, errMessage);
        }
    }
}

void InteractImManager::OnImReceiveCommand(const InteractUserModel& userFrom, const CommandInfo& command)
{
    AutoThreadSection autolock(&mThreadSection);
    for (std::vector<InteractImCallback*>::iterator it = mCallbacks.begin(); it != mCallbacks.end(); ++it)
    {
        if (*it != NULL) {
            (*it)->OnImReceiveCommand(userFrom, command);
        }
    }
}
void InteractImManager::OnImReceiveOtherCommand(const InteractUserModel& userFrom, const std::string& str)
{
    AutoThreadSection autolock(&mThreadSection);
    for (std::vector<InteractImCallback*>::iterator it = mCallbacks.begin(); it != mCallbacks.end(); ++it)
    {
        if (*it != NULL) {
            (*it)->OnImReceiveOtherCommand(userFrom, str);
        }
    }
}

void InteractImManager::OnGetTokenSuccess(const std::string& userId, const std::string& token)
{
    m_RongCloudApi.Connect(token.c_str(), connect_callback);

}

void InteractImManager::OnIMServerFailed(int errCode, const std::string& errMessage)
{
    OnImConnectStatusChanged(ImConstant::CONNECT_STATUS_SERVER_INVALID);
}

void InteractImManager::SendCommandMessage(const tstring& targetId, ImConstant::CONVERSATION_TYPE conversationType, int cmd, const tstring& cmdTip)
{
    InteractUserModel* userModel = InteractGlobalManager::GetInstance().GetInteractUserModel();

    Json::Value jobj;
    Json::FastWriter writer;
    //发送消息
    jobj["name"] = "command";
    jobj["user"]["id"] = STR2A(userModel->userid);
    jobj["user"]["name"] = "haha";
    jobj["user"]["portrait"] = "100197";
    jobj["data"]["cmd"] = cmd;
    jobj["data"]["cmd_tip"] = "";
    jobj["data"]["target"] = m_strCurrentRoomId;
    jobj["data"]["os_type"] = "pc";
    jobj["data"]["ver"] = "1.0";
    jobj["data"]["extra"] = "";

    std::string info = writer.write(jobj);

    int messageId = 0;


    m_RongCloudApi.sendMessage(STR2A(targetId).c_str(), 1, 2, "RC:CmdMsg", Util::Encode::ASCIIToWide(info).c_str(), "", " ", messageId, [](const char* json_str)
    {
        InteractImManager::GetInstance().PostIMMessage((WPARAM)InteractImManager::IM_TYPE_SEND_MESSAGE_CALLBACK, (LPARAM)strdup(json_str));
    });
}

void InteractImManager::CreateNotifyWnd()
{
    WNDCLASSEX   w = { 0 };
    w.cbSize = sizeof(w);
    w.hInstance = (HINSTANCE)&__ImageBase;
    w.lpszClassName = _T("im_event_wnd");
    w.lpfnWndProc = im_message_proc;
    ::RegisterClassEx(&w);

    m_hWnd = ::CreateWindowEx(0, w.lpszClassName, NULL, 0, 0, 0, 0, 0, HWND_MESSAGE, NULL, w.hInstance, 0);
    ::SetWindowLongPtr(m_hWnd, GWLP_USERDATA, (__int3264)(LONG_PTR)this); // __int3264 to remove warning in VS2005
}

void InteractImManager::PostIMMessage(WPARAM wParam, LPARAM lParam)
{
    ::PostMessage(m_hWnd, WM_IM_EVENT, wParam, lParam);
}

LRESULT InteractImManager::im_message_proc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    InteractImManager   * pThis = (InteractImManager*)(LONG_PTR)::GetWindowLongPtr(hWnd, GWLP_USERDATA);
    if (pThis)
    {
        switch (uMsg)
        {
            case InteractImManager::WM_IM_EVENT:
                char* json_str = (char*)lParam;
                pThis->OnIMEvent((int)wParam, json_str);
                free(json_str);
                return 0;
        }
    }
    return ::DefWindowProc(hWnd, uMsg, wParam, lParam);
}

void InteractImManager::OnIMEvent(int nType, char* json_str)
{
    switch (nType)
    {
        case IM_TYPE_RECEIVE_MESSAGE:
        {
            Json::Reader reader;
            Json::Value jobj;
            Json::Value msg_jobj;
            Json::Value data_jobj;
            if (reader.parse(json_str, jobj))  // reader将Json字符串解析到jobj将包含Json里所有子元素  
            {
                std::string ansi_msg_json_str = jobj["m_Message"].asString();
                if (reader.parse(ansi_msg_json_str, msg_jobj))
                {
                    data_jobj = msg_jobj["data"];
                    bool parseRet = true;
                    if (data_jobj.isString()){
                        std::string command = data_jobj.asString();
                        parseRet = reader.parse(command, data_jobj);
                    }
                    if (parseRet)
                    {
                        int cmd = data_jobj["cmd"].asInt();
                        std::string cmdTip = data_jobj["cmdTip"].asString();
                        CommandInfo commandInfo(cmd, STR2T(cmdTip), STR2T(data_jobj["target"].asString()));
                        commandInfo.osType = STR2T(data_jobj["osType"].asString());
                        commandInfo.ver = STR2T(data_jobj["ver"].asString());
                        commandInfo.extra = STR2T(data_jobj["extra"].asString());
                        commandInfo.receivedTime = STR2T(jobj["m_RcvTime"].asString());
                        commandInfo.sentTime = STR2T(jobj["m_SendTime"].asString());

                        InteractUserModel userModel;
                        userModel.userid = STR2T(msg_jobj["user"]["id"].asString());
                        userModel.nickname = STR2T(msg_jobj["user"]["name"].asString());
                        userModel.portraint = STR2T(msg_jobj["user"]["portrait"].asString());

                        OnImReceiveCommand(userModel, commandInfo);

                    }
                }
            }
            return;
        }
        case IM_TYPE_EXCEPTION_CALLBACK:
        {
            Json::Reader reader;
            Json::Value jobj;
            if (reader.parse(json_str, jobj))  // reader将Json字符串解析到jobj将包含Json里所有子元素  
            {
                int nstatus = jobj["nstatus"].asInt();
                if (nstatus == 30011) {
                    OnImConnectStatusChanged(ImConstant::CONNECT_STATUS_DISCONNECTED);
                }

            }
            return;
        }
        case IM_TYPE_CONNECT_CALLBACK:
        {
            Json::Reader reader;
            Json::Value jobj;
            if (reader.parse(json_str, jobj))  // reader将Json字符串解析到jobj将包含Json里所有子元素  
            {
                std::string result = jobj["result"].asString();
                std::string userId = jobj["userId"].asString();
                int err_code = jobj["err_code"].asInt();

                if (err_code == 0) {
                    OnImConnectStatusChanged(ImConstant::CONNECT_STATUS_CONNECTED);
                }
                else {
                    OnImConnectStatusChanged(ImConstant::CONNECT_STATUS_NETWORK_UNAVAILABLE);
                }
            }
            return;
        }
        case IM_TYPE_JOIN_CHATROOM_CALLBACK:
        {
            Json::Reader reader;
            Json::Value jobj;
            if (reader.parse(json_str, jobj))  // reader将Json字符串解析到jobj将包含Json里所有子元素  
            {
                if (!jobj["err_code"].empty()) {
                    int errCode = jobj["err_code"].asInt();
                    if (errCode == 0) {
                        OnImJoinRoomSuccess(InteractImManager::GetInstance().m_strCurrentRoomId);
                    }
                    else {
                        std::string errMsg;
                        if (!jobj["err_code"].empty()) {
                            errMsg = jobj["err_message"].asString();
                        }
                        OnImJoinRoomFailed(errCode, errMsg);
                    }
                }
                else {
                    OnImJoinRoomFailed(-1, json_str);
                }
            }
            return;
        }
        case IM_TYPE_QUIT_CHATROOM_CALLBACK:
        {
            Json::Reader reader;
            Json::Value jobj;
            if (reader.parse(json_str, jobj))  // reader将Json字符串解析到jobj将包含Json里所有子元素  
            {
                if (!jobj["err_code"].empty()) {
                    int errCode = jobj["err_code"].asInt();
                    if (errCode == 0) {
                        OnImQuitRoomSuccess(m_strCurrentRoomId);
                    }
                    else {
                        std::string errMsg;
                        if (!jobj["err_code"].empty()) {
                            errMsg = jobj["err_message"].asString();
                        }
                        OnImQuitRoomFailed(errCode, errMsg);
                    }
                }
                else {
                    OnImQuitRoomFailed(-1, json_str);
                }
            }
            return;
        }
        case IM_TYPE_SEND_MESSAGE_CALLBACK:
        {
            Json::Reader reader;
            Json::Value jobj;
            if (reader.parse(json_str, jobj))  // reader将Json字符串解析到jobj将包含Json里所有子元素  
            {
                if (!jobj["err_code"].empty()) {
                    int errCode = jobj["err_code"].asInt();
                    if (errCode == 0) {
                        OnImSendMessageSuccess();
                    }
                    else {
                        std::string errMsg;
                        if (!jobj["err_code"].empty()) {
                            errMsg = jobj["err_message"].asString();
                        }
                        OnImSendMessageFailed(errCode, errMsg);
                    }
                }
                else {
                    OnImSendMessageFailed(-1, json_str);
                }
            }
            return;
        }
    }
}


