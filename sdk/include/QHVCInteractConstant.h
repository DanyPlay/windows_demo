#ifndef _QHVC_INTERACT_CONSTANT_H_
#define _QHVC_INTERACT_CONSTANT_H_

#pragma once

#define QHVC_INTERACT_DEVICE_INFO_MAX_LEN               (512)

namespace QHVC
{
    namespace INTERACT
    {
        /**
         * ����
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
         * ����
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

            ERR_NO_USE_EXTERNAL_RENDER = -101, //δ�����ⲿ��Ⱦ��Ƶ
            ERR_NO_SET_EXTERNAL_RENDER_CALLBACK = -102, //δ�����ⲿ��Ⱦcallback��QHExternalRenderCallback��

            ERR_MIXSTREAM_INPUT_NOT_EXIST = -150, //������������������
            ERR_MIXSTREAM_FAILED = -151, //����ʧ��
            ERR_MIXSTREAM_STOP_FAILED = -152, //ֹͣ����ʧ��
            ERR_MIXSTREAM_INPUT_PARAMETER_ERR = -153, //�����������
            ERR_MIXSTREAM_OUTPUT_PARAMETER_ERR = -154, //�����������
            ERR_MIXSTREAM_INPUT_RATIO_ERR = -155, //����ֱ��ʸ�ʽ����
            ERR_MIXSTREAM_OUTPUT_RATIO_ERR = -156, //����ֱ��ʸ�ʽ����
            ERR_MIXSTREAM_NO_OPEN = -157, //����û��

            //zego error code
            ERR_TEMP_BROKEN = -200, //ֱ����ʱ�жϣ��ѷ���
            ERR_FATAL_ERR = -201, //ֱ���������ص����⣨����֣�����ϵ ZEGO ����֧�֣�
            ERR_CREATESTREAM_ERROR = -202, //����ֱ����ʧ��
            ERR_FETCH_STREAM_ERROR = -203, //��ȡ����Ϣʧ��
            ERR_NOSTREAM_ERROR = -204, //������Ϣ
            ERR_LOGIC_SERVER_NETWORK_ERROR = -205, //�߼��������������
            ERR_DNS_RESOLVE_ERROR = -206, //DNS ����ʧ��
            ERR_NOT_LONGIN_ERROR = -207, //δ��¼
            ERR_UNKNOWN_ERROR = -208, //δ֪����
            ERR_PUBLISH_BAD_NAME_ERROR = -209, //
            ERR_HTTP_DNS_RESOLVE_ERROR = -210, // HTTP DNS ��������

            ERROR_ZEGOAVKIT_IS_NULL = -601, //��������Ϊ��
            ERROR_UID_IS_NULL = -602, //uidΪ��
            ERROR_STREADID_IS_NULL = -603, //��IDΪ��
            ERROR_REPEAT_SETUPREMOTEVIDEO = -604, //�ظ�������setupRemoteVideo
            ERROR_REPEAT_REMOVEREMOTEVIDEO = -605, //�ظ�������removeRemoteVideo��δ���ù�setupRemoteVideo

            ERROR_INTERACT_ENGINE_NULL = -700, //��������Ϊ��
            ERROR_SET_CLIENT_ROLE_FAILED = -701, //SetClientRoleʧ��



            ERROR_SERVER_PARAM_IS_NEEDED = 4001, //����ȱʧ
            ERROR_SERVER_INTERNAL_ERROR = 4101, //�����ڲ��쳣
            ERROR_SERVER_REQUEST_URL_IS_EXPIRED = 4102, //�����ѹ���
            ERROR_SERVER_UNAUTHORIZED_TOKEN_MISS_MATCH = 4103, //ǩ����֤ʧ��
            ERROR_SERVER_SNAME_NOT_FOUND = 4201, //sname ������
            ERROR_SERVER_SNAME_NOT_SUPPORTED = 4202, //sname δ��ͨ����
            ERROR_SERVER_CREATE_ROOM_FAILED = 4301, //��������ʧ��
            ERROR_SERVER_JOIN_ROOM_FAILED = 4302, // ���뷿��ʧ��
            ERROR_SERVER_QUIT_ROOM_FAILED = 4303, // �˳�����ʧ��
            ERROR_SERVER_GET_ROOM_INFO_FAILED = 4304, // ��ȡ������Ϣʧ��
            ERROR_SERVER_UPDATE_MEMBER_ATTRIBUTE_FAILED = 4305, //���³�Ա����ʧ��
            ERROR_SERVER_UPDATE_ROOM_ATTRIBUTE_FAILED = 4306, //���·�������ʧ��
            ERROR_SERVER_UPDATE_HEARTBEAT_FAILED = 4307, // ��������ʧ��
            ERROR_SERVER_ROOM_ALREADY_EXISTS = 4308, // �����Ѵ���
            ERROR_SERVER_INVALID_SNAME = 4309, // sname ������
            ERROR_SERVER_GENERATE_PUB_CTX_FAILED = 4310, // ����������Ϣʧ��
            ERROR_SERVER_NO_AVAILABLE_RTC_SETTING = 4311, //û�п��õ���������
        };

        /**
         * ��������
         */
        enum ERR_TYPE
        {
            ERR_TYPE_JOIN_ERR = 3601, //����Ƶ������
            ERR_TYPE_PUBLISH_ERR = 3602, //�����д���
            ERR_TYPE_LOADENGINE_ERROR = 3603, //�����������
        };

