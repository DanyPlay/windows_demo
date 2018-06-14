#ifndef _QHVC_INTERACT_H_
#define _QHVC_INTERACT_H_

#pragma once

#include "QHVCSdk.h"
#include "QHVCInteractCallback.h"
#include "QHVCInteractConstant.h"

namespace QHVC
{
    namespace INTERACT
    {
        namespace QHVCInteract {

            /**
            * 获取SDK版本号
            *
            * @return SDK版本号
            */
            QHVC_API const char* GetVersion();

#pragma region 基本流程

            /**
            * 设置公共业务信息该方法需要在所有实例方法之前调用，用于设置业务所需要的一些必要信息，便于业务区分、统计使用。
            *
            * @param channelId 渠道Id，用于区分公司或部门下拥有多款应用。由直播云为应用程序开发者签发，如果没有，请向直播云申请。
            * @param appKey 直播云为应用程序开发者签发的App Key。如果没有，请向直播云申请。
            * @param userSign  用户签名。开发者需结合直播云提供的算法生成此 User Sign，用于服务器端用户验证。
            */
            QHVC_API void SetPublicServiceInfo(const char* channelId, const char*  appKey, const char*  userSign);

            /**
            * 加载互动直播引擎数据，进行准备工作。调用该方法前，必须先调用setPublicServiceInfo进行业务信息准备，
            * 必须在该方法调用后才能调用其它实例方法。该方法是异步执行，待回调执行之后才能执行其它实例方法。
            *
            * @param roomId           房间Id
            * @param userId           用户id
            * @param sessionId        会话ID，用于标识业务会话请求，每一次完整的流程之后，该值需要重新设置
            * @param optionInfo       可选字典，使用json串。若需旁路直播功能等可通过该字典设置，如果业务准备使用视频云的直播服务，
            *                         可以通过applyforBypassLiveAddress接口申请推流服务； 
            *                         例如: {"push_addr":"xxx", "pull_addr";"xxxx"},具体参数参见
            *                         {@link QHVC#INTERACT#ENGINE_OPTION}定义。
            * @param interactCallBack 回调
            * @return 0:表示成功,其他表示失败，见｛@link QHVC#INTERACT#ERR｝
            */
            QHVC_API int LoadEngine(const char*  roomId, const char*  userId, const char*  sessionId, const char* optionInfo,
                const IQHVCInteractCallback* interactCallBack);

            /**
            * 释放连麦引擎
            */
            QHVC_API void ReleaseEngine();


            /**
            * 该方法让用户加入通话频道，在同一个频道内的用户可以互相通话，多个用户加入同一个频道，
            * 可以群聊。如果已在通话中，用户必须调用leaveChannel 退出当前通话，才能进入下一个频道。
            *
            * @return 0：方法调用成功，其他表示失败，见｛@link QHVC#INTERACT#ERR｝
            */
            QHVC_API int JoinChannel();


            /**
            * 离开频道
            * 即挂断或退出通话。joinChannel后，必须调用leaveChannel以结束通话，否则不能进行下一次通话。不管当前
            * 是否在通话中，都可以调用leaveChannel，没有副作用。leaveChannel会把会话相关的所有资源释放掉。leaveChannel
            * 是异步操作，调用返回时并没有真正退出频道。在真正退出频道后，SDK会触发IQHVCInteractCallBack.onLeaveChannel()。
            *
            * @return 0 成功, 其他表示失败，见｛@link QHVC#INTERACT#ERR｝
            */
            QHVC_API int LeaveChannel();

            /**
            * 该方法用于设置频道模式(Profile)。 SDK需知道应用程序的使用场景， 从而使用
            * 不同的优化手段。
            * 注：
            *     1. 同一频道内只能同时设置一种模式。
            *     2. 该方法必须在进入频道前设置，在频道中设置无效。
            *
            * @param profile 指定频道的通话模式(profile)。支持以下模式：
            *                QHVC#INTERACT#CHANNEL_PROFILE_COMMUNICATION (通信模式：默认)
            *                QHVC#INTERACT#CHANNEL_PROFILE _LIVE_BROADCASTING (直播模式)
            *                QHVC#INTERACT#CHANNEL_PROFILE_GAME (游戏语言模式)
            * @return 0：方法调用成功，其他表示失败，见｛@link QHVC#INTERACT#ERR｝
            */
            QHVC_API int SetChannelProfile(CHANNEL_PROFILE profile);

