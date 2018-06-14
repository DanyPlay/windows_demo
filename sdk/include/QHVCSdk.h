#ifndef _QHVC_SDK_H_
#define _QHVC_SDK_H_

#pragma once

#include "QHVCSdkConstant.h"

namespace QHVC
{
    namespace QHVCSdk
    {
        /**
        * ��Ƶ��SDK������Ϣ
        */
        class QHVCSdkConfig
        {
        public:
            char szMachineId[64];           //�豸Ψһ��ʶ
            char szAppVersion[32];          //Ӧ�ó���汾��
            char szUserId[64];              //�û�Ψһ��ʶ������Ϊ��
            char szCacheDir[MAX_PATH];      //�����ļ�����Ŀ¼������ָ��Ϊ%appdata%Ŀ¼

            char szBusinessId[64];          //��Ƶ�Ʒ����Business ID
            char szControlUrl[MAX_PATH];    //��Ƶ���ƿ�URL������Ϊ��

            QHVCSdkConfig()
            {
                ZeroMemory(this, sizeof(QHVCSdkConfig));
            }
        };

        /**
        * ��ȡSDK�汾��
        *
        * @return SDK�汾��
        */
        QHVC_API char* GetVersion();

        /**
        * ��Ƶ��SDK��ʼ��
        *
        * @param pSdkConfig SDK����
        * @return true ��ʼ���ɹ���false ��ʼ��ʧ��
        */
        QHVC_API bool Init(const QHVCSdkConfig* pSdkConfig);

        /**
        * ��ȡ��Ƶ��SDK������Ϣ
        *
        * @return ��Ƶ��SDK������Ϣ
        */
        QHVC_API QHVCSdkConfig* GetSdkConfig();

        /**
        * ������־����ȼ�
        *
        * @param logLevel ��־����ȼ������ڸõȼ�����־���������
        */
        QHVC_API void SetLogLevel(E_LOG_LEVEL logLevel);

        /**
        * ��ȡ��־����ȼ�
        *
        * @return ��־����ȼ�
        */
        QHVC_API E_LOG_LEVEL GetLogLevel();
    }
}

#endif
