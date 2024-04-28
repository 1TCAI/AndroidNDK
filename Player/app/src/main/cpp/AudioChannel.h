//
// Created by 96302 on 2024-04-07.
//

#ifndef PLAYER_AUDIOCHANNEL_H
#define PLAYER_AUDIOCHANNEL_H
extern "C"{
#include <libavcodec/avcodec.h>
};

#include "BaseChannel.h"

class AudioChannel :public BaseChannel{

public:
    AudioChannel(int stream_index, AVCodecContext *codecContext);

    virtual ~AudioChannel();
    void stop();

    void start();

    pthread_t pid_audio_decode;
    pthread_t pid_audio_play;

    void audio_decode();

    void audio_play();
};


#endif //PLAYER_AUDIOCHANNEL_H
