package com.example.player;

import android.media.MediaPlayer;
import android.view.Surface;
import android.view.SurfaceHolder;
import android.view.SurfaceView;

import androidx.annotation.NonNull;

public class Player implements SurfaceHolder.Callback {

    public Player(){
    }

    static {
        System.loadLibrary("native-lib");
    }

    private String dataSource;
    public void setDataSource(String dataSource){
        this.dataSource = dataSource;
    }

    public void prepare(){
        prepareNative(dataSource);
    }

    public void start(){
        startNative();
    }

    public void stop(){
        stopNative();
    }

    public void release(){
        releaseNative();
    }


    private OnPreparedListener onPreparedListener;
    public void onPrepared(){                       //c++ prepare准备好了 会回调这个函数
        if (onPreparedListener != null){
            onPreparedListener.onPrepared();        //然后进入这个函数
        }
    }



    public interface OnPreparedListener {
        void onPrepared();                              //最后这个函数会进入mainactivity
    }
    public void setOnPreparedListener(OnPreparedListener onPreparedListener){
        this.onPreparedListener = onPreparedListener;
    }


    public native void prepareNative(String dataSource);
    public native void startNative();
    public native void stopNative();
    public native void releaseNative();
    public native void setSurfaceNative(Surface surface);

    @Override
    public void surfaceCreated(@NonNull SurfaceHolder holder) {

    }

    @Override
    public void surfaceChanged(@NonNull SurfaceHolder holder, int format, int width, int height) {
        setSurfaceNative(holder.getSurface());
    }

    @Override
    public void surfaceDestroyed(@NonNull SurfaceHolder holder) {

    }
    private SurfaceHolder surfaceHolder; // TODO 第三节课新增

    public void setSurfaceView(SurfaceView surfaceView) {
        if (this.surfaceHolder != null) {
            surfaceHolder.removeCallback(this); // 清除上一次的
        }
        surfaceHolder = surfaceView.getHolder();
        surfaceHolder.addCallback(this); // 监听
    }
}
