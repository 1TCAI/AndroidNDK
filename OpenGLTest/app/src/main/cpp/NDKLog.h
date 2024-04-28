//
// Created by 96302 on 2024-04-23.
//

#ifndef OPENGLTEST_NDKLOG_H
#define OPENGLTEST_NDKLOG_H

#include <android/log.h>
#define LOG "CCNDK-OpenGLES"
#define LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG,LOG,__VA_ARGS__)
#define LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG,__VA_ARGS__)
#define LOGW(...)  __android_log_print(ANDROID_LOG_WARN,LOG,__VA_ARGS__)
#define LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG,__VA_ARGS__)
#define LOGF(...)  __android_log_print(ANDROID_LOG_FATAL,LOG,__VA_ARGS__)








#endif //OPENGLTEST_NDKLOG_H