            /**
            * 开启视频模式, 该方法用于开启视频模式。可以在加入频道前或者通话中调用，在加入频道前调用，则自动开
            * 启视频模式，在通话中调用则由音频模式切换为视频模式。调用 disablevideo 方法可关闭视
            * 频模式。
            *
            * @return 0：方法调用成功，其他表示失败，见｛@link QHVC#INTERACT#ERR｝
            */
            QHVC_API int EnableVideo();

            /**
            * 开启纯音频模式, 该方法用于关闭视频，开启纯音频模式。可以在加入频道前或者通话中调用，在加入频道前调
            * 用，则自动开启纯音频模式，在通话中调用则由视频模式切换为纯音频频模式。调用
            * enablevideo 方法可开启视频模式。
            *
            * @return 0：方法调用成功，其他表示失败，见｛@link QHVC#INTERACT#ERR｝
            */
            QHVC_API int DisableVideo();

            /**
            * 禁用/启用本地视频功能。该方法用于只看不发的视频场景。该方法不需要本地有摄像头。
            *
            * @param enabled true: 启用本地视频（默认） false: 禁用本地视频
            * @return 0：方法调用成功，其他表示失败，见｛@link QHVC#INTERACT#ERR｝
            */
            QHVC_API int EnableLocalVideo(bool enabled);

            /**
            * 该方法允许用户设置视频的优化选项。
            *
            * @param preferFrameRateOverImageQuality true: 画质和流畅度里，优先保证画质(默认),false: 画质和流畅度里，优先保证流畅度
            * @return 0 成功，其他表示失败，见｛@link QHVC#INTERACT#ERR｝
            */
            QHVC_API int SetVideoQualityParameters(bool preferFrameRateOverImageQuality);

            /**
            * 该方法用于启动本地视频预览。在开启预览前，必须先调用 setupLocalVideo 设置预览窗口及
            * 属性，且必须调用 enableVideo 开启视频功能。如果在调用 joinChannel 进入频道之前调用了
            * startPreview 启动本地视频预览，在调用 leaveChannel 退出频道之后本地预览仍然处于启动
            * 状态，如需要关闭本地预览，需要调用 stopPreview。
            * 注：本方法仅适用于使用SDK采集视频，对于业务层自采集视频时，请不要调用。
            *
            * @return 0：方法调用成功，其他表示失败，见｛@link QHVC#INTERACT#ERR｝
            */
            QHVC_API int StartPreview();

            /**
            * 该方法用于停止本地视频预览。
            *
            * @return 0：方法调用成功，其他表示失败，见｛@link QHVC#INTERACT#ERR｝
            */
            QHVC_API int StopPreview();

            /**
            * 开启语音模式
            * 该方法开启语音模式(默认为开启状态)。
            *
            * @return 0 成功，其他表示失败，见｛@link QHVC#INTERACT#ERR｝
            */
            QHVC_API int EnableAudio();

            /**
            * 关闭语音模式
            *
            * @return 0 成功，其他表示失败，见｛@link QHVC#INTERACT#ERR｝
            */
            QHVC_API int DisableAudio();

            /**
            * 设置一些SDK的特殊参数
            *
            * @param options 必须是json格式
            * @return 0 成功，其他表示失败，见｛@link QHVC#INTERACT#ERR｝
            */
            QHVC_API int SetParameters(const char* options);

            /**
            * 设置和切换用户模式
            * 在加入频道前，用户需要通过本方法设置观众或主播模式（默认）。
            * 在加入频道后，用户可以通过本方法切换用户模式。
            * 该方法仅在直播模式有效。
            *
            * @param role 直播的用户角色。 QHVC#INTERACT#CLIENT_ROLE_BROADCASTER :主播
            *             QHVC#INTERACT#CLIENT_ROLE_GUEST 嘉宾
            * @return 0：方法调用成功，其他表示失败，见｛@link QHVC#INTERACT#ERR｝
            */
            QHVC_API int SetClientRole(CLIENT_ROLE role);

            /**
            * 设置云引擎角色
            *
            * @param role 云引擎角色名称，需要与视频云云引擎配置一致，具体参数列表见云引擎配置
            * @return 0：方法调用成功，其他表示失败，见｛@link QHVC#INTERACT#ERR｝
            */
            QHVC_API int SetCloudControlRole(const char* role);

