#ifndef _QHVC_INTERACT_CONSTANT_H_
#define _QHVC_INTERACT_CONSTANT_H_

#pragma once

#define QHVC_INTERACT_DEVICE_INFO_MAX_LEN               (512)

namespace QHVC
{
    namespace INTERACT
    {
        /**
         * 警告
         */
        enum WARN
        {
            WARN_PENDING = 20,
            WARN_NO_AVAILABLE_CHANNEL = 103,
            WARN_LOOKUP_CHANNEL_TIMEOUT = 104,
            WARN_LOOKUP_CHANNEL_REJECTED = 105,
            WARN_OPEN_CHANNEL_TIMEOUT = 106,
            WARN_OPEN_CHANNEL_REJECTED = 107,
            WARN_SWITCH_LIVE_VIDEO_TIMEOUT = 111,
            WARN_SET_CLIENT_ROLE_TIMEOUT = 118,
            WARN_SET_CLIENT_ROLE_NOT_AUTHORIZED = 119,
            WARN_AUDIO_MIXING_OPEN_ERROR = 701,
            WARN_ADM_RUNTIME_PLAYOUT_WARNING = 1014,
            WARN_ADM_RUNTIME_RECORDING_WARNING = 1016,
            WARN_ADM_RECORD_AUDIO_SILENCE = 1019,
            WARN_ADM_PLAYOUT_MALFUNCTION = 1020,
            WARN_ADM_RECORD_MALFUNCTION = 1021,
            WARN_APM_HOWLING = 1051,
        };

        /**
         * 错误
         */
        enum ERR
        {
            ERR_OK = 0,
            ERR_FAILED = 1,
            ERR_INVALID_ARGUMENT = 2,
            ERR_NOT_READY = 3,
            ERR_NOT_SUPPORTED = 4,
            ERR_REFUSED = 5,
            ERR_BUFFER_TOO_SMALL = 6,
            ERR_NOT_INITIALIZED = 7,
            ERR_INVALID_VIEW = 8,
            ERR_NO_PERMISSION = 9,
            ERR_TIMEDOUT = 10,
            ERR_CANCELED = 11,
            ERR_TOO_OFTEN = 12,
            ERR_BIND_SOCKET = 13,
            ERR_NET_DOWN = 14,
            ERR_NET_NOBUFS = 15,
            ERR_INIT_VIDEO = 16,
            ERR_JOIN_CHANNEL_REJECTED = 17,
            ERR_LEAVE_CHANNEL_REJECTED = 18,
            ERR_ALREADY_IN_USE = 19,
            ERR_ABORTED = 20,
            ERR_INIT_NET_ENGINE = 21,
            ERR_RESOURCE_LIMITED = 22,

            ERR_INVALID_APP_ID = 101,
            ERR_INVALID_CHANNEL_NAME = 102,
            ERR_CHANNEL_KEY_EXPIRED = 109,
            ERR_INVALID_CHANNEL_KEY = 110,
            ERR_CONNECTION_INTERRUPTED = 111,
            ERR_CONNECTION_LOST = 112,
            ERR_NOT_IN_CHANNEL = 113,
            ERR_SIZE_TOO_LARGE = 114,
            ERR_BITRATE_LIMIT = 115,
            ERR_TOO_MANY_DATA_STREAMS = 116,
            ERR_DECRYPTION_FAILED = 120,
            ERR_CLIENT_IS_BANNED_BY_SERVER = 123,

            //signaling: 400~600
            ERR_LOGOUT_OTHER = 400,
            ERR_LOGOUT_USER = 401,
            ERR_LOGOUT_NET = 402,
            ERR_LOGOUT_KICKED = 403,
            ERR_LOGOUT_PACKET = 404,
            ERR_LOGOUT_TOKEN_EXPIRED = 405,
            ERR_LOGOUT_OLDVERSION = 406,
            ERR_LOGOUT_TOKEN_WRONG = 407,
            ERR_LOGOUT_ALREADY_LOGOUT = 408,

