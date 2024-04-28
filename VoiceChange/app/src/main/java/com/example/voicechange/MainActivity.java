package com.example.voicechange;

import android.os.Bundle;
import android.view.View;
import android.widget.Toast;

import androidx.appcompat.app.AppCompatActivity;

import org.fmod.FMOD;

public class MainActivity extends AppCompatActivity {

    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("native-lib");
    }

    private static final int MODE_NORMAL = 0;       // 正常
    private static final int MODE_LUOLI = 1;        // 萝莉
    private static final int MODE_DASHU = 2;        // 大叔
    private static final int MODE_JINGSONG = 3;     // 惊悚
    private static final int MODE_GAOGUAI = 4;      // 搞怪
    private static final int MODE_KONGLING = 5;     // 空灵

    // 播放的路径
    private String path;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        path = "file:///android_asset/derry.mp3";
        FMOD.init(this); // jar包
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        FMOD.close(); // jar包
    }
    private void playerEnd(String msg) {    //告诉java 结束了
        Toast.makeText(this, "" +msg, Toast.LENGTH_SHORT).show();
    }
    // 六个 点击事件
    public void onFix(View view) {
        switch (view.getId()) {
            case R.id.btn_normal:
                voiceChangeNative(MODE_NORMAL, path);
                break;
            case R.id.btn_luoli:
                voiceChangeNative(MODE_LUOLI, path);
                break;
            case R.id.btn_dashu:
                voiceChangeNative(MODE_DASHU, path);
                break;
            case R.id.btn_jingsong:
                voiceChangeNative(MODE_JINGSONG, path);
                break;
            case R.id.btn_gaoguai:
                voiceChangeNative(MODE_GAOGUAI, path);
                break;
            case R.id.btn_kongling:
                voiceChangeNative(MODE_KONGLING, path);
                break;
        }
    }



    /**
     * A native method that is implemented by the 'native-lib' native library,
     * which is packaged with this application.
     */
    public native String stringFromJNI();
    private native void voiceChangeNative(int mode, String path);

}
