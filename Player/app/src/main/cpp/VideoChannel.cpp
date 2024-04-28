//
// Created by 96302 on 2024-04-07.
//

#include "VideoChannel.h"


VideoChannel::VideoChannel(int stream_index, AVCodecContext *codecContext)
:BaseChannel(stream_index,codecContext)
{

}

VideoChannel::~VideoChannel() {

}

void VideoChannel::stop() {

}
void * task_video_decode(void * args){
    VideoChannel * video_channel = static_cast<VideoChannel *>(args);
    video_channel->video_decode();
}

void * task_video_play(void * args){
    VideoChannel * video_channel = static_cast<VideoChannel *>(args);
    video_channel->video_play();
}
void VideoChannel::start() {
    isPlay = true;
    packets.setWork(1);
    frames.setWork(1);

    pthread_create(&pid_vide_decode,NULL,task_video_decode,this);
    pthread_create(&pid_vide_play,NULL,task_video_play,this);
}

void VideoChannel::video_decode() {
    AVPacket * pkt = nullptr;
    while (isPlay){
        int ret = packets.getQueueAndDel(pkt);  //这里返回1 是成功
        //由于getQueueAndDel是阻塞式函数，所以可能阻塞过程中 isPlay被修改，所以需要重新判断
        if (!isPlay){
            break;
        }
        if (!ret){
            continue;
        }
        ret = avcodec_send_packet(codecContext,pkt);
        if (ret != 0){
            break;
        }
        releaseAVPacket(&pkt); //ffmpeg对缓冲区有拷贝 可以大胆释放
        AVFrame * frame = av_frame_alloc();
        avcodec_receive_frame(codecContext,frame);
        if (ret != 0){
            if (ret == AVERROR(EAGAIN)){  //I帧一定成功。这里again 只可能是B帧，因为P帧没出来
                continue;
            } else{
                break;
            }
        }
        frames.insertToQueue(frame);
    }
    releaseAVPacket(&pkt);
}

void VideoChannel::video_play() {

    AVFrame * frame = nullptr;
    uint8_t * dst_data[4];
    int dst_linesize[4];
    av_image_alloc(dst_data,dst_linesize,codecContext->width,codecContext->height
                    ,AV_PIX_FMT_RGBA,1);
    struct SwsContext *sws_ctx = sws_getContext(codecContext->width,codecContext->height,codecContext->pix_fmt
                                                ,codecContext->height,codecContext->pix_fmt,AV_PIX_FMT_RGBA,SWS_BILINEAR
                                                , nullptr, nullptr, nullptr);

    while (isPlay){

        int ret = frames.getQueueAndDel(frame);  //这里返回1 是成功
        //由于getQueueAndDel是阻塞式函数，所以可能阻塞过程中 isPlay被修改，所以需要重新判断
        if (!isPlay){
            break;
        }
        if (!ret){      //
            continue;
        }

//        struct SwsContext *c, const uint8_t *const srcSlice[],
//        const int srcStride[], int srcSliceY, int srcSliceH,
//        uint8_t *const dst[], const int dstStride[]

        //格式转换
        sws_scale(sws_ctx,frame->data,frame->linesize,
                0,codecContext->height,dst_data,dst_linesize);
        //ffmpeg 编解码codec源代码是按照8字节除法
        //ANatvieWindows 按照64除法，所以有时候拿到ANatvieWindows是有余数的，会强制向上补充对齐。
        //所以在排列的时候就需要每行留空值，否则会有视频花屏。
        // 640 argb 640*4/8 每行可以整除， /64 也可以。 但是如果宽是426 *4 / 64 就不能整除。

        //surface和这个类没有关系，所以使用函数指针回调将 宽高数据 回调给native-lib 进行渲染。
        //数组被传递会退化为指针，所以需要告诉数组长度。
        renderCallback(dst_data[0],dst_linesize[0],codecContext->width,codecContext->height);
        //函数指针必须先定义再赋值。不能一句写完，不同平台可能崩溃。
        releaseAVFrame(&frame);
    }
    releaseAVFrame(&frame); //avframe的释放必须慎重，因为他最大，一个frame可能就几百兆。
    isPlay = false;
    av_free(dst_data[0]);
    sws_freeContext(sws_ctx);
}

void VideoChannel::set_renderCallback(RenderCallback renderCallback) {
    this->renderCallback = renderCallback;
}