            ERR_LOGIN_OTHER = 420,
            ERR_LOGIN_NET = 421,
            ERR_LOGIN_FAILED = 422,
            ERR_LOGIN_CANCELED = 423,
            ERR_LOGIN_TOKEN_EXPIRED = 424,
            ERR_LOGIN_OLD_VERSION = 425,
            ERR_LOGIN_TOKEN_WRONG = 426,
            ERR_LOGIN_TOKEN_KICKED = 427,
            ERR_LOGIN_ALREADY_LOGIN = 428,

            ERR_JOIN_CHANNEL_OTHER = 440,

            ERR_SEND_MESSAGE_OTHER = 440,
            ERR_SEND_MESSAGE_TIMEOUT = 441,

            ERR_QUERY_USERNUM_OTHER = 450,
            ERR_QUERY_USERNUM_TIMEOUT = 451,
            ERR_QUERY_USERNUM_BYUSER = 452,

            ERR_LEAVE_CHANNEL_OTHER = 460,
            ERR_LEAVE_CHANNEL_KICKED = 461,
            ERR_LEAVE_CHANNEL_BYUSER = 462,
            ERR_LEAVE_CHANNEL_LOGOUT = 463,
            ERR_LEAVE_CHANNEL_DISCONNECTED = 464,

            ERR_INVITE_OTHER = 470,
            ERR_INVITE_REINVITE = 471,
            ERR_INVITE_NET = 472,
            ERR_INVITE_PEER_OFFLINE = 473,
            ERR_INVITE_TIMEOUT = 474,
            ERR_INVITE_CANT_RECV = 475,

            ERR_LOAD_MEDIA_ENGINE = 1001,
            ERR_START_CALL = 1002,
            ERR_START_CAMERA = 1003,
            ERR_START_VIDEO_RENDER = 1004,
            ERR_ADM_GENERAL_ERROR = 1005,
            ERR_ADM_JAVA_RESOURCE = 1006,
            ERR_ADM_SAMPLE_RATE = 1007,
            ERR_ADM_INIT_PLAYOUT = 1008,
            ERR_ADM_START_PLAYOUT = 1009,
            ERR_ADM_STOP_PLAYOUT = 1010,
            ERR_ADM_INIT_RECORDING = 1011,
            ERR_ADM_START_RECORDING = 1012,
            ERR_ADM_STOP_RECORDING = 1013,
            ERR_ADM_RUNTIME_PLAYOUT_ERROR = 1015,
            ERR_ADM_RUNTIME_RECORDING_ERROR = 1017,
            ERR_ADM_RECORD_AUDIO_FAILED = 1018,
            ERR_ADM_INIT_LOOPBACK = 1022,
            ERR_ADM_START_LOOPBACK = 1023,
            ERR_ADM_NO_PERMISSION = 1027,
            ERR_ADM_RECORD_AUDIO_IS_ACTIVE = 1033,
            ERR_ADM_WIN_CORE_INIT = 1301,
            ERR_ADM_WIN_CORE_INIT_RECORDING = 1303,
            ERR_ADM_WIN_CORE_INIT_PLAYOUT = 1306,
            ERR_ADM_WIN_CORE_INIT_PLAYOUT_NULL = 1307,
            ERR_ADM_WIN_CORE_START_RECORDING = 1309,
            ERR_ADM_WIN_CORE_CREATE_REC_THREAD = 1311,
            ERR_ADM_WIN_CORE_CAPTURE_NOT_STARTUP = 1314,
            ERR_ADM_WIN_CORE_CREATE_RENDER_THREAD = 1319,
            ERR_ADM_WIN_CORE_RENDER_NOT_STARTUP = 1320,
            ERR_ADM_WIN_CORE_NO_RECORDING_DEVICE = 1322,
            ERR_ADM_WIN_CORE_NO_PLAYOUT_DEVICE = 1323,
            ERR_ADM_WIN_WAVE_INIT = 1351,
            ERR_ADM_WIN_WAVE_INIT_RECORDING = 1353,
            ERR_ADM_WIN_WAVE_INIT_MICROPHONE = 1354,
            ERR_ADM_WIN_WAVE_INIT_PLAYOUT = 1355,
            ERR_ADM_WIN_WAVE_INIT_SPEAKER = 1356,
            ERR_ADM_WIN_WAVE_START_RECORDING = 1357,
            ERR_ADM_WIN_WAVE_START_PLAYOUT = 1358,
            ERR_ADM_NO_RECORDING_DEVICE = 1359,
            ERR_ADM_NO_PLAYOUT_DEVICE = 1360,

