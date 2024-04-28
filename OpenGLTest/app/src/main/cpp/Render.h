//
// Created by 96302 on 2024-04-23.
//

#ifndef OPENGLTEST_RENDER_H
#define OPENGLTEST_RENDER_H

#include "GLCommonDef.h"
#include "Buffer.h"
#include "GLTexture.h"
#include "GLShader.h"
#include "FrameBufferObject.h"
#include "GLVAO.h"
class Render {


public:
    Render();

    ~Render();

    void InitGL();
    void PaintGL();
    void ResizeGL(int w, int h);

    void SetupAssetManager(AAssetManager *pManager);
private:
    void loadTextureResources(AAssetManager *pManager);
    void loadShaderResources(AAssetManager *pManager);
    void renderTriangle();
    void setupFrameBufferObject();
    void setupRenderingObject();
    void renderObject();

private:
    float m_angle =0.0f;
    GLuint m_texID[6];

    AAssetManager*      m_pAssetManager;
    GLVAO*      m_pVAO;
    Buffer*     m_pVBO;
    Buffer*     m_pEBO;
    FrameBufferObject*  m_pFBO;

    GLShader*       m_pOpenGLShader;

};


#endif //OPENGLTEST_RENDER_H