            /**
            * 该方法设置本地视频显示信息。应用程序通过调用此接口绑定本地视频流的显示视窗(view)，
            * 并设置视频显示模式。在应用程序开发中， 通常在初始化后调用该方法进行本地视频设置，
            * 然后再加入频道。退出频道后，绑定仍然有效，如果需要解除绑定，可以指定空(NULL)View
            * 调用 setupLocalVideo。
            *
            * @param view       视频显示视窗。
            * @param renderMode 视频显示模式。
            *                   QHVC#INTERACT#RENDER_MODE_HIDDEN (1): 如果视频尺寸与显示视窗尺寸不一致，则视频流会按照显示视窗的比例进行周边裁剪或图像拉伸后填满视窗。
            *                   QHVC#INTERACT#ENDER_MODE_FIT(2): 如果视频尺寸与显示视窗尺寸不一致，在保持长宽比的前提下，将视频进行缩放后填满视窗。
            *                   QHVC#INTERACT#RENDER_MODE_ADAPTIVE(3)：如果自己和对方都是竖屏，或者如果自己和对方都是横屏，使用RENDER_MODE_HIDDEN；如果对方和自己一个
            *                   竖屏一个横屏，则使用 RENDER_MODE_FIT。
            * @param uid        本地用户ID, 与 joinchannel 方法中的 uid 保持一致。
            * @return 0：方法调用成功，其他表示失败，见｛@link QHVC#INTERACT#ERR｝
            */
            QHVC_API int SetupLocalVideo(HWND view, RENDER_MODE renderMode, const char* uid);

            /**
            * 该方法绑定远程用户和显示视图，即设定 uid 指定的用户用哪个视图来显示哪一路流。建议由业务端来控制什么时候显示。解除某个用户的绑定视图可以调用removeRemoteVideo。
            * 退出频道后， SDK 会把远程用户的绑定关系清除掉。
            *
            * @param view       视频显示视窗。
            * @param renderMode 视频显示模式。
            *                   QHVC#INTERACT#RENDER_MODE_HIDDEN (1): 如果视频尺寸与显示视窗尺寸不一致，则视频流会按照显示视窗的比例进行周边裁剪或图像拉伸后填满视窗。
            *                   QHVC#INTERACT#RENDER_MODE_FIT(2): 如果视频尺寸与显示视窗尺寸不一致，在保持长宽比的前提下，将视频进行缩放后填满视窗。
            *                   QHVC#INTERACT#RENDER_MODE_ADAPTIVE(3)：如果自己和对方都是竖屏，或者如果自己和对方都是横屏，使用 RENDER_MODE_HIDDEN；如果对方和自己一个竖屏一个横屏，则使用RENDER_MODE_FIT。
            * @param uid        用户 ID，指定远端视频来自哪个用户。
            * @param streamId   流ID，指定远端视频的流ID
            * @return 0：方法调用成功，其他表示失败，见｛@link QHVC#INTERACT#ERR｝
            */
            QHVC_API int SetupRemoteVideo(HWND view, RENDER_MODE renderMode, const char* uid, const char* streamId);

            /**
            * 该方法用于解绑远程用户的显示视图，当远端用户离开频道后，调用此方法来解绑。
            *
            * @param streamId 指定远端视频的流ID
            * @param uid      用户ID，指定远端视频来自哪个用户
            * @return 0：方法调用成功，其他表示失败，见｛@link QHVC#INTERACT#ERR｝
            */
            QHVC_API int RemoveRemoteVideo(const char* streamId, const char* uid);

            /**
            * 该方法设置本地视频显示模式。应用程序可以多次调用此方法更改显示模式。
            * 注：本方法仅适用于使用SDK采集视频，对于业务层自采集视频时，请不要调用。
            *
            * @param mode 设置视频显示模式。
            *             QHVC#INTERACT#RENDER_MODE_HIDDEN (1): 如果视频尺寸与显示视窗尺寸不一致，则视频流会按照显示视窗的比例进行周边裁剪或图像拉伸后填满视窗。
            *             QHVC#INTERACT#RENDER_MODE_FIT(2): 如果视频尺寸与显示视窗尺寸不一致，在保持长宽比的前提下，将视频进行缩放后填满视窗。
            *             QHVC#INTERACT#RENDER_MODE_ADAPTIVE(3)：如果自己和对方都是竖屏，或者如果自己和对方都是横屏，使用RENDER_MODE_HIDDEN；如果对方和自己一个竖屏一个横屏，则使用 RENDER_MODE_FIT。
            * @return 0：方法调用成功，其他表示失败，见｛@link QHVC#INTERACT#ERR｝
            */
            QHVC_API int SetLocalRenderMode(RENDER_MODE mode);

