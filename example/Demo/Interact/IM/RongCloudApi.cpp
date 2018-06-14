#include "stdafx.h"
#include "RongCloudApi.h"
#include <objbase.h>


RongCloudApi::RongCloudApi()
{
    m_InitClientFun = NULL;
    m_SetDeviceInfoFun = NULL;
    m_ConnectFun = NULL;
    m_RegisterMessageTypeFun = NULL;
    m_SetMessageListenerFun = NULL;
    m_SetExceptionListenerFun = NULL;
    m_DisconnectFun = NULL;
    m_SaveMessageFun = NULL;
    m_SendMessageFun = NULL;
}


RongCloudApi::~RongCloudApi()
{
}

bool RongCloudApi::Init()
{
    std::string dll_file_name = "rcsdk.dll";
    char path[512] = { 0 };
    GetModuleFileNameA(NULL, path, 511);
    std::string module_path = path;
    std::string dll_path = module_path.substr(0, module_path.rfind("\\") + 1);
    dll_path.append(dll_file_name);


    m_hDll = LoadLibraryA(dll_path.c_str());
    if (m_hDll != NULL) {
        m_InitClientFun = (InitClientFun)GetProcAddress(m_hDll, "InitClient");
        m_SetDeviceInfoFun = (SetDeviceInfoFun)GetProcAddress(m_hDll, "SetDeviceInfo");
        m_ConnectFun = (ConnectFun)GetProcAddress(m_hDll, "Connect");
        m_RegisterMessageTypeFun = (RegisterMessageTypeFun)GetProcAddress(m_hDll, "RegisterMessageType");
        m_SetMessageListenerFun = (SetMessageListenerFun)GetProcAddress(m_hDll, "SetMessageListener");
        m_SetExceptionListenerFun = (SetExceptionListenerFun)GetProcAddress(m_hDll, "SetExceptionListener");
        m_DisconnectFun = (DisconnectFun)GetProcAddress(m_hDll, "Disconnect");
        m_SaveMessageFun = (SaveMessageFun)GetProcAddress(m_hDll, "SaveMessage");
        m_SendMessageFun = (SendMessageFun)GetProcAddress(m_hDll, "sendMessage");
        m_JoinChatRoomFun = (JoinChatRoomFun)GetProcAddress(m_hDll, "JoinChatRoom");
        m_QuitChatRoomFun = (QuitChatRoomFun)GetProcAddress(m_hDll, "QuitChatRoom");

    }

    return true;
}

bool RongCloudApi::UnInit() {

    FreeLibrary(m_hDll);

    return true;
}

void RongCloudApi::InitClient(const char *appId, const char* appName, const char* deviceId, const char* localPath, const char* databasePath)
{
    if (m_InitClientFun) {
        try {
            m_InitClientFun(appId, appName, deviceId, localPath, databasePath);
        }
        catch (...) {

        }

    }
}

void RongCloudApi::Connect(const char* token, ConnectAckListenerCallback callback)
{
    if (m_ConnectFun) {
        try {
            m_ConnectFun(token, callback);
        }
        catch (...) {

        }
    }
}

void RongCloudApi::Disconnect(int state)
{
    if (m_DisconnectFun) {
        try {
            m_DisconnectFun(state);
        }
        catch (...) {

        }
    }
}

void RongCloudApi::SetDeviceInfo(const char* manufacturer, const char* model, const char* osVersion, const char* network, const char* networkOperator)
{
    if (m_SetDeviceInfoFun) {
        try {
            m_SetDeviceInfoFun(manufacturer, model, osVersion, network, networkOperator);
        }
        catch (...) {

        }
    }
}

void RongCloudApi::SetMessageListener(MessageListenerCallback callback)
{
    if (m_SetMessageListenerFun) {
        try {
            m_SetMessageListenerFun(callback);
        }
        catch (...) {

        }
    }
}

void RongCloudApi::SetExceptionListener(ExceptionListenerCallback callback)
{
    if (m_SetExceptionListenerFun) {
        try {
            m_SetExceptionListenerFun(callback);
        }
        catch (...) {

        }
    }
}

bool RongCloudApi::RegisterMessageType(const char* clazzName, const unsigned int operateBits)
{
    if (m_RegisterMessageTypeFun) {
        try {
            return m_RegisterMessageTypeFun(clazzName, operateBits);
        }
        catch (...) {

        }
    }

    return false;
}

void RongCloudApi::sendMessage(const char* targetId, int categoryId, int transferType, const char* clazzname, const wchar_t* message, const char* push, const char* appData, int messageId, PublishAckListenerCallback callback)
{
    if (m_SendMessageFun) {
        try {
            m_SendMessageFun(targetId, categoryId, transferType, clazzname, message, push, appData, messageId, callback);
        }
        catch (...) {

        }
    }
}

long RongCloudApi::SaveMessage(const char* targetId, int categoryId, const char* clazzName, const char* senderId, const wchar_t* message, const char* push, const char* appData)
{
    if (m_SaveMessageFun) {
        try {
            return m_SaveMessageFun(targetId, categoryId, clazzName, senderId, message, push, appData);
        }
        catch (...) {

        }
    }

    return false;
}

void RongCloudApi::JoinChatRoom(const char* targetId, int conversationType, int messageCount, PublishAckListenerCallback callback)
{
    if (m_JoinChatRoomFun) {
        try {
            m_JoinChatRoomFun(targetId, conversationType, messageCount, callback);
        }
        catch (...) {

        }
    }
}

void RongCloudApi::QuitChatRoom(const char* targetId, int conversationType, PublishAckListenerCallback callback)
{
    if (m_QuitChatRoomFun) {
        try {
            m_QuitChatRoomFun(targetId, conversationType, callback);
        }
        catch (...) {

        }
    }
}

