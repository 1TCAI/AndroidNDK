//
// Created by 96302 on 2024-04-23.
//

#ifndef OPENGLTEST_FRAMEBUFFEROBJECT_H
#define OPENGLTEST_FRAMEBUFFEROBJECT_H

#include "GLCommonDef.h"
class FrameBufferObject {
public:
    FrameBufferObject();

    ~FrameBufferObject();
    void Bind();
    void Release();
    GLuint GetTextureID();

    void GenerateFBOWithSize(int width, int height);

private:
    GLuint      m_fboId;
    GLuint      m_renderBufferId;
    GLuint      m_textureId;
};


#endif //OPENGLTEST_FRAMEBUFFEROBJECT_H
