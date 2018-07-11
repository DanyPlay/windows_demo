#ifndef _QHVC_INTERACT_CALLBACK_H_
#define _QHVC_INTERACT_CALLBACK_H_

#pragma once

#include "QHVCInteractConstant.h"

namespace QHVC
{
    namespace INTERACT
    {
        /**
        * ͨ��ͳ����Ϣ
        */
        typedef struct RtcStats
        {
            unsigned int totalDuration;         //�ۼ�ͨ��ʱ��(��)
            unsigned int txBytes;               //�ۼƷ����ֽ���(bytes)
            unsigned int rxBytes;               //�ۼƽ����ֽ���(bytes)
            unsigned int txKBitRate;            //��������(kbps)��˲ʱֵ
            unsigned int rxKBitRate;            //��������(kbps)��˲ʱֵ

            unsigned int txAudioKBitRate;       //��Ƶ��������(kbps)��˲ʱֵ
            unsigned int rxAudioKBitRate;       //��Ƶ��������(kbps)��˲ʱֵ
            unsigned int txVideoKBitRate;       //��Ƶ��������(kbps)��˲ʱֵ
            unsigned int rxVideoKBitRate;       //��Ƶ��������(kbps)��˲ʱֵ

            int users;                          //�û���
            double cpuTotalUsage;               //��ǰϵͳ��CPUʹ����(%)
            double cpuAppUsage;                 //��ǰӦ�ó����CPUʹ����(%)

            RtcStats()
            {
                ZeroMemory(this, sizeof(RtcStats));
            }
        } RtcStats;

        /**
        * ������Ƶͳ����Ϣ
        */
        typedef struct LocalVideoStats
        {
            unsigned int sentBitrate;           //���ϴ�ͳ�ƺ����ֽ���(bytes)
            unsigned int sentFrameRate;         //���ϴ�ͳ�ƺ��͵�֡��
            unsigned int packageLost;           //���ϴ�ͳ�ƺ󶪰���

            LocalVideoStats()
            {
                ZeroMemory(this, sizeof(LocalVideoStats));
            }
        } LocalVideoStats;

        /**
        * Զ����Ƶͳ����Ϣ
        */
        typedef struct RemoteVideoStats
        {
            char userId[64];                    //�û�ID
            unsigned int delay;                 //�ӳ�
            unsigned int width;                 //��Ƶ�ֱ���-��
            unsigned int height;                //��Ƶ�ֱ���-��
            unsigned int receivedBitrate;       //���ϴ�ͳ�ƺ���ܵ��ֽ���(byte)
            unsigned int receivedFrameRate;     //���ϴ�ͳ�ƺ���ܵ�֡��
            VIDEO_STREAM_LEVEL rxStreamType;    //��Ƶ���ȼ�
            QUALITY quality;                    //��Ƶ������

            RemoteVideoStats()
            {
                ZeroMemory(this, sizeof(RemoteVideoStats));
            }
        } RemoteVideoStats;

        /**
        * ��Ƶ������Ϣ
        */
        typedef struct AudioVolumeInfo
        {
            char userId[64];;                   //�û�ID
            unsigned int volume;                //����

            AudioVolumeInfo()
            {
                ZeroMemory(this, sizeof(AudioVolumeInfo));
            }
        } AudioVolumeInfo;

        class IQHVCInteractCallback
        {
        public:
            /**
            * ����ص�
            * ע���ûص���ʾSDK����ʱ�����ˣ������ý����صģ����档ͨ������£�SDK�ϱ��ľ�����ϢӦ�ó�����Ժ��ԣ�SDK���Զ��ָ�
            *
            * @param warn �����룬�ο�{@link INTERACT::WARN}����
            * @param msg ������Ϣ
            */
            virtual void OnWarning(int warn, const char* msg) = 0;

            /**
            * ����ص�
            * ע���ûص���ʾSDK����ʱ�����ˣ������ý����صģ�����ͨ������£�SDK�ϱ��Ĵ�����ζ��SDK�޷��Զ��ָ�����ҪӦ�ó����Ԥ
            * ����ʾ�û���
            *
            * @param error �����룬�ο�{@link INTERACT::ERR}����
            * @param msg ������Ϣ
            */
            virtual void OnError(int error, const char* msg) = 0;

            /**
            * ���ػ���ֱ������ɹ�����ʱҵ�񷽿��Խ���һϵ�в��������ã�֮�����{@link JoinChannel}����������
            *
            * @param roomId ����ID
            * @param userId �û�ID
            */
            virtual void OnLoadEngineSuccess(const char* roomId, const char* userId) = 0;

            /**
            * ���뷿��ɹ�
            *
            * @param roomId ����ID
            * @param userId �û�ID
            * @param elapsed ��{@link JoinChannel}��ʼ�����¼��������ӳ٣���λ����
            */
            virtual void OnJoinChannelSuccess(const char* roomId, const char* userId, long elapsed) = 0;

