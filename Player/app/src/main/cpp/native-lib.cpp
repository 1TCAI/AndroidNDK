#include <jni.h>
#include <string>
#include "Player.h"
#include "JNICallbackHelper.h"
#include <android/native_window_jni.h>

#include <android/log.h>
#define TAG "Derry"
// __VA_ARGS__ 代表 ...的可变参数
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, TAG,  __VA_ARGS__);
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, TAG,  __VA_ARGS__);
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, TAG,  __VA_ARGS__);






extern "C" JNIEXPORT jstring JNICALL
Java_com_example_player_MainActivity_stringFromJNI(
        JNIEnv* env,
        jobject /* this */) {
    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}

JavaVM * vm = nullptr;
jint JNI_OnLoad(JavaVM * vm, void * args){

    ::vm = vm;
    return JNI_VERSION_1_6;
}
ANativeWindow *window = nullptr;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void renderCallback(uint8_t * src_data, int linesize, int width, int height){
    pthread_mutex_lock(&mutex);
    if (!window) {
        pthread_mutex_unlock(&mutex); // 出现了问题后，必须考虑到，释放锁，怕出现死锁问题
    }

    // 设置窗口的大小，各个属性
    ANativeWindow_setBuffersGeometry(window, width, height, WINDOW_FORMAT_RGBA_8888);

    // 他自己有个缓冲区 buffer
    ANativeWindow_Buffer window_buffer; // 目前他是指针吗？

    // 如果我在渲染的时候，是被锁住的，那我就无法渲染，我需要释放 ，防止出现死锁
    if (ANativeWindow_lock(window, &window_buffer, 0)) {
        ANativeWindow_release(window);
        window = 0;

        pthread_mutex_unlock(&mutex); // 解锁，怕出现死锁
        return;
    }



    pthread_mutex_unlock(&mutex);

}

Player* player = nullptr;
extern "C"
JNIEXPORT void JNICALL
Java_com_example_player_Player_prepareNative(JNIEnv *env, jobject thiz, jstring data_source) {

    const char * datasource = env->GetStringUTFChars(data_source,NULL);
    LOGD("datasource: %s\n",datasource)
    JNICallbackHelper * helper = new JNICallbackHelper(vm,env,thiz);
    player = new Player(datasource,helper);
    player->set_renderCallback(renderCallback);
    player->prepared();
    env->ReleaseStringUTFChars(data_source,datasource); //这个地方释放 导致类中指针悬空，需要深拷贝。
}
extern "C"
JNIEXPORT void JNICALL
Java_com_example_player_Player_startNative(JNIEnv *env, jobject thiz) {
    player->start();
}
extern "C"
JNIEXPORT void JNICALL
Java_com_example_player_Player_stopNative(JNIEnv *env, jobject thiz) {

}extern "C"
JNIEXPORT void JNICALL
Java_com_example_player_Player_releaseNative(JNIEnv *env, jobject thiz) {

}
extern "C"
JNIEXPORT void JNICALL
Java_com_example_player_Player_setSurfaceNative(JNIEnv *env, jobject thiz, jobject surface) {
    pthread_mutex_lock(&mutex);
    if (window){
        ANativeWindow_release(window);
        window = nullptr;
    }
    ANativeWindow_fromSurface(env,surface);
    pthread_mutex_unlock(&mutex);
}