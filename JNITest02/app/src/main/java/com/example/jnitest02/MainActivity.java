package com.example.jnitest02;

import androidx.appcompat.app.AlertDialog;
import androidx.appcompat.app.AppCompatActivity;

import android.content.Intent;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.TextView;
import android.widget.Toast;
import android.os.Looper;

public class MainActivity extends AppCompatActivity {

    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("native-lib");
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);


    }

    /**
     * A native method that is implemented by the 'native-lib' native library,
     * which is packaged with this application.
     */
    public native String stringFromJNI();
    public native void dynamicMethod1();
    public native int dynamicMethod2(String str);
    public void dynamic01(View view) {
        dynamicMethod1();
    }

    // 动态注册 的 点击事件2
    public void dynamic02(View view) {
        int result = dynamicMethod2("神照功");
        Toast.makeText(this, "result:" + result, Toast.LENGTH_SHORT).show();
    }
    //----------------------------------------------------
    //JNI 线程
    public native void naitveThread(); // Java层 调用 Native层 的函数，完成JNI线程
    public native void closeThread(); // 释放全局引用
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



    //jnivm 属于全局的
    //jnienv是分线程的


    //1.JavaVM全局,绑定当前进程,只有一个地址
    //2．JNIEnv线程绑定，绑定主线程,绑定子线程
    //3. jobject谁调用JNI函数,谁的实例会给jobject

// JNIEnV *env不能跨越线程，否则奔溃，他可以跨越函数【解决方式:使用全局的JavaVN附加当前异步线程得到权限env操作】
// jobject thiz不能跨越线程，否则奔溃，不能跨越函数，否则奔溃【解决方式:默认是局部引用，提升全局引用，可解决此问题
// JavaVM能够跨越线程,能够跨越函数

}