            /**
            * ���¼��뷿��ɹ�����������ԭ�򣬿ͻ��˿��ܻ�ͷ�����ʧȥ���ӣ�SDK������Զ��������Զ������ɹ��󴥷��˻ص�
            *
            * @param roomId ����ID
            * @param userId �û�ID
            * @param elapsed ��{@link JoinChannel}��ʼ�����¼��������ӳ٣���λ����
            */
            virtual void OnRejoinChannelSuccess(const char* roomId, const char* userId, long elapsed) = 0;

            /**
            * �뿪����ص�
            *
            * @param rtcStats ͨ����ص�ͳ����Ϣ
            */
            virtual void OnLeaveChannel(const RtcStats& rtcStats) = 0;

            /**
            * ��ɫ����л��ɹ��ص�
            * ������{@link SetClientRole}�����л���ɫ���ʱ���л��ɹ���ص��˷���������л����ʧ�ܣ���ص�onError()
            * ע��ֻ�м���Ƶ���ɹ�֮���л���ݲŻ��д˻ص���
            *
            * @param clientRole �ı��Ľ�ɫ����
            */
            virtual void OnChangeClientRoleSuccess(CLIENT_ROLE clientRole) = 0;

            /**
            * �����жϻص����ûص���ʾSDK�ͷ�����ʧȥ���������ӡ�ʧȥ���Ӻ󣬳���Ӧ�ó�����������{@link LeaveChannel}��
            * SDK��һֱ�����Զ�����
            */
            virtual void OnConnectionInterrupted() = 0;

            /**
            * ���Ӷ�ʧ�ص����ûص���ʾSDK�ͷ�����ʧȥ���������ӣ����ҳ����Զ�����һ��ʱ�䣨Ĭ��10�룩����δ����
            *
            * @param error �����룬�ο�{@link INTERACT::ERR}����
            */
            virtual void OnConnectionLost(int error) = 0;

            /**
            * �����û����뵱ǰ����
            * ע�������ǰ�û����뷿��ʱ�Ѿ������ڷ����ڣ�SDKҲ����Ӧ�ó����ϱ����ڷ����е��û�
            *
            * @param userId �û�ID
            * @param elapsed ��{@link JoinChannel}��ʼ�����¼��������ӳ٣���λ����
            */
            virtual void OnUserJoined(const char* userId, long elapsed) = 0;

            /**
            * �����û��뿪��ǰ����
            * ע��SDK�ж��û��뿪��������ݣ���һ��ʱ���ڣ�15�룩û���յ��Է����κ����ݰ����ж�Ϊ�Է����ߡ�������ϲ��
            * ����£����ܴ����󱨡�����ɿ��ĵ��߼����Ӧ�ó���ͨ����������������
            *
            * @param userId �û�ID
            * @param userOfflineReason �û��뿪ԭ��
            */
            virtual void OnUserOffline(const char* userId, USER_OFFLINE_REASON userOfflineReason) = 0;

            /**
            * �����û�����/�ر���Ƶ
            * ע:�ر���Ƶ������ָ���û�ֻ�ܽ�������ͨ����������ʾ�������Լ�����Ƶ��Ҳ���ܽ��ձ��˵���Ƶ
            *
            * @param userId �û�ID
            * @param enabled true ������Ƶ��false �ر���Ƶ
            */
            virtual void OnUserEnableVideo(const char* userId, bool enabled) = 0;

            /**
            * �û������ص�
            *
            * @param userId �û�ID
            * @param muted true �û�����Ƶ������false �û�ȡ������Ƶ����
            */
            virtual void OnUserMuteAudio(const char* userId, bool muted) = 0;

            /**
            * �û���ֹ/������Ƶ�ص�
            *
            * @param userId �û�ID
            * @param muted true �û���ֹ��Ƶ���ͣ�false �û�������Ƶ����
            */
            virtual void OnUserMuteVideo(const char* userId, bool muted) = 0;

            /**
            * ����ͷ���ûص������Կ�ʼ������Ƶ
            */
            virtual void OnCameraReady() = 0;

            /**
            * ������Ƶֹͣ�ص�
            */
            virtual void OnVideoStopped() = 0;

            /**
            * ������Ƶ��ʾ�ص�����һ֡������Ƶ�����Ѿ���ʾ����Ļ��
            *
            * @param width ��Ƶ��ȣ���λ����
            * @param height ��Ƶ�߶ȣ���λ����
            * @param elapsed �Ӽ��뷿�俪ʼ�����¼��������ӳ٣���λ����
            */
            virtual void OnFirstLocalVideoFrame(int width, int height, int elapsed) = 0;

            /**
            * Զ����Ƶ��ʾ�ص�����һ֡Զ����Ƶ�����Ѿ���ʾ����Ļ��
            *
            * @param userId �û�ID
            * @param width ��Ƶ��ȣ���λ����
            * @param height ��Ƶ�߶ȣ���λ����
            * @param elapsed �Ӽ��뷿�俪ʼ�����¼��������ӳ٣���λ����
            */
            virtual void OnFirstRemoteVideoFrame(const char* userId, int width, int height, int elapsed) = 0;

