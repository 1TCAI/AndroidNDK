//
// Created by 96302 on 2024-04-24.
//

#ifndef OPENGLTEST_JNICALLBACKHELPER_H
#define OPENGLTEST_JNICALLBACKHELPER_H

#include <jni.h>

class JNICallbackHelper {

private:
    JavaVM *vm = 0;
    JNIEnv *env = 0;
    jobject job;
    jmethodID jmd_prepared;

public:
    JNICallbackHelper(JavaVM *vm, JNIEnv *env, jobject job);

    ~JNICallbackHelper();

};


#endif //OPENGLTEST_JNICALLBACKHELPER_H
