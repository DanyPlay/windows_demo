#ifndef _QHVC_INTERACT_CALLBACK_H_
#define _QHVC_INTERACT_CALLBACK_H_

#pragma once

#include "QHVCInteractConstant.h"

namespace QHVC
{
    namespace INTERACT
    {
        /**
        * 通话统计信息
        */
        typedef struct RtcStats
        {
            unsigned int totalDuration;         //累计通话时长(秒)
            unsigned int txBytes;               //累计发送字节数(bytes)
            unsigned int rxBytes;               //累计接收字节数(bytes)
            unsigned int txKBitRate;            //发送码率(kbps)，瞬时值
            unsigned int rxKBitRate;            //接收码率(kbps)，瞬时值

            unsigned int txAudioKBitRate;       //音频发送码率(kbps)，瞬时值
            unsigned int rxAudioKBitRate;       //音频接收码率(kbps)，瞬时值
            unsigned int txVideoKBitRate;       //视频发送码率(kbps)，瞬时值
            unsigned int rxVideoKBitRate;       //视频接收码率(kbps)，瞬时值

            int users;                          //用户数
            double cpuTotalUsage;               //当前系统的CPU使用率(%)
            double cpuAppUsage;                 //当前应用程序的CPU使用率(%)

            RtcStats()
            {
                ZeroMemory(this, sizeof(RtcStats));
            }
        } RtcStats;

        /**
        * 本地视频统计信息
        */
        typedef struct LocalVideoStats
        {
            unsigned int sentBitrate;           //自上次统计后发送字节数(bytes)
            unsigned int sentFrameRate;         //自上次统计后发送的帧数
            unsigned int packageLost;           //自上次统计后丢包数

            LocalVideoStats()
            {
                ZeroMemory(this, sizeof(LocalVideoStats));
            }
        } LocalVideoStats;

        /**
        * 远端视频统计信息
        */
        typedef struct RemoteVideoStats
        {
            char userId[64];                    //用户ID
            unsigned int delay;                 //延迟
            unsigned int width;                 //视频分辨率-宽
            unsigned int height;                //视频分辨率-高
            unsigned int receivedBitrate;       //自上次统计后接受的字节数(byte)
            unsigned int receivedFrameRate;     //自上次统计后接受的帧数
            VIDEO_STREAM_LEVEL rxStreamType;    //视频流等级
            QUALITY quality;                    //视频流质量

            RemoteVideoStats()
            {
                ZeroMemory(this, sizeof(RemoteVideoStats));
            }
        } RemoteVideoStats;

        /**
        * 音频音量信息
        */
        typedef struct AudioVolumeInfo
        {
            char userId[64];;                   //用户ID
            unsigned int volume;                //音量

            AudioVolumeInfo()
            {
                ZeroMemory(this, sizeof(AudioVolumeInfo));
            }
        } AudioVolumeInfo;

        class IQHVCInteractCallback
        {
        public:
            /**
            * 警告回调
            * 注：该回调表示SDK运行时出现了（网络或媒体相关的）警告。通常情况下，SDK上报的警告信息应用程序可以忽略，SDK会自动恢复
            *
            * @param warn 警告码，参考{@link INTERACT::WARN}定义
            * @param msg 警告信息
            */
            virtual void OnWarning(int warn, const char* msg) = 0;

            /**
            * 错误回调
            * 注：该回调表示SDK运行时出现了（网络或媒体相关的）错误。通常情况下，SDK上报的错误意味着SDK无法自动恢复，需要应用程序干预
            * 或提示用户。
            *
            * @param error 错误码，参考{@link INTERACT::ERR}定义
            * @param msg 错误信息
            */
            virtual void OnError(int error, const char* msg) = 0;

            /**
            * 加载互动直播引擎成功，此时业务方可以进行一系列参数的设置，之后调用{@link JoinChannel}及其他操作
            *
            * @param roomId 房间ID
            * @param userId 用户ID
            */
            virtual void OnLoadEngineSuccess(const char* roomId, const char* userId) = 0;

            /**
            * 加入房间成功
            *
            * @param roomId 房间ID
            * @param userId 用户ID
            * @param elapsed 从{@link JoinChannel}开始到该事件产生的延迟，单位毫秒
            */
            virtual void OnJoinChannelSuccess(const char* roomId, const char* userId, long elapsed) = 0;