        /**
         * ��Ƶ����
         * resolution fps rage(kpbs)
         */
        enum VIDEO_PROFILE
        {
            VIDEO_PROFILE_240P_3 = 22, // 240x240   15   140
            VIDEO_PROFILE_240P_4 = 23, // 424x240   15   220
            VIDEO_PROFILE_360P_3 = 32, // 360x360   15   260
            VIDEO_PROFILE_360P_6 = 35, // 360x360   30   400
            VIDEO_PROFILE_360P   = 30, // 640x360   15   400
            VIDEO_PROFILE_360P_4 = 33, // 640x360   30   600 �������� // 640x360   15   600 ��������
            VIDEO_PROFILE_360P_9 = 38, // 640x360   15   800
            VIDEO_PROFILE_480P_3 = 42, // 480x480   15   400
            VIDEO_PROFILE_480P_6 = 45, // 480x480   30   600
            VIDEO_PROFILE_480P_8 = 47, // 848x480   15   610
            VIDEO_PROFILE_480P_9 = 48, // 848x480   30   930
            VIDEO_PROFILE_720P   = 50, // 1280x720  15   1130
            VIDEO_PROFILE_720P_3 = 52, // 1280x720  30   1710
        };

        /**
         * ��Ƶ֡��ɫ�����ʽ
         */
        enum VIDEO_FRAME_FORMAT
        {
            VIDEO_FRAME_FORMAT_I420 = 1,
            VIDEO_FRAME_FORMAT_ARGB = 2,
            VIDEO_FRAME_FORMAT_NV21 = 3,
            VIDEO_FRAME_FORMAT_RGBA = 4,
        };

        /**
         * ��Ƶ���ȼ�
         */
        enum VIDEO_STREAM_LEVEL
        {
            VIDEO_STREAM_LEVEL_HIGH = 0, //����(�ߴ����߳ɱ�)
            VIDEO_STREAM_LEVEL_LOW = 1, //С��(�ʹ����ͳɱ�)
            VIDEO_STREAM_LEVEL_MEDIUM = 2, //�е�
        };

        /**
         * ��Ƶ���뷽ʽ
         */
        enum VIDEO_ENCODER
        {
            VIDEO_ENCODER_HARDWARE = 0, //Ӳ��
            VIDEO_ENCODER_SOFTWARE = 1, //���
        };

