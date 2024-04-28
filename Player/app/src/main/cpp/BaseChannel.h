//
// Created by 96302 on 2024-04-08.
//

#ifndef PLAYER_BASECHANNEL_H
#define PLAYER_BASECHANNEL_H

extern "C"{
    #include <libavcodec/avcodec>
};

#include "safe_queue.h"
class BaseChannel{
public:
    int stream_index;
    SafeQueue<AVPacket * > packets; // 压缩的 数据包队列
    SafeQueue<AVFrame *> frames;    // 原始的 数据包队列
    bool isPlay; // 音频 和 视频 都会有的标记 是否播放

    AVCodecContext *codecContext = 0; // 音频 视频 都需要的 解码器上下文

    BaseChannel(int stream_index, AVCodecContext *codecContext)
            :
            stream_index(stream_index),
            codecContext(codecContext)
    {
        packets.setReleaseCallback(releaseAVPacket); // 给队列设置Callback，Callback释放队列里面的数据
        frames.setReleaseCallback(releaseAVFrame); // 给队列设置Callback，Callback释放队列里面的数据
    }
    static void releaseAVPacket(AVPacket ** p) {
        if (p) {
            av_packet_free(p); // 释放队列里面的 T == AVPacket
            *p = 0;
        }
    }
    static void releaseAVFrame(AVFrame ** f) {
        if (f) {
            av_frame_free(f); // 释放队列里面的 T == AVFrame
            *f = 0;
        }
    }
    //父类析构一定要加virtual
    virtual ~BaseChannel() {
        packets.clear();
        frames.clear();
    }
};





#endif //PLAYER_BASECHANNEL_H