            /**
            * 重新加入房间成功，由于网络原因，客户端可能会和服务器失去连接，SDK会进行自动重连，自动重连成功后触发此回调
            *
            * @param roomId 房间ID
            * @param userId 用户ID
            * @param elapsed 从{@link JoinChannel}开始到该事件产生的延迟，单位毫秒
            */
            virtual void OnRejoinChannelSuccess(const char* roomId, const char* userId, long elapsed) = 0;

            /**
            * 离开房间回调
            *
            * @param rtcStats 通话相关的统计信息
            */
            virtual void OnLeaveChannel(const RtcStats& rtcStats) = 0;

            /**
            * 角色身份切换成功回调
            * 当调用{@link SetClientRole}方法切换角色身份时，切换成功后回调此方法。如果切换身份失败，会回调onError()
            * 注：只有加入频道成功之后切换身份才会有此回调。
            *
            * @param clientRole 改变后的角色类型
            */
            virtual void OnChangeClientRoleSuccess(CLIENT_ROLE clientRole) = 0;

            /**
            * 连接中断回调，该回调表示SDK和服务器失去了网络连接。失去连接后，除非应用程序主动调用{@link LeaveChannel}，
            * SDK会一直尝试自动重连
            */
            virtual void OnConnectionInterrupted() = 0;

            /**
            * 连接丢失回调，该回调表示SDK和服务器失去了网络链接，并且尝试自动重连一段时间（默认10秒）后仍未连上
            *
            * @param error 错误码，参考{@link INTERACT::ERR}定义
            */
            virtual void OnConnectionLost(int error) = 0;

            /**
            * 其他用户加入当前房间
            * 注：如果当前用户加入房间时已经有人在房间内，SDK也会向应用程序上报已在房间中的用户
            *
            * @param userId 用户ID
            * @param elapsed 从{@link JoinChannel}开始到该事件产生的延迟，单位毫秒
            */
            virtual void OnUserJoined(const char* userId, long elapsed) = 0;

            /**
            * 其他用户离开当前房间
            * 注：SDK判断用户离开房间的依据：在一定时间内（15秒）没有收到对方的任何数据包，判定为对方掉线。在网络较差的
            * 情况下，可能存在误报。建议可靠的掉线检测由应用程序通过长连信令来做。
            *
            * @param userId 用户ID
            * @param userOfflineReason 用户离开原因
            */
            virtual void OnUserOffline(const char* userId, USER_OFFLINE_REASON userOfflineReason) = 0;

            /**
            * 其他用户启用/关闭视频
            * 注:关闭视频功能是指该用户只能进行语音通话，不能显示、发送自己的视频，也不能接收别人的视频
            *
            * @param userId 用户ID
            * @param enabled true 启用视频；false 关闭视频
            */
            virtual void OnUserEnableVideo(const char* userId, bool enabled) = 0;

            /**
            * 用户静音回调
            *
            * @param userId 用户ID
            * @param muted true 用户将音频静音；false 用户取消了音频静音
            */
            virtual void OnUserMuteAudio(const char* userId, bool muted) = 0;

            /**
            * 用户禁止/重启视频回调
            *
            * @param userId 用户ID
            * @param muted true 用户禁止视频发送；false 用户重启视频发送
            */
            virtual void OnUserMuteVideo(const char* userId, bool muted) = 0;

            /**
            * 摄像头启用回调，可以开始捕获视频
            */
            virtual void OnCameraReady() = 0;

            /**
            * 所有视频停止回调
            */
            virtual void OnVideoStopped() = 0;

            /**
            * 本地视频显示回调，第一帧本地视频画面已经显示在屏幕上
            *
            * @param width 视频宽度，单位像素
            * @param height 视频高度，单位像素
            * @param elapsed 从加入房间开始到该事件产生的延迟，单位毫秒
            */
            virtual void OnFirstLocalVideoFrame(int width, int height, int elapsed) = 0;

            /**
            * 远端视频显示回调，第一帧远端视频画面已经显示在屏幕上
            *
            * @param userId 用户ID
            * @param width 视频宽度，单位像素
            * @param height 视频高度，单位像素
            * @param elapsed 从加入房间开始到该事件产生的延迟，单位毫秒
            */
            virtual void OnFirstRemoteVideoFrame(const char* userId, int width, int height, int elapsed) = 0;

