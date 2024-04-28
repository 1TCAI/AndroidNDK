//
// Created by 96302 on 2024-04-07.
//

#include "AudioChannel.h"

AudioChannel::AudioChannel(int stream_index, AVCodecContext *codecContext)
:BaseChannel(stream_index,codecContext)
{

}

AudioChannel::~AudioChannel() {

}

void AudioChannel::stop() {

}
void *task_audio_decode(void * args) {
    auto *audio_channel = static_cast<AudioChannel *>(args);
    audio_channel->audio_decode();
    return 0;
}

void *task_audio_play(void * args) {
    auto *audio_channel = static_cast<AudioChannel *>(args);
    audio_channel->audio_play();
    return 0;
}
void AudioChannel::start() {


    isPlay = 1;

    // 队列开始工作了samples_per_channel
    packets.setWork(1);
    frames.setWork(1);

    // 第一个线程： 音频：取出队列的压缩包 进行解码 解码后的原始包 再push队列中去 （音频：PCM数据）
    pthread_create(&pid_audio_decode, 0, task_audio_decode, this);

    // 第二线线程：视频：从队列取出原始包，播放  音频播放OpenSLES
    pthread_create(&pid_audio_play, 0, task_audio_play, this);
}

void AudioChannel::audio_decode() {
    AVPacket *pkt = 0;
    while (isPlaying) {
        int ret = packets.getQueueAndDel(pkt); // 阻塞式函数
        if (!isPlaying) {
            break; // 如果关闭了播放，跳出循环，releaseAVPacket(&pkt);
        }

        if (!ret) { // ret == 0
            continue; // 哪怕是没有成功，也要继续（假设：你生产太慢(压缩包加入队列)，我消费就等一下你）
        }

        // 最新的FFmpeg，和旧版本差别很大， 新版本：1.发送pkt（压缩包）给缓冲区，  2.从缓冲区拿出来（原始包）
        ret = avcodec_send_packet(codecContext, pkt);

        // FFmpeg源码缓存一份pkt，大胆释放即可
        releaseAVPacket(&pkt);

        if (ret) {
            break; // avcodec_send_packet 出现了错误，结束循环
        }

        // 下面是从 FFmpeg缓冲区 获取 原始包
        AVFrame *frame = av_frame_alloc(); // AVFrame： 解码后的视频原始数据包
        ret = avcodec_receive_frame(codecContext, frame);
        // 音频也有帧的概念，所以获取原始包的时候，最好还是判断下【严谨性，最好是判断下】
        if (ret == AVERROR(EAGAIN)) {
            continue; // 有可能音频帧，也会获取失败，重新拿一次
        } else if (ret != 0) {
            break; // 错误了
        }
        // 重要拿到了 原始包-- PCM数据
        frames.insertToQueue(frame);
    } // while end
    releaseAVPacket(&pkt); 

}

void AudioChannel::audio_play() {

}