            /**
            * Զ����Ƶ���ղ�����ص���Ӧ�ó�������ڴ˻ص������ø��û���View��HWND��
            *
            * @param userId �û�ID
            * @param width ��Ƶ��ȣ���λ����
            * @param height ��Ƶ�߶ȣ���λ����
            * @param elapsed �Ӽ��뷿�俪ʼ�����¼��������ӳ٣���λ����
            */
            virtual void OnFirstRemoteVideoDecoded(const char* userId, int width, int height, int elapsed) = 0;

            /**
            * ������Ƶͳ����Ϣ�ص�
            *
            * @param stats ������Ƶͳ����Ϣ
            */
            virtual void OnLocalVideoStats(const LocalVideoStats& stats) = 0;

            /**
            * Զ����Ƶͳ����Ϣ�ص�
            *
            * @param stats Զ����Ƶͳ����Ϣ
            */
            virtual void OnRemoteVideoStats(const RemoteVideoStats& stats) = 0;

            /**
            * ��Ƶ�����ص���ͨ��������ÿ���봥��һ�Σ����浱ǰͨ������Ƶ����
            *
            * @param userId �û�ID
            * @param quality ��Ƶ��������
            * @param delay �ӳ٣���λ����
            * @param lost �����ʣ��ٷֱ�
            */
            virtual void OnAudioQuality(const char* userId, QUALITY quality, unsigned short delay, unsigned short lost) = 0;

            /**
            * ˵�����������ص�
            *
            * @param speakers ˵����
            * @param totalVolume ���������������0-255��
            */
            virtual void OnAudioVolumeIndication(const AudioVolumeInfo* speakers, int speakerNumber, unsigned int totalVolume) = 0;

            /**
            * ͳ�����ݻص����ûص������ϱ�SDK������״̬��ÿ2-3�봥��һ��
            *
            * @param stats ͨ��ͳ����Ϣ
            */
            virtual void OnRtcStats(const RtcStats& stats) = 0;

            /**
            * ���������ص�
            *
            * @param userId �û�ID
            * @param txQuality ������������
            * @param rxQality ������������
            */
            virtual void OnNetworkQuality(const char* userId, QUALITY txQuality, QUALITY rxQuality) = 0;

            /**
            * ��Ƶ�豸״̬�ı�
            *
            * @param deviceId �豸ID
            * @param deviceType ��Ƶ�豸���ͣ����������豸
            * @param deviceState ��Ƶ�豸״̬�����á����á������ڡ��γ���
            */
            virtual void OnAudioDeviceStateChanged(const char* deviceId, AUDIO_DEVICE_TYPE deviceType, DEVICE_STATE deviceState) = 0;

            /**
            * ��Ƶ�豸״̬�ı�
            *
            * @param deviceId �豸ID
            * @param deviceState ��Ƶ�豸״̬�����á����á������ڡ��γ���
            */
            virtual void OnVideoDeviceStateChanged(const char* deviceId, DEVICE_STATE deviceState) = 0;

            /**
            * ���ಥ�Ž���
            */
            virtual void OnAudioMixingFinished() = 0;

            /**
            * �����û���ʼ���Ű���
            */
            virtual void OnRemoteAudioMixingBegin() = 0;

            /**
            * �����û��������Ű���
            */
            virtual void OnRemoteAudioMixingEnd() = 0;

            /**
            * ��Ч���Ž���
            */
            virtual void OnAudioEffectFinished(int soundId) = 0;
        };

        class IQHVCInteractLocalVideoRenderCallback
        {
        public:
            /**
            * ������Ƶ���ݻص������ڻ������ͷ�ɼ���ͼ��
            * @param videoFrame ��Ƶ����
            * @return true:  �������óɹ���flase��ʾʧ��
            */
            virtual bool OnCaptureVideoFrame(VideoFrame& videoFrame) = 0;
        };

        class IQHVCInteractRemoteVideoRenderCallback
        {
        public:
            /**
            * Զ�����ݻص������ڻ�������û�����Ƶ��
            * @param userId �û�ID
            * @param videoFrame ��Ƶ����
            * @return true:  �������óɹ���flase��ʾʧ��
            */
            virtual bool OnRenderVideoFrame(const char* userId, VideoFrame& videoFrame) = 0;
        };

        class IQHVCInteractAudioFrameCallback
        {
        public:
            /**
            *  ����ʱ�����أ��Լ��ģ�������������ص�
            * @param AudioFrame ��Ƶ����
            * @return true:  �������óɹ���flase��ʾʧ��
            */
            virtual bool OnRecordAudioFrame(AudioFrame& audioFrame) = 0;

            /**
            *  ����������������ص�
            * @param AudioFrame ��Ƶ����
            * @return true:  �������óɹ���flase��ʾʧ��
            */
            virtual bool OnPlaybackAudioFrame(AudioFrame& audioFrame) = 0;
        };
    }
}

#endif