            /**
            * 该方法设置远端视频显示模式。应用程序可以多次调用此方法更改显示模式。
            *
            * @param uid  用户 ID
            * @param mode 设置视频显示模式。
            * @return 0：方法调用成功，其他表示失败，见｛@link QHVC#INTERACT#ERR｝
            */
            QHVC_API int SetRemoteRenderMode(const char*  uid, RENDER_MODE mode);

            /**
            * 该方法设置视频编码属性(Profile)。每个属性对应一套视频参数，如分辨率、帧率、码率等。当设备的摄像头
            * 不支持指定的分辨率时， SDK 会自动选择一个合适的摄像头分辨率，但是编码分辨率仍然用 setVideoProfile 指定的。
            * 注：1. 应在调用 joinChannel/startPreview 前设置视频属性。
            * 2. 该方法仅设置编码器编出的码流属性，可能跟最终显示的属性不一致，例如编码码流分辨率为 640x480，码流
            * 的旋转属性为 90 度，则显示出来的分辨率为竖屏模式。
            *
            * @param profile            视频属性(Profile)。详见QHVCInteractConstant.VideoProfile中的定义。
            * @param swapWidthAndHeight 是否交换宽和高。    true：交换宽和高  false：不交换宽和高(默认)
            * @return 0：方法调用成功，其他表示失败，见｛@link QHVC#INTERACT#ERR｝
            */
            QHVC_API int SetVideoProfile(VIDEO_PROFILE profile, bool swapWidthAndHeight);

            /**
            * 该方法设置视频编码属性
            *
            * @param width 视频分辨率-宽
            * @param height 视频分辨率-高
            * @param frameRate 视频帧率
            * @param bitrate 视频码率
            * @return 0：方法调用成功，其他表示失败，见｛@link QHVC#INTERACT#ERR｝
            */
            QHVC_API int SetVideoProfileEx(int width, int height, int frameRate, int bitrate);

            /**
            * 暂停/恢复发送本地视频流。该方法用于允许/禁止往网络发送本地视频流。
            * 注： 该方法不影响本地视频流获取，没有禁用摄像头。
            *
            * @param muted True: 不发送本地视频流  False: 发送本地视频流
            * @return 0：方法调用成功，其他表示失败，见｛@link QHVC#INTERACT#ERR｝
            */
            QHVC_API int MuteLocalVideoStream(bool muted);

            /**
            * 暂停/恢复所有人视频流。本方法用于允许/禁止播放所有人的视频流。
            * 注： 该方法不影响视频数据流的接收，只是不播放视频流。
            *
            * @param muted True: 停止播放接收到的所有视频流 False: 允许播放接收到的所有视频流
            * @return 0：方法调用成功，其他表示失败，见｛@link QHVC#INTERACT#ERR｝
            */
            QHVC_API int MuteAllRemoteVideoStreams(bool muted);

            /**
            * 暂停/恢复指定远端视频流。本方法用于允许/禁止播放指定远端视频流。
            * 注： 该方法不影响视频数据流的接收，只是不播放视频流。
            *
            * @param uid   指定用户
            * @param muted True: 停止播放接收到的视频流 False: 允许播放接收到的视频流
            * @return 0：方法调用成功，其他表示失败，见｛@link QHVC#INTERACT#ERR｝
            */
            QHVC_API int MuteRemoteVideoStream(const char* uid, bool muted);

            /**
            * 启用说话者音量提示，该方法允许 SDK 定期向应用程序反馈当前谁在说话以及说话者的音量
            *
            * @param interval 指定音量提示的时间间隔
            *                 <= 0：禁用音量提示功能
            *                 > 0：返回音量提示的间隔，单位为毫秒。建议设置到大于 200 毫秒。
            *                 启用该方法后，无论频道内是否有人说话，都会在说话声音音量提示回调 (onAudioVolumeIndication) 回调中按设置的间隔时间返回音量提示
            * @param smooth 平滑系数，默认可以设置为3
            * @return 0：方法调用成功，其他表示失败，见｛@link QHVC#INTERACT#ERR｝
            */
            QHVC_API int EnableAudioVolumeIndication(int interval, int smooth);

