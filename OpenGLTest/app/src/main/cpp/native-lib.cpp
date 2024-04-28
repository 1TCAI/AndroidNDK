#include <jni.h>
#include <string>

#include "Render.h"
#include "JNICallbackHelper.h"

Render m_ndkRender;
extern "C" JNIEXPORT jstring JNICALL
Java_com_example_opengltest_MainActivity_stringFromJNI(
        JNIEnv* env,
        jobject /* this */) {
    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}

#include <GLES/gl.h>
#include <GLES2/gl2.h>


float m_angle =0.0f;
GLuint m_texID[6];


JavaVM *vm = nullptr;
jint JNI_OnLoad(JavaVM *vm, void *args ){

    ::vm = vm;
    return JNI_VERSION_1_6;
}

extern "C"
JNIEXPORT void JNICALL
Java_com_example_opengltest_CCGLRender_ndkInitGL(JNIEnv *env, jobject thiz,jobject assetManager) {
//    JNICallbackHelper * helper  = new JNICallbackHelper(vm, env,thiz);
    AAssetManager *astManager = AAssetManager_fromJava (env, assetManager);
    if (NULL != astManager){
        m_ndkRender.SetupAssetManager(astManager);
    }
    m_ndkRender.InitGL();
}

extern "C"
JNIEXPORT void JNICALL
Java_com_example_opengltest_CCGLRender_ndkPaintGL(JNIEnv *env, jobject thiz) {
    m_ndkRender.PaintGL();
}

extern "C"
JNIEXPORT void JNICALL
Java_com_example_opengltest_CCGLRender_ndkResizeGL(JNIEnv *env, jobject thiz, jint width,
                                                   jint height) {
    m_ndkRender.ResizeGL(width,height);
}

