#include <jni.h>
#include <string>

#include <android/log.h>
#define TAG "Derry"
// __VA_ARGS__ 代表 ...的可变参数
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, TAG,  __VA_ARGS__);
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, TAG,  __VA_ARGS__);
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, TAG,  __VA_ARGS__);

extern "C" JNIEXPORT jstring JNICALL
Java_com_example_jnitest01_MainActivity_stringFromJNI(
        JNIEnv* env,
        jobject /* this */) {
    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}

extern "C"
JNIEXPORT void JNICALL
Java_com_example_jnitest01_MainActivity_changeName(JNIEnv *env, jobject thiz) {

    //c++的env是一级指针 所以调用函数是 env->函数();
    //c  的env是二级指针 所以调用函数是 (*env)->函数();
    //c++是有对象的。                         env->函数(参数);
    //c  是没有对象的所以调用函数需要把env也传入。(*env)->函数(env,参数);
    jclass MainActivityClass = env->GetObjectClass(thiz);
    jfieldID nameID = env->GetFieldID(MainActivityClass,"name", "Ljava/lang/String;");
    jstring newName = env->NewStringUTF("li");
    env->SetObjectField(thiz, nameID,newName);
}extern "C"
JNIEXPORT void JNICALL
Java_com_example_jnitest01_MainActivity_changeAge(JNIEnv *env, jclass clazz) {

    jfieldID ageID = env->GetStaticFieldID(clazz,"age", "I");
    env->SetStaticIntField(clazz,ageID, 1000);
}extern "C"
JNIEXPORT void JNICALL
Java_com_example_jnitest01_MainActivity_changeNumber(JNIEnv *env, jobject thiz) {

    jclass MainActivityClass = env->GetObjectClass(thiz);

    jfieldID number1ID = env->GetFieldID(MainActivityClass,"number1", "D");
    env->SetDoubleField(thiz,number1ID,1000.0);

    jfieldID number2ID = env->GetFieldID(MainActivityClass,"number2", "D");
    env->SetDoubleField(thiz,number2ID,2000.0);
}extern "C"
JNIEXPORT jint JNICALL
Java_com_example_jnitest01_MainActivity_callIntMethod(JNIEnv *env, jobject thiz) {

    jclass MainActivityClass = env->GetObjectClass(thiz);

    jmethodID intMethodID = env->GetMethodID(MainActivityClass,"add","(II)I");
    jint r = env->CallIntMethod(thiz,intMethodID,1,1);
    return r;
}extern "C"
JNIEXPORT jstring JNICALL
Java_com_example_jnitest01_MainActivity_callStringMethod(JNIEnv *env, jobject thiz, jstring str) {

    const char * str_ = env->GetStringUTFChars(str,NULL);
    jclass MainActivityClass = env->GetObjectClass(thiz);
    jmethodID stringMethodID = env->GetMethodID(MainActivityClass,"show","(Ljava/lang/String;)Ljava/lang/String;");
    jstring newStr = env->NewStringUTF("callStringMethod");
    env->CallObjectMethod(thiz,stringMethodID,newStr);
    return newStr;
}extern "C"
JNIEXPORT void JNICALL
Java_com_example_jnitest01_MainActivity_testArray(JNIEnv *env, jobject thiz, jint count,
                                                  jstring str, jintArray ints, jobjectArray strs) {

    int _count = count;
    LOGD("testArray count %d\n:", _count);
    const char * _str = env->GetStringUTFChars(str,NULL);
    LOGD("testArray str: %s\n", _str);
    env->ReleaseStringUTFChars(str,_str);

    jint * intval = env->GetIntArrayElements(ints,NULL);
    jsize intsLength = env->GetArrayLength(ints);
    for (int i = 0; i < intsLength; ++i) {
        LOGD("testArray修改前： %d\n", *(intval + i));
        *(intval + i) += 100;
        LOGD("testArray修改后： %d\n", *(intval + i));
    }
    env->ReleaseIntArrayElements(ints,intval,JNI_OK);


    jsize strsLength = env->GetArrayLength(strs);
    LOGD("strsLength： %d\n", strsLength);

    for (int i = 0; i < strsLength; ++i) {
        jstring  strval = (jstring)env->GetObjectArrayElement(strs,i);
        const char * _strval = env->GetStringUTFChars(strval, NULL);
        LOGD("testArray修改前： %s\n", _strval);
        env->ReleaseStringUTFChars(strval,_strval);

        jstring newStrVal = env->NewStringUTF("lalala");
        const char * _newStrVal = env->GetStringUTFChars(newStrVal, NULL);
        env->SetObjectArrayElement(strs,i,newStrVal);
        LOGD("testArray修改后： %s\n", _newStrVal);
        env->ReleaseStringUTFChars(newStrVal,_newStrVal);

    }
}extern "C"
JNIEXPORT void JNICALL
Java_com_example_jnitest01_MainActivity_pubObject(JNIEnv *env, jobject thiz, jobject student,
                                                  jstring str) {

    const char * _str = env->GetStringUTFChars(str,NULL);
    LOGD("pubObject _str： %s\n", _str);
    env->ReleaseStringUTFChars(str,_str);

    jclass stuClass = env->GetObjectClass(student);
    //set name
    jmethodID setnameID = env->GetMethodID(stuClass,"setName", "(Ljava/lang/String;)V");
    jstring newname = env->NewStringUTF("pubObject");
    env->CallVoidMethod(student,setnameID,newname);
    //get name
    jmethodID getnameID = env->GetMethodID(stuClass,"getName", "()Ljava/lang/String;");
    jstring getname = (jstring)env->CallObjectMethod(student,getnameID);


    jmethodID showInfo = env->GetStaticMethodID(stuClass, "showInfo", "(Ljava/lang/String;)V");
    jstring value2 = env->NewStringUTF("静态的函数 李元霸");
    env->CallStaticVoidMethod(stuClass, showInfo, value2);
    env->DeleteLocalRef(stuClass); // 已经释放了

}