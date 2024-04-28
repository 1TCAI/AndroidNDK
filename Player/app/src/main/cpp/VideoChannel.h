//
// Created by 96302 on 2024-04-07.
//

#ifndef PLAYER_VIDEOCHANNEL_H
#define PLAYER_VIDEOCHANNEL_H
extern "C"{
    #include <libavcodec/avcodec.h>
    #include <libswscale/swscale.h>
    #include <libavutil/imgutils.h>
};
#include "BaseChannel.h"

typedef void (*RenderCallback)(uint8_t *, int, int, int);   //函数指针声明

class VideoChannel : public BaseChannel{

private:
    pthread_t pid_vide_decode;
    pthread_t pid_vide_play;
    RenderCallback renderCallback;
public:
    VideoChannel(int stream_index, AVCodecContext *codecContext);

    virtual ~VideoChannel();
    void stop();

    void start();

    void video_decode();

    void video_play();
    void set_renderCallback(RenderCallback renderCallback);
};


#endif //PLAYER_VIDEOCHANNEL_H