            ERR_VDM_CAMERA_NOT_AUTHORIZED = 1501,

            ERR_VCM_UNKNOWN_ERROR = 1600,
            ERR_VCM_ENCODER_INIT_ERROR = 1601,
            ERR_VCM_ENCODER_ENCODE_ERROR = 1602,
            ERR_VCM_ENCODER_SET_ERROR = 1603,

            ERR_NO_USE_EXTERNAL_RENDER = -101, //未设置外部渲染视频
            ERR_NO_SET_EXTERNAL_RENDER_CALLBACK = -102, //未设置外部渲染callback（QHExternalRenderCallback）

            ERR_MIXSTREAM_INPUT_NOT_EXIST = -150, //混流的输入流不存在
            ERR_MIXSTREAM_FAILED = -151, //混流失败
            ERR_MIXSTREAM_STOP_FAILED = -152, //停止混流失败
            ERR_MIXSTREAM_INPUT_PARAMETER_ERR = -153, //输入参数错误
            ERR_MIXSTREAM_OUTPUT_PARAMETER_ERR = -154, //输出参数错误
            ERR_MIXSTREAM_INPUT_RATIO_ERR = -155, //输入分辨率格式错误
            ERR_MIXSTREAM_OUTPUT_RATIO_ERR = -156, //输出分辨率格式错误
            ERR_MIXSTREAM_NO_OPEN = -157, //混流没开

            //zego error code
            ERR_TEMP_BROKEN = -200, //直播临时中断，已废弃
            ERR_FATAL_ERR = -201, //直播遇到严重的问题（如出现，请联系 ZEGO 技术支持）
            ERR_CREATESTREAM_ERROR = -202, //创建直播流失败
            ERR_FETCH_STREAM_ERROR = -203, //获取流信息失败
            ERR_NOSTREAM_ERROR = -204, //无流信息
            ERR_LOGIC_SERVER_NETWORK_ERROR = -205, //逻辑服务器网络错误
            ERR_DNS_RESOLVE_ERROR = -206, //DNS 解析失败
            ERR_NOT_LONGIN_ERROR = -207, //未登录
            ERR_UNKNOWN_ERROR = -208, //未知错误
            ERR_PUBLISH_BAD_NAME_ERROR = -209, //
            ERR_HTTP_DNS_RESOLVE_ERROR = -210, // HTTP DNS 解析错误

            ERROR_ZEGOAVKIT_IS_NULL = -601, //即构引擎为空
            ERROR_UID_IS_NULL = -602, //uid为空
            ERROR_STREADID_IS_NULL = -603, //流ID为空
            ERROR_REPEAT_SETUPREMOTEVIDEO = -604, //重复调用了setupRemoteVideo
            ERROR_REPEAT_REMOVEREMOTEVIDEO = -605, //重复调用了removeRemoteVideo或还未调用过setupRemoteVideo

            ERROR_INTERACT_ENGINE_NULL = -700, //互动引擎为空
            ERROR_SET_CLIENT_ROLE_FAILED = -701, //SetClientRole失败



