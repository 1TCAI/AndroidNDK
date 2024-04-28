#include <jni.h>
#include <string>
#include <fmod.hpp>
#include <unistd.h>

extern "C" JNIEXPORT jstring JNICALL
Java_com_example_voicechange_MainActivity_stringFromJNI(
        JNIEnv* env,
        jobject /* this */) {
    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}
#undef com_derry_derry_voicechange_MainActivity_MODE_NORMAL
#define com_derry_derry_voicechange_MainActivity_MODE_NORMAL 0L
#undef com_derry_derry_voicechange_MainActivity_MODE_LUOLI
#define com_derry_derry_voicechange_MainActivity_MODE_LUOLI 1L
#undef com_derry_derry_voicechange_MainActivity_MODE_DASHU
#define com_derry_derry_voicechange_MainActivity_MODE_DASHU 2L
#undef com_derry_derry_voicechange_MainActivity_MODE_JINGSONG
#define com_derry_derry_voicechange_MainActivity_MODE_JINGSONG 3L
#undef com_derry_derry_voicechange_MainActivity_MODE_GAOGUAI
#define com_derry_derry_voicechange_MainActivity_MODE_GAOGUAI 4L
#undef com_derry_derry_voicechange_MainActivity_MODE_KONGLING
#define com_derry_derry_voicechange_MainActivity_MODE_KONGLING 5L

using namespace FMOD;
extern "C"
JNIEXPORT void JNICALL
Java_com_example_voicechange_MainActivity_voiceChangeNative(JNIEnv *env, jobject thiz, jint mode,
                                                            jstring path) {

    char * content_ = "默认 播放完毕";
    const char * path_ = env->GetStringUTFChars(path,NULL);

    System * system = nullptr;
    Sound * sound = nullptr;
    Channel * channel = nullptr;
    DSP * dsp = 0;
    // TODO 第一步 创建系统
    System_Create(&system);

    // TODO 第二步 系统的初始化 参数1：最大音轨数，  参数2：系统初始化标记， 参数3：额外数据
    system->init(32, FMOD_INIT_NORMAL, 0);

    // TODO 第三步 创建声音  参数1：路径，  参数2：声音初始化标记， 参数3：额外数据， 参数4：声音指针
    system->createSound(path_, FMOD_DEFAULT, 0, &sound);

    // TODO 第四步：播放声音  音轨 声音
    // 参数1：声音，  参数2：分组音轨， 参数3：控制， 参数4：通道
    system->playSound(sound, 0, false, &channel);

    // TODO 第五步：增加特效
    switch (mode) {
        case com_derry_derry_voicechange_MainActivity_MODE_NORMAL: // 原生
            content_ = "原生 播放完毕";
            break;
        case com_derry_derry_voicechange_MainActivity_MODE_LUOLI: // 萝莉
            content_ = "萝莉 播放完毕";
            // 音调高 -- 萝莉 2.0
            // 1.创建DSP类型的Pitch 音调条件
            system->createDSPByType(FMOD_DSP_TYPE_PITCHSHIFT, &dsp);
            // 2.设置Pitch音调调节2.0
            dsp->setParameterFloat(FMOD_DSP_PITCHSHIFT_PITCH, 2.0f);
            // 3.添加音效进去 音轨
            channel->addDSP(0, dsp);
            break;
        case com_derry_derry_voicechange_MainActivity_MODE_DASHU: // 大叔
            content_ = "大叔 播放完毕";

            // 音调低 -- 大叔 0.7
            // 1.创建DSP类型的Pitch 音调条件
            system->createDSPByType(FMOD_DSP_TYPE_PITCHSHIFT, &dsp);
            // 2.设置Pitch音调调节2.0
            dsp->setParameterFloat(FMOD_DSP_PITCHSHIFT_PITCH, 0.7f);
            // 3.添加音效进去 音轨
            channel->addDSP(0, dsp);
            break;
        case com_derry_derry_voicechange_MainActivity_MODE_GAOGUAI: // 搞怪
            content_ = "搞怪 小黄人 播放完毕";
            // 小黄人声音 频率快
            // 从音轨拿 当前 帧率
            float mFrequency;
            channel->getFrequency(&mFrequency);
            // 修改 帧率
            channel->setFrequency(mFrequency * 1.5f);
            break;

        case com_derry_derry_voicechange_MainActivity_MODE_JINGSONG: // 惊悚
            content_ = "惊悚 播放完毕";

            // 惊悚音效：特点： 很多声音的拼接

            // TODO 音调低
            // 音调低 -- 大叔 0.7
            // 1.创建DSP类型的Pitch 音调条件
            system->createDSPByType(FMOD_DSP_TYPE_PITCHSHIFT, &dsp);
            // 2.设置Pitch音调调节2.0
            dsp->setParameterFloat(FMOD_DSP_PITCHSHIFT_PITCH, 0.7f);
            // 3.添加音效进去 音轨
            channel->addDSP(0, dsp); // 第一个音轨

            // TODO 搞点回声
            // 回音 ECHO
            system->createDSPByType(FMOD_DSP_TYPE_ECHO, &dsp);
            dsp->setParameterFloat(FMOD_DSP_ECHO_DELAY, 200); // 回音 延时    to 5000.  Default = 500.
            dsp->setParameterFloat(FMOD_DSP_ECHO_FEEDBACK, 10); // 回音 衰减度 Default = 50   0 完全衰减了
            channel->addDSP(1,dsp); // 第二个音轨

            // TODO 颤抖
            // Tremolo 颤抖音 正常5    非常颤抖  20
            system->createDSPByType(FMOD_DSP_TYPE_TREMOLO, &dsp);
            dsp->setParameterFloat(FMOD_DSP_TREMOLO_FREQUENCY, 20); // 非常颤抖
            dsp->setParameterFloat(FMOD_DSP_TREMOLO_SKEW, 0.8f); // ？？？
            channel->addDSP(2, dsp); // 第三个音轨
            break;

        case com_derry_derry_voicechange_MainActivity_MODE_KONGLING: // 空灵  学校广播
            content_ = "空灵 播放完毕";

            // 回音 ECHO
            system->createDSPByType(FMOD_DSP_TYPE_ECHO, &dsp);
            dsp->setParameterFloat(FMOD_DSP_ECHO_DELAY, 200); // 回音 延时    to 5000.  Default = 500.
            dsp->setParameterFloat(FMOD_DSP_ECHO_FEEDBACK, 10); // 回音 衰减度 Default = 50   0 完全衰减了
            channel->addDSP(0,dsp);
            break;
    }
    // 监听音轨 通道，释放播放完，如果播放完，我就让你释放，否则我一直等待
    bool isPlayer = 9;
    while (isPlayer) {
        channel->isPlaying(&isPlayer); // 如果播放完成，内部会通过地址 修改为 0
        usleep(1000 * 1000); // 每隔一秒
    }

    // 时时刻刻记得回收
    sound->release();
    system->close();
    system->release();
    env->ReleaseStringUTFChars(path, path_);

    // 告知Java播放完成
    jclass mainCls = env->GetObjectClass(thiz);
    jmethodID endMethod = env->GetMethodID(mainCls, "playerEnd", "(Ljava/lang/String;)V");
    jstring value = env->NewStringUTF(content_);
    env->CallVoidMethod(thiz, endMethod, value);
}