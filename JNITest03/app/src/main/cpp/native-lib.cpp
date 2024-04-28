#include <jni.h>
#include <string>

#include <android/log.h>

#define TAG "JNISTUDY"
// __VA_ARGS__ 代表 ...的可变参数
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, TAG,  __VA_ARGS__);
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, TAG,  __VA_ARGS__);
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, TAG,  __VA_ARGS__);

extern "C" JNIEXPORT jstring JNICALL
Java_com_example_jni04_1study_MainActivity_stringFromJNI(
        JNIEnv* env,
        jobject /* this */) {
    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}
void func1(JNIEnv *env, jobject thiz){
    LOGD("func1 动态注册成功")
}
int func2(JNIEnv *env, jobject thiz, jstring str){
    LOGD("func2 动态注册成功")
    const char * _str = env->GetStringUTFChars(str,NULL);
    LOGD("func2str: %s\n",_str)
    return reinterpret_cast<int>(200);
}
static const JNINativeMethod methods[] = {
        {"dynamicMethod1","()V",(void *)func1},
        {"dynamicMethod2","(Ljava/lang/String;)I",(jint *)func2}
};
JavaVM *vm = nullptr;
jint JNI_OnLoad(JavaVM * vm, void * args){
    ::vm = vm;
    JNIEnv * env;
    jint r = vm->GetEnv(reinterpret_cast<void**>(&env),JNI_VERSION_1_6);
    if (r != JNI_OK){
        return -1;
    }
    jclass MainActivityClass = env->FindClass("com/example/jni04_study/MainActivity");
    r = env->RegisterNatives(MainActivityClass,methods, sizeof(methods)/sizeof(JNINativeMethod));
    if (r != 0){
        LOGD("动态注册失败")
    }else{
        LOGD("动态注册成功")
    }
    return JNI_VERSION_1_6;
}

extern "C"
JNIEXPORT void JNICALL
Java_com_example_jni04_1study_MainActivity_naitveThread(JNIEnv *env, jobject thiz) {

    jclass updateActivityUIClass = env->GetObjectClass(thiz);
    jmethodID updateActivityUIID = env->GetMethodID(updateActivityUIClass,"updateActivityUI","()V");
    env->CallVoidMethod(thiz,updateActivityUIID);
}