            ERROR_SERVER_PARAM_IS_NEEDED = 4001, //参数缺失
            ERROR_SERVER_INTERNAL_ERROR = 4101, //服务内部异常
            ERROR_SERVER_REQUEST_URL_IS_EXPIRED = 4102, //请求已过期
            ERROR_SERVER_UNAUTHORIZED_TOKEN_MISS_MATCH = 4103, //签名验证失败
            ERROR_SERVER_SNAME_NOT_FOUND = 4201, //sname 不存在
            ERROR_SERVER_SNAME_NOT_SUPPORTED = 4202, //sname 未开通服务
            ERROR_SERVER_CREATE_ROOM_FAILED = 4301, //创建房间失败
            ERROR_SERVER_JOIN_ROOM_FAILED = 4302, // 加入房间失败
            ERROR_SERVER_QUIT_ROOM_FAILED = 4303, // 退出房间失败
            ERROR_SERVER_GET_ROOM_INFO_FAILED = 4304, // 获取房间信息失败
            ERROR_SERVER_UPDATE_MEMBER_ATTRIBUTE_FAILED = 4305, //更新成员属性失败
            ERROR_SERVER_UPDATE_ROOM_ATTRIBUTE_FAILED = 4306, //更新房间属性失败
            ERROR_SERVER_UPDATE_HEARTBEAT_FAILED = 4307, // 心跳更新失败
            ERROR_SERVER_ROOM_ALREADY_EXISTS = 4308, // 房间已存在
            ERROR_SERVER_INVALID_SNAME = 4309, // sname 不可用
            ERROR_SERVER_GENERATE_PUB_CTX_FAILED = 4310, // 生成连麦信息失败
            ERROR_SERVER_NO_AVAILABLE_RTC_SETTING = 4311, //没有可用的连麦配置
        };

        /**
         * 错误类型
         */
        enum ERR_TYPE
        {
            ERR_TYPE_JOIN_ERR = 3601, //加入频道错误
            ERR_TYPE_PUBLISH_ERR = 3602, //连麦中错误
            ERR_TYPE_LOADENGINE_ERROR = 3603, //加载引擎错误
        };

        /**
         * 视频质量
         * resolution fps rage(kpbs)
         */
        enum VIDEO_PROFILE
        {
            VIDEO_PROFILE_240P_3 = 22, // 240x240   15   140
            VIDEO_PROFILE_240P_4 = 23, // 424x240   15   220
            VIDEO_PROFILE_360P_3 = 32, // 360x360   15   260
            VIDEO_PROFILE_360P_6 = 35, // 360x360   30   400
            VIDEO_PROFILE_360P   = 30, // 640x360   15   400
            VIDEO_PROFILE_360P_4 = 33, // 640x360   30   600 （声网） // 640x360   15   600 （即构）
            VIDEO_PROFILE_360P_9 = 38, // 640x360   15   800
            VIDEO_PROFILE_480P_3 = 42, // 480x480   15   400
            VIDEO_PROFILE_480P_6 = 45, // 480x480   30   600
            VIDEO_PROFILE_480P_8 = 47, // 848x480   15   610
            VIDEO_PROFILE_480P_9 = 48, // 848x480   30   930
            VIDEO_PROFILE_720P   = 50, // 1280x720  15   1130
            VIDEO_PROFILE_720P_3 = 52, // 1280x720  30   1710
        };

        /**
         * 视频帧颜色编码格式
         */
        enum VIDEO_FRAME_FORMAT
        {
            VIDEO_FRAME_FORMAT_I420 = 1,
            VIDEO_FRAME_FORMAT_ARGB = 2,
            VIDEO_FRAME_FORMAT_NV21 = 3,
            VIDEO_FRAME_FORMAT_RGBA = 4,
        };

        /**
         * 视频流等级
         */
        enum VIDEO_STREAM_LEVEL
        {
            VIDEO_STREAM_LEVEL_HIGH = 0, //大流(高带宽，高成本)
            VIDEO_STREAM_LEVEL_LOW = 1, //小流(低带宽，低成本)
            VIDEO_STREAM_LEVEL_MEDIUM = 2, //中等
        };

        /**
         * 视频解码方式
         */
        enum VIDEO_ENCODER
        {
            VIDEO_ENCODER_HARDWARE = 0, //硬解
            VIDEO_ENCODER_SOFTWARE = 1, //软解
        };