        /**
        * ��Ƶ���������������ʡ����뷽ʽ������������������
        */
        enum AUDIO_PROFILE
        {
            AUDIO_PROFILE_DEFAULT = 0,                  //Ĭ�����ã�ͨ�ų�����Ϊ 1��ֱ��������Ϊ 2
            AUDIO_PROFILE_SPEECH_STANDARD = 1,          //32 KHz�����ʣ���������, ����������������Լ 18 kbps
            AUDIO_PROFILE_MUSIC_STANDARD = 2,           //48 KHz�����ʣ����ֱ���, ����������������Լ 48 kbps
            AUDIO_PROFILE_MUSIC_STANDARD_STEREO = 3,    //48 KHz�����ʣ����ֱ���, ˫��������������Լ 56 kbps
            AUDIO_PROFILE_MUSIC_HIGH_QUALITY = 4,       //48 KHz�����ʣ����ֱ���, ����������������Լ 128 kbps
            AUDIO_PROFILE_MUSIC_HIGH_QUALITY_STEREO = 5,//48 KHz�����ʣ����ֱ���, ˫��������������Լ 192 kbps
        };

        /**
        * ��ƵӦ�ó���
        */
        enum AUDIO_SCENARIO {
            AUDIO_SCENARIO_DEFAULT = 0,                 //Ĭ������
            AUDIO_SCENARIO_CHATROOM_ENTERTAINMENT = 1,  //����Ӧ�ã���ҪƵ��������ĳ���
            AUDIO_SCENARIO_EDUCATION = 2,               //����Ӧ�ã������Ⱥ��ȶ�������
            AUDIO_SCENARIO_GAME_STREAMING = 3,          //��Ϸֱ��Ӧ�ã���Ҫ�����Ϸ��ЧҲֱ����ȥ�ĳ���
            AUDIO_SCENARIO_SHOWROOM = 4,                //�㳡Ӧ�ã��������Ⱥ͸��õ�רҵ����֧��
            AUDIO_SCENARIO_CHATROOM_GAMING = 5,         //��Ϸ����
        };

        /**
        * Ƶ��ģʽ
        */
        enum CHANNEL_PROFILE
        {
            CHANNEL_PROFILE_COMMUNICATION = 0, //����绰ģʽ
            CHANNEL_PROFILE_LIVE_BROADCASTING = 1, //ֱ��ģʽ
            CHANNEL_PROFILE_GAME = 2, //��Ϸ����
        };

        /**
         * ������
         */
        namespace STREAM_TYPE
        {
            const std::string STREAM_TYPE_ALL = "all"; //����Ƶ
            const std::string STREAM_TYPE_AUDIO = "audio"; //����Ƶ
            const std::string STREAM_TYPE_VIDEO = "video"; //����Ƶ
        }

        /**
         * ����ģʽ
         */
        enum STREAM_MODE
        {
            STREAM_MODE_SEPARATE = 1, //����ģʽ
            STREAM_MODE_COMBINE = 2, //����ģʽ
        };

        /**
         * ���ļ���ʽ
         */
        namespace STREAM_FILE_TYPE
        {
            const std::string STREAM_FILE_TYPE_RTMP = "rtmp";
            const std::string STREAM_FILE_TYPE_FLV = "flv";
            const std::string STREAM_FILE_TYPE_M3U8 = "m3u8";
        }

        /**
         * �������ʽ
         */
        namespace STREAM_ENCODE_TYPE
        {
            const std::string STREAM_ENCODE_TYPE_H264 = "h264";
            const std::string STREAM_ENCODE_TYPE_H265 = "h265";
        }

        /**
         * ͨ������
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
        * RTCͨ������
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
         * ͨ�����������ʽ
         */
        enum QUALITY_REPORT_FORMAT
        {
            QUALITY_REPORT_FORMAT_JSON = 0,
            QUALITY_REPORT_FORMAT_HTML = 1,
        };

        /**
         * ��������
         */
        namespace ISP
        {
            const std::string ISP_360LIVECLOUD = "Qlive";
            const std::string ISP_AGORA = "agora";
            const std::string ISP_ZEGO = "zego";
        }

        /**
         * ������Ƚ��
         */
        enum SCHEDULE_RESULT
        {
            SCHEDULE_RESULT_OK = 0, //���ȳɹ�
            SCHEDULE_RESULT_FAILED = -1, //����ʧ��
            SCHEDULE_RESULT_NEED_UPDATEVERSION = 417, //����ʧ�ܣ���ǰ�汾���߱�����������������ҵ����µ����°汾
        };

