//
// Created by 96302 on 2024-04-23.
//

#ifndef OPENGLTEST_GLTEXTURE_H
#define OPENGLTEST_GLTEXTURE_H

#include "GLCommonDef.h"
#include "GLImage.h"
class GLTexture {
public:
    GLTexture();

    ~GLTexture();
    GLuint  GetTextureID();
    GLuint  CreateGLTextureFromFile(AAssetManager *assetManager, const char* fileName);

private:
    GLuint  generateTexture(AAssetManager *assetManager, const char* fileName);
    GLuint  createOpenGLTexture(GLImage* pImg);

private:
    GLuint m_texID;
};


#endif //OPENGLTEST_GLTEXTURE_H
