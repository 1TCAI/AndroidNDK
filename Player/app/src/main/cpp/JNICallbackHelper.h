//
// Created by 96302 on 2024-04-07.
//

#ifndef PLAYER_JNICALLBACKHELPER_H
#define PLAYER_JNICALLBACKHELPER_H

#include <jni.h>
#include "util.h"

#include <android/log.h>
#define TAG "Derry"
// __VA_ARGS__ 代表 ...的可变参数
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, TAG,  __VA_ARGS__);
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, TAG,  __VA_ARGS__);
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, TAG,  __VA_ARGS__);


class JNICallbackHelper {
public:
    JNICallbackHelper(JavaVM *vm, JNIEnv *env, jobject job);
    ~JNICallbackHelper();
    void onPrepared(int thread_mode);

private:
    JavaVM * vm = nullptr;
    JNIEnv * env = nullptr;
    jobject job;

    jmethodID jmd_prepared;
};


#endif //PLAYER_JNICALLBACKHELPER_H