            /**
            * 远端视频接收并解码回调，应用程序可以在此回调中设置该用户的View（HWND）
            *
            * @param userId 用户ID
            * @param width 视频宽度，单位像素
            * @param height 视频高度，单位像素
            * @param elapsed 从加入房间开始到该事件产生的延迟，单位毫秒
            */
            virtual void OnFirstRemoteVideoDecoded(const char* userId, int width, int height, int elapsed) = 0;

            /**
            * 本地视频统计信息回调
            *
            * @param stats 本地视频统计信息
            */
            virtual void OnLocalVideoStats(const LocalVideoStats& stats) = 0;

            /**
            * 远端视频统计信息回调
            *
            * @param stats 远端视频统计信息
            */
            virtual void OnRemoteVideoStats(const RemoteVideoStats& stats) = 0;

            /**
            * 音频质量回调，通话过程中每两秒触发一次，报告当前通话的音频质量
            *
            * @param userId 用户ID
            * @param quality 音频质量评分
            * @param delay 延迟，单位毫秒
            * @param lost 丢包率，百分比
            */
            virtual void OnAudioQuality(const char* userId, QUALITY quality, unsigned short delay, unsigned short lost) = 0;

            /**
            * 说话声音音量回调
            *
            * @param speakers 说话者
            * @param totalVolume 混音后的总音量（0-255）
            */
            virtual void OnAudioVolumeIndication(const AudioVolumeInfo* speakers, int speakerNumber, unsigned int totalVolume) = 0;

            /**
            * 统计数据回调，该回调定期上报SDK的运行状态，每2-3秒触发一次
            *
            * @param stats 通话统计信息
            */
            virtual void OnRtcStats(const RtcStats& stats) = 0;

            /**
            * 网络质量回调
            *
            * @param userId 用户ID
            * @param txQuality 上行网络质量
            * @param rxQality 下行网络质量
            */
            virtual void OnNetworkQuality(const char* userId, QUALITY txQuality, QUALITY rxQuality) = 0;

            /**
            * 音频设备状态改变
            *
            * @param deviceId 设备ID
            * @param deviceType 音频设备类型，输入或输出设备
            * @param deviceState 音频设备状态，可用、禁用、不存在、拔出等
            */
            virtual void OnAudioDeviceStateChanged(const char* deviceId, AUDIO_DEVICE_TYPE deviceType, DEVICE_STATE deviceState) = 0;

            /**
            * 视频设备状态改变
            *
            * @param deviceId 设备ID
            * @param deviceState 视频设备状态，可用、禁用、不存在、拔出等
            */
            virtual void OnVideoDeviceStateChanged(const char* deviceId, DEVICE_STATE deviceState) = 0;

            /**
            * 伴奏播放结束
            */
            virtual void OnAudioMixingFinished() = 0;

            /**
            * 其他用户开始播放伴奏
            */
            virtual void OnRemoteAudioMixingBegin() = 0;

            /**
            * 其他用户结束播放伴奏
            */
            virtual void OnRemoteAudioMixingEnd() = 0;

            /**
            * 音效播放结束
            */
            virtual void OnAudioEffectFinished(int soundId) = 0;
        };

        class IQHVCInteractLocalVideoRenderCallback
        {
        public:
            /**
            * 本地视频数据回调。用于获得摄像头采集的图像
            * @param videoFrame 视频数据
            * @return true:  方法调用成功，flase表示失败
            */
            virtual bool OnCaptureVideoFrame(VideoFrame& videoFrame) = 0;
        };

        class IQHVCInteractRemoteVideoRenderCallback
        {
        public:
            /**
            * 远端数据回调。用于获得其他用户的视频。
            * @param userId 用户ID
            * @param videoFrame 视频数据
            * @return true:  方法调用成功，flase表示失败
            */
            virtual bool OnRenderVideoFrame(const char* userId, VideoFrame& videoFrame) = 0;
        };

        class IQHVCInteractAudioFrameCallback
        {
        public:
            /**
            *  连麦时，本地（自己的）声音数据输出回调
            * @param AudioFrame 音频数据
            * @return true:  方法调用成功，flase表示失败
            */
            virtual bool OnRecordAudioFrame(AudioFrame& audioFrame) = 0;

            /**
            *  连麦声音数据输出回调
            * @param AudioFrame 音频数据
            * @return true:  方法调用成功，flase表示失败
            */
            virtual bool OnPlaybackAudioFrame(AudioFrame& audioFrame) = 0;
        };
    }
}

#endif
