package com.example.opengltest;

import androidx.appcompat.app.AppCompatActivity;

import android.content.res.AssetManager;
import android.opengl.GLSurfaceView;
import android.os.Bundle;
import android.widget.TextView;

public class MainActivity extends AppCompatActivity {


    private GLSurfaceView m_glSurfaceView;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        m_glSurfaceView = new GLSurfaceView(this);
        m_glSurfaceView.setEGLContextClientVersion(3);//如果使用3.0 需要加这一句话。
        m_glSurfaceView.setRenderer(new CCGLRender(this));
        AssetManager assetManager = this.getAssets();
        setContentView(m_glSurfaceView);
    }
}
