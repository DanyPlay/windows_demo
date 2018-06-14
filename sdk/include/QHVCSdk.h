#ifndef _QHVC_SDK_H_
#define _QHVC_SDK_H_

#pragma once

#include "QHVCSdkConstant.h"

namespace QHVC
{
    namespace QHVCSdk
    {
        /**
        * 视频云SDK配置信息
        */
        class QHVCSdkConfig
        {
        public:
            char szMachineId[64];           //设备唯一标识
            char szAppVersion[32];          //应用程序版本号
            char szUserId[64];              //用户唯一标识，可以为空
            char szCacheDir[MAX_PATH];      //数据文件缓存目录，建议指定为%appdata%目录

            char szBusinessId[64];          //视频云分配的Business ID
            char szControlUrl[MAX_PATH];    //视频云云控URL，可以为空

            QHVCSdkConfig()
            {
                ZeroMemory(this, sizeof(QHVCSdkConfig));
            }
        };

        /**
        * 获取SDK版本号
        *
        * @return SDK版本号
        */
        QHVC_API char* GetVersion();

        /**
        * 视频云SDK初始化
        *
        * @param pSdkConfig SDK配置
        * @return true 初始化成功；false 初始化失败
        */
        QHVC_API bool Init(const QHVCSdkConfig* pSdkConfig);

        /**
        * 获取视频云SDK配置信息
        *
        * @return 视频云SDK配置信息
        */
        QHVC_API QHVCSdkConfig* GetSdkConfig();

        /**
        * 设置日志输出等级
        *
        * @param logLevel 日志输出等级，低于该等级的日志将不被输出
        */
        QHVC_API void SetLogLevel(E_LOG_LEVEL logLevel);

        /**
        * 获取日志输出等级
        *
        * @return 日志输出等级
        */
        QHVC_API E_LOG_LEVEL GetLogLevel();
    }
}

#endif
