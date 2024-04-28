//
// Created by 96302 on 2024-04-07.
//

#include "JNICallbackHelper.h"

JNICallbackHelper::JNICallbackHelper(JavaVM *vm, JNIEnv *env, jobject job) {
    this->vm= vm;
    this->env = env;
    env->NewGlobalRef(job);
    jclass clazz = env->GetObjectClass(job);
    jmd_prepared = env->GetMethodID(clazz,"onPrepared","()V");
}

JNICallbackHelper::~JNICallbackHelper() {
    vm = nullptr;
    job = nullptr;
    env->DeleteGlobalRef(job);
    env = nullptr;
}

void JNICallbackHelper::onPrepared(int thread_mode) {
    LOGD("onPrepared: %d\n",thread_mode)
    if(thread_mode == THREAD_MAIN){
        env->CallVoidMethod(job,jmd_prepared);
    } else if (thread_mode == THREAD_CHILD){
        JNIEnv *env_child;
        vm->AttachCurrentThread(&env_child,0);
        env_child->CallVoidMethod(job,jmd_prepared);
        vm->DetachCurrentThread();
    }
}
