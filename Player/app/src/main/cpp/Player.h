//
// Created by 96302 on 2024-04-06.
//

#ifndef PLAYER_PLAYER_H
#define PLAYER_PLAYER_H

#include <cstring>
#include <pthread.h>
#include "VideoChannel.h"
#include "AudioChannel.h"
#include "JNICallbackHelper.h"
#include "util.h"

#include <android/log.h>
#define TAG "Derry"
// __VA_ARGS__ 代表 ...的可变参数
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, TAG,  __VA_ARGS__);
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, TAG,  __VA_ARGS__);
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, TAG,  __VA_ARGS__);
extern "C"{
    #include <libavformat/avformat.h>
    #include <libavutil/error.h>
};
class Player {

public:
    Player(const char * data_source,JNICallbackHelper * helper);
    ~Player();
    void prepared();
    void prepared_();

    void start();

    void task_start_();
    void set_renderCallback(RenderCallback renderCallback);

private:
    char * data_source = nullptr;
    pthread_t pid_prepare = -1;
    AVFormatContext * fmt_ctx = nullptr;
    VideoChannel * video_channel = nullptr;
    AudioChannel * audio_channel = nullptr;
    JNICallbackHelper * helper = nullptr;
    bool isPlay;
    pthread_t pid_start;
    RenderCallback renderCallback;

};


#endif //PLAYER_PLAYER_H
