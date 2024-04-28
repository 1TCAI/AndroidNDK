#include <jni.h>
#include <string>
#include <pthread.h>

// 日志输出
#include <android/log.h>

#define TAG "JNISTUDY"
// __VA_ARGS__ 代表 ...的可变参数
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, TAG,  __VA_ARGS__);
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, TAG,  __VA_ARGS__);
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, TAG,  __VA_ARGS__);




extern "C" JNIEXPORT jstring JNICALL
Java_com_example_jnitest02_MainActivity_stringFromJNI(
        JNIEnv* env,
        jobject /* this */) {
    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}


void func1(){
    LOGD("动态注册 func1\n")
}
int func2(JNIEnv * env, jobject obj, jstring str){
    LOGD("动态注册 func2\n")
    return reinterpret_cast<int>(200);
}
const JNINativeMethod methods[] = {
        {"dynamicMethod1","()V",(void *)func1},
        {"dynamicMethod2","(Ljava/lang/String;)I",(int *)func2}
};
JavaVM * vm = nullptr;
jint JNI_OnLoad(JavaVM * vm, void * args){
    ::vm = vm;
    JNIEnv * env;
    jint r = vm->GetEnv(reinterpret_cast<void **>(&env),JNI_VERSION_1_6);
    if(r != JNI_OK){
        return -1;
    }
    jclass MainActicityClass = env->FindClass("com/example/jnitest02/MainActivity");

    r = env->RegisterNatives(MainActicityClass,methods, sizeof(methods)/ sizeof(JNINativeMethod));
    if(r != JNI_OK){
        return -1;
    }
    return JNI_VERSION_1_6;
}
class MyContext{
public:
//    JNIEnv * env;
    jobject instance;
};

void * threadFunc(void *args){

    LOGD("c++异步线程\n")
    MyContext * mycontext = (MyContext *)args;

    JNIEnv * env = nullptr;
    jint r = ::vm->AttachCurrentThread(&env, nullptr);
//    if (r != JNI_OK){
//        return
//    }
    jclass MainActivityClass = env->GetObjectClass(mycontext->instance);
    jmethodID voidMethodID = env->GetMethodID(MainActivityClass,"updateActivityUI","()V");
    env->CallVoidMethod(mycontext->instance,voidMethodID);
    ::vm->DetachCurrentThread();
    return nullptr;
}

extern "C"
JNIEXPORT void JNICALL
Java_com_example_jnitest02_MainActivity_naitveThread(JNIEnv *env, jobject thiz) {

    //主线程调用
//    jclass MainActivityClass = env->GetObjectClass(thiz);
//    jmethodID voidMethodID = env->GetMethodID(MainActivityClass,"updateActivityUI","()V");
//    env->CallVoidMethod(thiz,voidMethodID);

    // JavaVM * 是全局的，是相当于 APP进程的 全局 成员，他什么都可以
    // JNIEnv *jniEnv = nullptr;  // 不能跨线程 ，会奔溃
    //jobject instance = nullptr; // 不能跨线程 ，会奔溃, 并且不能跨函数 也会奔溃
    //env 可以提升为全局是没有用的，它只能在主线程中用。
    //obj 可以提升为全局。
    MyContext * mycontext = new MyContext();
    jobject gObject = env->NewGlobalRef(thiz);
    mycontext->instance = gObject;

    pthread_t pid;          //但是异步线程没有env进行调用java的函数。
    pthread_create(&pid, nullptr,threadFunc, mycontext);
    pthread_join(pid, nullptr);


    env->DeleteGlobalRef(gObject);
    delete mycontext;
    mycontext = nullptr;
}

extern "C"
JNIEXPORT void JNICALL
Java_com_example_jnitest02_MainActivity_closeThread(JNIEnv *env, jobject thiz) {

}