            /**
            * 静音/取消静音。该方法用于允许/禁止往网络发送本地音频流。
            * 注： 该方法不影响录音状态，并没有禁用麦克风。
            *
            * @param muted True：麦克风静音  False：取消静音
            * @return 0：方法调用成功，其他表示失败，见｛@link QHVC#INTERACT#ERR｝
            */
            QHVC_API int MuteLocalAudioStream(bool muted);

            /**
            * 静音所有远端用户/对所有远端用户取消静音。本方法用于允许/禁止播放远端用户的音频流。
            * 注： 该方法不影响音频数据流的接收，只是不播放音频流。
            *
            * @param muted True：麦克风静音  False：取消静音
            * @return 0：方法调用成功，其他表示失败，见｛@link QHVC#INTERACT#ERR｝
            */
            QHVC_API int MuteAllRemoteAudioStreams(bool muted);

            /**
            * 静音指定远端用户/对指定远端用户取消静音。本方法用于允许/禁止播放远端用户的音频流。
            * 注： 该方法不影响音频数据流的接收，只是不播放音频流。
            *
            * @param uid   指定用户
            * @param muted True：麦克风静音 False：取消静音
            * @return 0：方法调用成功，其他表示失败，见｛@link QHVC#INTERACT#ERR｝
            */
            QHVC_API int MuteRemoteAudioStream(const char* uid, bool muted);

#pragma endregion 基本流程

#pragma region 双流相关

            /**
            * 使用双流/单流模式
            *
            * @param enabled 指定双流或者单流模式 true：双流 false：单流（默认）
            * @return 0：方法调用成功，其他表示失败，见｛@link QHVC#INTERACT#ERR｝
            */
            QHVC_API int EnableDualStreamMode(bool enabled);

            /**
            * 设置小流的参数（仅在开启双流模式时有效）
            * 小流分辨率需要和大流的成比例，比如大流 360*640，小流可以设置 180*320，小流不要设置太大。
            * 注： 请确保在joinChannel之前调用。
            *
            * @param width   视频的宽度
            * @param height  视频的宽度
            * @param fps     帧率
            * @param bitrate 码率 (单位：kbps)
            * @return 0：方法调用成功，其他表示失败，见｛@link QHVC#INTERACT#ERR｝
            */
            QHVC_API int SetLowStreamVideoProfile(int width, int height, int fps, int bitrate);

            /**
            * 视频大小流切换, 该方法指定接收远端用户的视频流大小。使用该方法可以根据视频窗口的大小动态调整对应
            * 视频流的大小，以节约带宽和计算资源。本方法调用状态将在下文的SDK 默认收到视频小流，节省带宽。
            * 如需使用视频大流，调用本方法进行切换。
            *
            * @param uid         用户 ID
            * @param streamLevel 设置视频流大小。QHVCInteractConstant.VIDEO_STREAM_LEVEL_HIGH (0): 视频大流
            *                                    QHVCInteractConstant.VIDEO_STREAM_LEVEL_LOW (1): 视频小流
            * @return 0：方法调用成功，其他表示失败，见｛@link QHVC#INTERACT#ERR｝
            */
            QHVC_API int SetRemoteVideoStream(const char* uid, VIDEO_STREAM_LEVEL streamLevel);

#pragma endregion 双流相关

#pragma region 合流相关

            /**
            * N+1路转推（1路合流+N路分流），合流信息预设接口
            * 请确保要在joinChannel之前调用。
            *
            * @param mixStreamConfig 合流参数
            * @param streamLifeCycle 合流任务的生命周期，可以绑定主播或绑定房间
            * @return 0 成功，其他表示失败，见｛@link QHVC#INTERACT#ERR｝
            */
            QHVC_API int SetMixStreamInfo(const MixStreamConfig* mixStreamConfig, STREAM_LIFE_CYCLE streamLifeCycle);

