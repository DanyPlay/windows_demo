#ifndef _QHVC_SDK_CONSTANT_H_
#define _QHVC_SDK_CONSTANT_H_

#pragma once

#ifdef QHVC_EXPORTS
#define QHVC_API __declspec(dllexport)
#else
#define QHVC_API __declspec(dllimport)
#endif

namespace QHVC
{
    namespace QHVCSdk
    {
        enum E_LOG_LEVEL
        {
            LOG_LEVEL_OFF = 0,
            LOG_LEVEL_TRACE,
            LOG_LEVEL_DEBUG,
            LOG_LEVEL_INFO,
            LOG_LEVEL_WARN,
            LOG_LEVEL_ERROR,
        };
    }
}

#endif