        /**
         * �û���ɫ
         */
        enum CLIENT_ROLE
        {
            CLIENT_ROLE_BROADCASTER = 1, //����
            CLIENT_ROLE_AUDIENCE = 2, //����
            //CLIENT_ROLE_GUEST = 8, //�α�
        };

        /**
         * �û���ɫ
         */
        namespace CLIENT_ROLE_TEXT
        {
            const std::string CLIENT_ROLE_TEXT_BROADCASTER = "anchor"; //����
            const std::string CLIENT_ROLE_TEXT_AUDIENCE = "audience"; //����
            const std::string CLIENT_ROLE_TEXT_GUEST = "guest"; //�α�
        }

        /**
         * �û��뿪ԭ��
         */
        enum USER_OFFLINE_REASON
        {
            USER_OFFLINE_REASON_QUIT = 0, //�û������뿪
            USER_OFFLINE_REASON_DROPPED = 1, //�����ʱ���ղ����Է����ݰ�����ʱ����
            USER_OFFLINE_REASON_BECOME_AUDIENCE, //�ɼα���̹���
        };

        /**
         * ��Ƶ��Ⱦģʽ
         */
        enum RENDER_MODE
        {
            RENDER_MODE_HIDDEN = 1, //�����Ƶ�ߴ�����ʾ�Ӵ��ߴ粻һ�£�����Ƶ���ᰴ����ʾ�Ӵ��ı��������ܱ߲ü���ͼ������������Ӵ�
            RENDER_MODE_FIT = 2, //�����Ƶ�ߴ�����ʾ�Ӵ��ߴ粻һ�£��ڱ��ֳ���ȵ�ǰ���£�����Ƶ�������ź������Ӵ�
            RENDER_MODE_ADAPTIVE = 3, //����Լ��ͶԷ�������������������Լ��ͶԷ����Ǻ�����ʹ��RENDER_MODE_HIDDEN������Է����Լ�һ������һ����������ʹ�� RENDER_MODE_FIT
        };

        enum VIDEO_MIRROR_MODE
        {
            VIDEO_MIRROR_MODE_AUTO = 0,//�� SDK ��������ģʽ
            VIDEO_MIRROR_MODE_ENABLED = 1,//���þ���ģʽ
            VIDEO_MIRROR_MODE_DISABLED = 2,//�رվ���ģʽ
        };

        /**
        * ��Ƶ�豸����
        */
        enum AUDIO_DEVICE_TYPE
        {
            AUDIO_DEVICE_INPUT = 0,     //�����豸
            AUDIO_DEVICE_OUTPUT,        //����豸
        };

        /**
        * �豸��Ϣ
        */
        typedef struct DeviceInfo
        {
            char szDeviceId[QHVC_INTERACT_DEVICE_INFO_MAX_LEN];     //�豸ID
            char szDeviceName[QHVC_INTERACT_DEVICE_INFO_MAX_LEN];   //�豸����
        } DeviceInfo;

        /**
        * �豸״̬
        */
        enum DEVICE_STATE
        {
            DEVICE_STATE_ACTIVE = 1,        //�豸����
            DEVICE_STATE_DISABLED = 2,      //�豸������
            DEVICE_STATE_NOT_PRESENT = 4,   //�豸������
            DEVICE_STATE_UNPLUGGED = 8      //�豸���γ�
        };

        /**
        * ת��N+1·��ʱ�������Ĳ���
        */
        struct MixStreamRegion {
            char userID[256]; //�û�ΨһID
            int x; //��������ڻ�����x����
            int y; //��������ڻ�����y����
            int width; //չʾ���
            int height; //չʾ�߶�
            int zOrder; //0��ʾ������ͼ��λ�����²㣬100��ʾ������ͼ��λ�����ϲ�
            int alpha; //[0, 1.0] 0��ʾͼ��Ϊ͸���ģ� 1��ʾͼ��Ϊ��ȫ��͸��
            int renderMode;  //��Ⱦ���ģʽ, Ĭ��RenderMode.RENDER_MODE_HIDDEN������μ�{@link RenderMode#RENDER_MODE_HIDDEN}��

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

