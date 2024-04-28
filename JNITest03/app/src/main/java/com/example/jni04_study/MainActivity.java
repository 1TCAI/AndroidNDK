package com.example.jni04_study;

import androidx.appcompat.app.AppCompatActivity;
import androidx.appcompat.app.AlertDialog;

import android.os.Bundle;
import android.os.Looper;
import android.util.Log;
import android.view.View;
import android.widget.TextView;
import android.widget.Toast;

import com.example.jni04_study.databinding.ActivityMainBinding;

public class MainActivity extends AppCompatActivity {

    static {
        System.loadLibrary("jni04_study");
    }

    private ActivityMainBinding binding;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        binding = ActivityMainBinding.inflate(getLayoutInflater());
        setContentView(binding.getRoot());


    }

    public native void dynamicMethod1();
    public native int dynamicMethod2(String str);
    public native String stringFromJNI();

    // 第二部分 JNI线程 区域 =====================================================
    public native void naitveThread(); // Java层 调用 Native层 的函数，完成JNI线程
    public native void closeThread(); // 释放全局引用

    /**
     * TODO 下面时点击事件区域
     */
    // 动态注册 的 点击事件1
    public void dynamic01(View view) {
        dynamicMethod1();
    }

    // 动态注册 的 点击事件2
    public void dynamic02(View view) {
        int result = dynamicMethod2("神照功");
        Toast.makeText(this, "result:" + result, Toast.LENGTH_SHORT).show();
    }

    // 第二部分 JNI线程 的 点击事件 =====================================================
    public void nativeCallJava(View view) {
        naitveThread();
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        closeThread(); // 为了释放，全局成员
    }


    /**
     * TODO 下面是 被native代码调用的 Java方法
     * 第二部分 JNI线程
     */
    public void updateActivityUI() {
        if (Looper.getMainLooper() == Looper.myLooper()) { // TODO C++ 用主线程调用到此函数 ---->  主线程
            new AlertDialog.Builder(MainActivity.this)
                    .setTitle("UI")
                    .setMessage("updateActivityUI Activity UI ...")
                    .setPositiveButton("老夫知道了", null)
                    .show();
        } else {  // TODO  C++ 用异步线程调用到此函数 ---->  异步线程
            Log.d("Derry", "updateActivityUI 所属于子线程，只能打印日志了..");

            runOnUiThread(new Runnable() { // 哪怕是异步线程  UI操作 正常下去 runOnUiThread
                @Override
                public void run() {

                    // 可以在子线程里面 操作UI
                    new AlertDialog.Builder(MainActivity.this)
                            .setTitle("updateActivityUI")
                            .setMessage("所属于子线程，只能打印日志了..")
                            .setPositiveButton("老夫知道了", null)
                            .show();
                }
            });
        }
    }

}