            /**
            * 转推N+1路流时，更新合流的视频布局
            * 注：1. 请确保在IQHVCInteractCallBack.onFirstRemoteVideoFrame()接口中（或之后），调用该方法。
            *     2、count 最大10
            *
            * @param mixStreamInfos 布局信息，请确保设置所有的视频流，包括主播的视频流。
            * @param count mixStreamInfos数组的大小。
            * @return 0:  方法调用成功，其他表示失败，见｛@link QHVC#INTERACT#ERR｝
            */
            QHVC_API int SetVideoCompositingLayout(const MixStreamRegion* mixStreamInfos, int count);

            /**
            * 取消合流布局
            *
            * @return 0:  方法调用成功，其他表示失败，见｛@link QHVC#INTERACT#ERR｝
            */
            QHVC_API int ClearVideoCompositingLayout();

#pragma endregion 合流相关

#pragma region 设备相关

            /**
            * 释放设备列表
            *
            * @param [in] 设备列表
            */
            QHVC_API void FreeDeviceList(__in DeviceInfo* parrDeviceList);

            /**
            * 获取视频设备列表
            *
            * @param [out] 视频设备数量
            * @return 视频设备列表
            */
            QHVC_API DeviceInfo* GetVideoDeviceList(__out int& deviceCount);

            /**
            * 获取当前视频设备
            *
            * @param deviceId 设备ID
            * @param length deviceId长度
            * @return 0 成功，其他参考{@link INTERACT::Err} 定义
            */
            QHVC_API int GetVideoDevice(__out char* deviceId, int length);

            /**
            * 设置当前视频设备
            *
            * @param deviceId 设备ID
            * @return 0 成功，其他参考{@link INTERACT::Err} 定义
            */
            QHVC_API int SetVideoDevice(__in const char* deviceId);

            /**
            * 获取音频设备列表
            *
            * @param deviceType [in] 音频设备类型，输入或输出设备
            * @param deviceCount [out] 音频设备数量
            * @return 音频设备列表
            */
            QHVC_API DeviceInfo* GetAudioDeviceList(__in AUDIO_DEVICE_TYPE deviceType, __out int& deviceCount);

            /**
            * 获取当前音频设备
            *
            * @param deviceType [in] 音频设备类型，输入或输出设备
            * @param deviceId 设备ID
            * @param length deviceId长度
            * @return 0 成功，其他参考{@link INTERACT::Err} 定义
            */
            QHVC_API int GetAudioDevice(__in AUDIO_DEVICE_TYPE deviceType, __out char* deviceId, int length);

            /**
            * 设置当前音频设备
            *
            * @param deviceType [in] 音频设备类型，输入或输出设备
            * @param deviceId 设备ID
            * @return 0 成功，其他参考{@link INTERACT::Err} 定义
            */
            QHVC_API int SetAudioDevice(__in AUDIO_DEVICE_TYPE deviceType, __in const char* deviceId);

            /**
            * 获取当前音频设备音量
            *
            * @param deviceType [in] 音频设备类型，输入或输出设备
            * @param volume [out] 音量，0~255
            * @return 0 成功，其他参考{@link INTERACT::Err} 定义
            */
            QHVC_API int GetAudioDeviceVolume(__in AUDIO_DEVICE_TYPE deviceType, __out int& volume);

            /**
            * 设置当前音频设备音量
            *
            * @param deviceType [in] 音频设备类型，输入或输出设备
            * @param volume [in] 音量，0~255
            * @return 0 成功，其他参考{@link INTERACT::Err} 定义
            */
            QHVC_API int SetAudioDeviceVolume(__in AUDIO_DEVICE_TYPE deviceType, __in int volume);

            /**
            * 获取当前设备是否静音
            *
            * @param deviceType [in] 音频设备类型，输入或输出设备
            * @param mute [out] true 静音；false 非静音
            * @return 0 成功，其他参考{@link INTERACT::Err} 定义
            */
            QHVC_API int GetAudioDeviceMute(__in AUDIO_DEVICE_TYPE deviceType, __out bool& mute);

            /**
            * 设置当前设备是否静音
            *
            * @param deviceType [in] 音频设备类型，输入或输出设备
            * @param mute [in] true 静音；false 非静音
            * @return 0 成功，其他参考{@link INTERACT::Err} 定义
            */
            QHVC_API int SetAudioDeviceMute(__in AUDIO_DEVICE_TYPE deviceType, __in bool mute);
#pragma endregion 设备相关
        }
    }
}

#endif