        /**
        * 音频质量参数：采样率、编码方式、声道数、编码码率
        */
        enum AUDIO_PROFILE
        {
            AUDIO_PROFILE_DEFAULT = 0,                  //默认设置：通信场景下为 1，直播场景下为 2
            AUDIO_PROFILE_SPEECH_STANDARD = 1,          //32 KHz采样率，语音编码, 单声道，编码码率约 18 kbps
            AUDIO_PROFILE_MUSIC_STANDARD = 2,           //48 KHz采样率，音乐编码, 单声道，编码码率约 48 kbps
            AUDIO_PROFILE_MUSIC_STANDARD_STEREO = 3,    //48 KHz采样率，音乐编码, 双声道，编码码率约 56 kbps
            AUDIO_PROFILE_MUSIC_HIGH_QUALITY = 4,       //48 KHz采样率，音乐编码, 单声道，编码码率约 128 kbps
            AUDIO_PROFILE_MUSIC_HIGH_QUALITY_STEREO = 5,//48 KHz采样率，音乐编码, 双声道，编码码率约 192 kbps
        };

        /**
        * 音频应用场景
        */
        enum AUDIO_SCENARIO {
            AUDIO_SCENARIO_DEFAULT = 0,                 //默认设置
            AUDIO_SCENARIO_CHATROOM_ENTERTAINMENT = 1,  //娱乐应用，需要频繁上下麦的场景
            AUDIO_SCENARIO_EDUCATION = 2,               //教育应用，流畅度和稳定性优先
            AUDIO_SCENARIO_GAME_STREAMING = 3,          //游戏直播应用，需要外放游戏音效也直播出去的场景
            AUDIO_SCENARIO_SHOWROOM = 4,                //秀场应用，音质优先和更好的专业外设支持
            AUDIO_SCENARIO_CHATROOM_GAMING = 5,         //游戏开黑
        };

        /**
        * 频道模式
        */
        enum CHANNEL_PROFILE
        {
            CHANNEL_PROFILE_COMMUNICATION = 0, //网络电话模式
            CHANNEL_PROFILE_LIVE_BROADCASTING = 1, //直播模式
            CHANNEL_PROFILE_GAME = 2, //游戏语音
        };

        /**
         * 流类型
         */
        namespace STREAM_TYPE
        {
            const std::string STREAM_TYPE_ALL = "all"; //音视频
            const std::string STREAM_TYPE_AUDIO = "audio"; //仅音频
            const std::string STREAM_TYPE_VIDEO = "video"; //仅视频
        }

        /**
         * 推流模式
         */
        enum STREAM_MODE
        {
            STREAM_MODE_SEPARATE = 1, //分流模式
            STREAM_MODE_COMBINE = 2, //合流模式
        };

        /**
         * 流文件格式
         */
        namespace STREAM_FILE_TYPE
        {
            const std::string STREAM_FILE_TYPE_RTMP = "rtmp";
            const std::string STREAM_FILE_TYPE_FLV = "flv";
            const std::string STREAM_FILE_TYPE_M3U8 = "m3u8";
        }

        /**
         * 流编码格式
         */
        namespace STREAM_ENCODE_TYPE
        {
            const std::string STREAM_ENCODE_TYPE_H264 = "h264";
            const std::string STREAM_ENCODE_TYPE_H265 = "h265";
        }

        /**
         * 通信质量
         */
        enum QUALITY
        {
            QUALITY_UNKOWN = 0,
            QUALITY_EXCELLENT,
            QUALITY_GOOD,
            QUALITY_POOR,
            QUALITY_BAD,
            QUALITY_VBAD,
            QUALITY_DOWN,
            QUALITY_UNSUPPORTED,
        };

        /**
        * RTC通信质量
        */
        enum RTC_NETWORK_QUALITY
        {
            RTC_NETWORK_QUALITY_UNKOWN = 0,
            RTC_NETWORK_QUALITY_EXCELLENT,
            RTC_NETWORK_QUALITY_GOOD,
            RTC_NETWORK_QUALITY_POOR,
            RTC_NETWORK_QUALITY_BAD,
            RTC_NETWORK_QUALITY_VBAD,
            RTC_NETWORK_QUALITY_DOWN,
        };

        /**
         * 通信质量报告格式
         */
        enum QUALITY_REPORT_FORMAT
        {
            QUALITY_REPORT_FORMAT_JSON = 0,
            QUALITY_REPORT_FORMAT_HTML = 1,
        };

        /**
         * 连麦能力
         */
        namespace ISP
        {
            const std::string ISP_360LIVECLOUD = "Qlive";
            const std::string ISP_AGORA = "agora";
            const std::string ISP_ZEGO = "zego";
        }

