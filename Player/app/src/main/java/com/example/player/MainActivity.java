package com.example.player;

import androidx.annotation.NonNull;
import androidx.appcompat.app.AppCompatActivity;
import androidx.core.app.ActivityCompat;

import android.Manifest;
import android.app.Activity;
import android.content.pm.PackageManager;
import android.os.Bundle;
import android.os.Environment;
import android.util.Log;
import android.view.Surface;
import android.view.SurfaceView;
import android.widget.TextView;
import android.widget.Toast;

import java.io.File;

public class MainActivity extends AppCompatActivity {

    private Player player;
    private Permission permission;
    private SurfaceView surfaceView;
    @Override
    public void onRequestPermissionsResult(int requestCode, @NonNull String[] permissions, @NonNull int[] grantResults) {
        super.onRequestPermissionsResult(requestCode, permissions, grantResults);
        if (requestCode == permission.REQUESE_CODE){
            for (int i = 0 ; i < grantResults.length; i++){
                if (grantResults[i] != PackageManager.PERMISSION_GRANTED){
                    Log.e("permission", permissions[i] + " not");
                    Log.e("permission", grantResults[i] + " not");
                }else {
                    Log.e("permission", permissions[i] );
                    Log.e("permission", grantResults[i] + "");
                }
            }
        }
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        surfaceView = findViewById(R.id.surfaceView);



        permission = new Permission();
        permission.checkPermission(this);

        player = new Player();
        player.setSurfaceView(surfaceView);

        String filePath ="/storage/emulated/0/Movies/demo.mp4";
        String absolutePath = new File(filePath).getAbsolutePath();

        player.setDataSource(absolutePath);

        System.out.println("Absolute Path: " + absolutePath);
        player.setOnPreparedListener(new Player.OnPreparedListener() {
            @Override
            public void onPrepared() {
                runOnUiThread(new Runnable() {
                    @Override
                    public void run() {
                        Toast.makeText(MainActivity.this, "准备成功，即将开始播放", Toast.LENGTH_SHORT).show();
                    }
                });
                player.start(); // 调用 C++ 开始播放
            }
        });
    }



    @Override // ActivityThread.java Handler
    protected void onResume() { // 我们的准备工作：触发
        super.onResume();
        player.prepare();
    }

    @Override
    protected void onStop() {
        super.onStop();
        player.stop();
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        player.release();
    }



    /**
     * A native method that is implemented by the 'native-lib' native library,
     * which is packaged with this application.
     */
    public native String stringFromJNI();




}