            char mixStreamAddr[1024]; //������Ƶ������ַ
            int canvasWidth; //������ȣ��������Ŀ�ȣ�
            int canvasHeight; //�����߶ȣ��������ĸ߶ȣ�
            int videoFps; //������֡��
            int videoBitrate; //����֡�ʣ���λ��kbps��
            int mIframeInterval; // n seconds between I-frames ��λ���룩
            char backgroundColor[10]; //������ɫ eg: "#c0c0c0"
            //��Ƶ����
            char audioFormat[10]; // ��Ƶ��ʽ
            int audioSampleRate; // ��Ƶ������
            int audioBitrate; // ��ƵĿ������
            int audioNumsChannel; // ��Ƶ������

            MixStreamConfig() {
                memset(mixStreamAddr, 0, 1024);
                canvasWidth = 0;
                canvasHeight = 0;
                videoFps = 15;
                videoBitrate = 600;
                mIframeInterval = 2;
                memset(backgroundColor, 0, 10);
                strcpy_s(backgroundColor, 9, "#000000");
                //��Ƶ����
                memset(audioFormat, 0, 10);
                strcpy_s(audioFormat, 9, "aac");
                audioSampleRate = 44100;
                audioBitrate = 48;
                audioNumsChannel = 1;

            }

        } MixStreamConfig;


        enum STREAM_LIFE_CYCLE {
            STREAM_LIFE_CYCLE_BIND_USER, //������
            STREAM_LIFE_CYCLE_BIND_ROOM //�󶨷���
        };

        /**
        * ��ַ
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
        * ��Ƶ֡���ݸ�ʽ
        */
        struct VideoFrame {
            VIDEO_FRAME_TYPE type; // Ŀǰֻ����YUV 420
            int width;  //��Ƶ���ؿ��
            int height;  //��Ƶ���ظ߶�
            int yStride;  //YUV �����е� Y ���������п��
            int uStride;  //YUV �����е� U ���������п��
            int vStride;  //YUV �����е� V ���������п��
            void* yBuffer;  //ָ�� YU V�����е� Y ������ָ���ָ��
            void* uBuffer;  //ָ�� YU V�����е� U ������ָ���ָ��
            void* vBuffer;  //ָ�� YU V�����е� V ������ָ���ָ��
            int rotation; // ����Ⱦ��Ƶǰ���ø�֡��˳ʱ����ת�Ƕȣ�Ŀǰ֧�� 0 �ȣ�90 �ȣ�180 �ȣ��� 270 �ȡ�
            long long renderTimeMs; //  ��Ⱦ��Ƶ����ʱ������û�����Ⱦ��Ƶ��ʱ��ʹ�ø�ʱ���ͬ����Ƶ������Ⱦ����ʱ���Ϊ��Ⱦ��Ƶ����ʱ��������ǲɼ���Ƶ����ʱ�����
        };

        enum AUDIO_FRAME_TYPE {
            FRAME_TYPE_PCM16 = 0,  //PCM 16bit little endian
        };

        /**
        * ��Ƶ֡���ݸ�ʽ
        */
        struct AudioFrame {
            AUDIO_FRAME_TYPE type;// Ŀǰֻ����PCM 16bit 
            int samples;  //��֡����������
            int bytesPerSample;  //ÿ���������ֽ���: PCM (16λ)�������ֽ�
            int channels;  //Ƶ������(������������������ǽ����)
            int samplesPerSec;  //������
            void* buffer;  //���ݻ�����
            long long  renderTimeMs; // ��Ⱦ��Ƶ����ʱ������û��ڽ�����Ƶ����Ⱦʱʹ�ø�ʱ���ͬ����Ƶ������Ⱦ����ʱ���Ϊ��Ⱦ��Ƶ����ʱ��������ǲɼ���Ƶ����ʱ�����
        };
    }
}

#endif