        /**
         * 连麦调度结果
         */
        enum SCHEDULE_RESULT
        {
            SCHEDULE_RESULT_OK = 0, //调度成功
            SCHEDULE_RESULT_FAILED = -1, //调度失败
            SCHEDULE_RESULT_NEED_UPDATEVERSION = 417, //调度失败，当前版本不具备此连麦能力，建议业务更新到最新版本
        };

        /**
         * 用户角色
         */
        enum CLIENT_ROLE
        {
            CLIENT_ROLE_BROADCASTER = 1, //主播
            CLIENT_ROLE_AUDIENCE = 2, //观众
            //CLIENT_ROLE_GUEST = 8, //嘉宾
        };

        /**
         * 用户角色
         */
        namespace CLIENT_ROLE_TEXT
        {
            const std::string CLIENT_ROLE_TEXT_BROADCASTER = "anchor"; //主播
            const std::string CLIENT_ROLE_TEXT_AUDIENCE = "audience"; //观众
            const std::string CLIENT_ROLE_TEXT_GUEST = "guest"; //嘉宾
        }

        /**
         * 用户离开原因
         */
        enum USER_OFFLINE_REASON
        {
            USER_OFFLINE_REASON_QUIT = 0, //用户主动离开
            USER_OFFLINE_REASON_DROPPED = 1, //因过长时间收不到对方数据包，超时掉线
            USER_OFFLINE_REASON_BECOME_AUDIENCE, //由嘉宾变程观众
        };

        /**
         * 视频渲染模式
         */
        enum RENDER_MODE
        {
            RENDER_MODE_HIDDEN = 1, //如果视频尺寸与显示视窗尺寸不一致，则视频流会按照显示视窗的比例进行周边裁剪或图像拉伸后填满视窗
            RENDER_MODE_FIT = 2, //如果视频尺寸与显示视窗尺寸不一致，在保持长宽比的前提下，将视频进行缩放后填满视窗
            RENDER_MODE_ADAPTIVE = 3, //如果自己和对方都是竖屏，或者如果自己和对方都是横屏，使用RENDER_MODE_HIDDEN；如果对方和自己一个竖屏一个横屏，则使用 RENDER_MODE_FIT
        };

        enum VIDEO_MIRROR_MODE
        {
            VIDEO_MIRROR_MODE_AUTO = 0,//由 SDK 决定镜像模式
            VIDEO_MIRROR_MODE_ENABLED = 1,//启用镜像模式
            VIDEO_MIRROR_MODE_DISABLED = 2,//关闭镜像模式
        };

        /**
        * 音频设备类型
        */
        enum AUDIO_DEVICE_TYPE
        {
            AUDIO_DEVICE_INPUT = 0,     //输入设备
            AUDIO_DEVICE_OUTPUT,        //输出设备
        };

        /**
        * 设备信息
        */
        typedef struct DeviceInfo
        {
            char szDeviceId[QHVC_INTERACT_DEVICE_INFO_MAX_LEN];     //设备ID
            char szDeviceName[QHVC_INTERACT_DEVICE_INFO_MAX_LEN];   //设备名称
        } DeviceInfo;

        /**
        * 设备状态
        */
        enum DEVICE_STATE
        {
            DEVICE_STATE_ACTIVE = 1,        //设备可用
            DEVICE_STATE_DISABLED = 2,      //设备被禁用
            DEVICE_STATE_NOT_PRESENT = 4,   //设备不存在
            DEVICE_STATE_UNPLUGGED = 8      //设备被拔出
        };

        /**
        * 转推N+1路流时，合流的布局
        */
        struct MixStreamRegion {
            char userID[256]; //用户唯一ID
            int x; //该流相对于画布的x坐标
            int y; //该流相对于画布的y坐标
            int width; //展示宽度
            int height; //展示高度
            int zOrder; //0表示该区域图像位于最下层，100表示该区域图像位于最上层
            int alpha; //[0, 1.0] 0表示图像为透明的， 1表示图像为完全不透明
            int renderMode;  //渲染填充模式, 默认RenderMode.RENDER_MODE_HIDDEN，具体参见{@link RenderMode#RENDER_MODE_HIDDEN}等

