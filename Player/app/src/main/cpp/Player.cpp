//
// Created by 96302 on 2024-04-06.
//

#include "Player.h"
#include "AudioChannel.h"
#include "VideoChannel.h"

Player::Player(const char *data_source, JNICallbackHelper * helper) {
    this->data_source = new char[strlen(data_source) + 1];
    strcpy(this->data_source,data_source);  //深拷贝
    this->helper = helper;
    LOGD("Player datasource: %s\n", this->data_source);
}

Player::~Player() {
    if(data_source){
        delete [] this->data_source;
    }
    if (helper){
        delete helper;
    }
}
void * task_prepare(void * args){
    Player * player = static_cast<Player *>(args);
    LOGD("task_prepare\n");
    player->prepared_();
    return 0;
}
void Player::prepared_() {
    int ret = -1;
    fmt_ctx = avformat_alloc_context();
    if(fmt_ctx == nullptr){
        LOGD("avformat_alloc_context error\n")
        return;
    }
    AVDictionary * dit = nullptr;
    av_dict_set(&dit,"timeout","5000000",0);
    LOGD("avformat_open_input data_source: %s\n", data_source)
    ret = avformat_open_input(&fmt_ctx,data_source, nullptr,&dit);
    if(ret != 0){
        char buf[1024] = {0};
        av_strerror(ret,buf,1024);
        LOGD("avformat_open_input ret: %s\n", buf)
        return;
    }
    av_dict_free(&dit);

    ret = avformat_find_stream_info(fmt_ctx, nullptr);
    if(ret != 0){
        char buf[1024] = {0};
        av_strerror(ret,buf,1024);
        LOGD("avformat_find_stream_info ret: %s\n", buf)
        return;
    }
    //寻找媒体流
    int audio_idx = -1;
    ret = av_find_best_stream(fmt_ctx,AVMEDIA_TYPE_AUDIO,-1,-1, nullptr,0);
    if(ret != 0){
        char buf[1024] = {0};
        av_strerror(ret,buf,1024);
        LOGD("av_find_best_stream ret: %s\n", buf)
        return;
    }
    audio_idx = ret;
    int video_idx = -1;
    ret = av_find_best_stream(fmt_ctx,AVMEDIA_TYPE_VIDEO,-1,-1, nullptr,0);
    if(ret != 0){
        char buf[1024] = {0};
        av_strerror(ret,buf,1024);
        LOGD("av_find_best_stream ret: %s\n", buf)
        return;
    }
    video_idx = ret;

    //媒体流解码器
    AVCodec * audio_decoder = nullptr;
    audio_decoder = avcodec_find_decoder(fmt_ctx->streams[audio_idx]->codecpar->codec_id);
    if(!audio_decoder){
        LOGD("avcodec_find_decoder error\n")
        return;
    }
    AVCodecContext * audio_decoder_ctx = nullptr;
    audio_decoder_ctx = avcodec_alloc_context3(audio_decoder);
    if(!audio_decoder_ctx){
        LOGD("avcodec_alloc_context3 error\n")
        return;
    }
    ret = avcodec_parameters_to_context(audio_decoder_ctx,fmt_ctx->streams[audio_idx]->codecpar);
    if(ret != 0){
        char buf[1024] = {0};
        av_strerror(ret,buf,1024);
        LOGD("avcodec_parameters_to_context ret: %s\n", buf)
        return;
    }

    AVCodec * video_decoder = nullptr;
    video_decoder = avcodec_find_decoder(fmt_ctx->streams[video_idx]->codecpar->codec_id);
    if(!video_decoder){
        LOGD("avcodec_find_decoder error\n")
        return;
    }
    AVCodecContext * video_decoder_ctx = nullptr;
    video_decoder_ctx = avcodec_alloc_context3(video_decoder);
    if(!video_decoder_ctx){
        LOGD("avcodec_alloc_context3 error\n")
        return;
    }
    ret = avcodec_parameters_to_context(video_decoder_ctx,fmt_ctx->streams[video_idx]->codecpar);
    if(ret != 0){
        char buf[1024] = {0};
        av_strerror(ret,buf,1024);
        LOGD("avcodec_parameters_to_context ret: %s\n", buf)
        return;
    }

    //打开解码器上下文
    ret = avcodec_open2(audio_decoder_ctx,audio_decoder, nullptr);
    if(ret != 0){
        char buf[1024] = {0};
        av_strerror(ret,buf,1024);
        LOGD("avcodec_open2 ret: %s\n", buf)
        return;
    }
    ret = avcodec_open2(video_decoder_ctx,video_decoder, nullptr);
    if(ret != 0){
        char buf[1024] = {0};
        av_strerror(ret,buf,1024);
        LOGD("avcodec_open2 ret: %s\n", buf)
        return;
    }

    audio_channel = new AudioChannel(audio_idx,audio_decoder_ctx);
    video_channel = new VideoChannel(video_idx,video_decoder_ctx);
    video_channel->set_renderCallback(renderCallback);  //native 传给player 然后再给videochannel进行使用。
    if (!audio_channel && !video_channel){              //videochannel会把数据进行回调给native。
        return;
    }
    if (helper){
        helper->onPrepared(THREAD_CHILD);
    }
}

void Player::prepared() {

    pthread_create(&pid_prepare,NULL,task_prepare,this);

}
void * task_start(void * args){
    Player * player = static_cast<Player *>(args);
    LOGD("task_start\n");

    player->task_start_();

    return 0;
}
void Player::task_start_() {
    while (isPlay){
        AVPacket *pkt = av_packet_alloc();
        int ret = av_read_frame(fmt_ctx,pkt);
        if (!ret){
            if (video_channel && pkt->stream_index == video_channel->stream_index){
                video_channel->packets.insertToQueue(pkt);
            } else if (audio_channel && pkt->stream_index == audio_channel->stream_index){
                audio_channel->packets.insertToQueue(pkt);
            }
        } else if (ret == AVERROR_EOF){
            //读完了 但不表示播放完
        } else{ //出现错误
            break;
        }
    }
    isPlay = false;
    video_channel->stop();
    audio_channel->stop();
}

void Player::start() {
    isPlay = true;

    if (video_channel){
        video_channel->start();
    }
    if (audio_channel){
        audio_channel->start();
    }
    pthread_create(&pid_start,NULL,task_start,this);
}

void Player::set_renderCallback(RenderCallback renderCallback) {
    this->renderCallback = renderCallback;
}