            MixStreamRegion() {
                memset(userID, 0, 256);
                x = 0;
                y = 0;
                width = 0;
                height = 0;
                zOrder = 0;
                alpha = 1;
                renderMode = RENDER_MODE_HIDDEN;
            }
        };

        typedef struct MixStreamConfig {

            char mixStreamAddr[1024]; //合流视频推流地址
            int canvasWidth; //画布宽度（即合流的宽度）
            int canvasHeight; //画布高度（即合流的高度）
            int videoFps; //合流的帧率
            int videoBitrate; //编码帧率（单位：kbps）
            int mIframeInterval; // n seconds between I-frames 单位：秒）
            char backgroundColor[10]; //背景颜色 eg: "#c0c0c0"
            //音频参数
            char audioFormat[10]; // 音频格式
            int audioSampleRate; // 音频采样率
            int audioBitrate; // 音频目标码率
            int audioNumsChannel; // 音频声道数

            MixStreamConfig() {
                memset(mixStreamAddr, 0, 1024);
                canvasWidth = 0;
                canvasHeight = 0;
                videoFps = 15;
                videoBitrate = 600;
                mIframeInterval = 2;
                memset(backgroundColor, 0, 10);
                strcpy_s(backgroundColor, 9, "#000000");
                //音频参数
                memset(audioFormat, 0, 10);
                strcpy_s(audioFormat, 9, "aac");
                audioSampleRate = 44100;
                audioBitrate = 48;
                audioNumsChannel = 1;

            }

        } MixStreamConfig;


        enum STREAM_LIFE_CYCLE {
            STREAM_LIFE_CYCLE_BIND_USER, //绑定主播
            STREAM_LIFE_CYCLE_BIND_ROOM //绑定房间
        };

        /**
        * 地址
        */
        namespace ENGINE_OPTION
        {
            const std::string PUSH_ADDR = "push_addr";
            const std::string PULL_ADDR = "pull_addr";
        }


        enum VIDEO_FRAME_TYPE {
            FRAME_TYPE_YUV420 = 0,  //YUV 420 format
        };

        /**
        * 视频帧数据格式
        */
        struct VideoFrame {
            VIDEO_FRAME_TYPE type; // 目前只能是YUV 420
            int width;  //视频像素宽度
            int height;  //视频像素高度
            int yStride;  //YUV 数据中的 Y 缓冲区的行跨度
            int uStride;  //YUV 数据中的 U 缓冲区的行跨度
            int vStride;  //YUV 数据中的 V 缓冲区的行跨度
            void* yBuffer;  //指向 YU V数据中的 Y 缓冲区指针的指针
            void* uBuffer;  //指向 YU V数据中的 U 缓冲区指针的指针
            void* vBuffer;  //指向 YU V数据中的 V 缓冲区指针的指针
            int rotation; // 在渲染视频前设置该帧的顺时针旋转角度，目前支持 0 度，90 度，180 度，和 270 度。
            long long renderTimeMs; //  渲染视频流的时间戳。用户在渲染视频流时需使用该时间戳同步视频流的渲染。该时间戳为渲染视频流的时间戳，不是采集视频流的时间戳。
        };

        enum AUDIO_FRAME_TYPE {
            FRAME_TYPE_PCM16 = 0,  //PCM 16bit little endian
        };

        /**
        * 音频帧数据格式
        */
        struct AudioFrame {
            AUDIO_FRAME_TYPE type;// 目前只能是PCM 16bit 
            int samples;  //该帧的样本数量
            int bytesPerSample;  //每个样本的字节数: PCM (16位)含两个字节
            int channels;  //频道数量(如果是立体声，数据是交叉的)
            int samplesPerSec;  //采样率
            void* buffer;  //数据缓冲区
            long long  renderTimeMs; // 渲染音频流的时间戳。用户在进行音频流渲染时使用该时间戳同步音频流的渲染。该时间戳为渲染音频流的时间戳，不是采集音频流的时间戳。
        };
    }
}

#endif