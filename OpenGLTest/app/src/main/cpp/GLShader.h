//
// Created by 96302 on 2024-04-23.
//

#ifndef OPENGLTEST_GLSHADER_H
#define OPENGLTEST_GLSHADER_H

#include "GLCommonDef.h"
class GLShader {


public:
    GLShader();

    ~GLShader();
    void Bind();
    void Release();

    void InitShadersFromFile(AAssetManager*  pManager, const char* vShader,const char* fshader);

    void DisableAttributeArray(const char *name);
    void EnableAttributeArray(const char *name);
    void SetAttributeBuffer(const char* name,GLenum type, const void *values, int tupleSize, int stride = 0);
    void EnableAttributeArray(int location);

    void SetUniformValue(const char* name, int iValue);
    void SetUniformValue(const char* name, GLfloat fValue);
    void SetUniformValue(const char* name, glm::vec3 vecValue);
    void SetUniformValue(const char* name, glm::mat4 matValue);
    void SetAttributeBuffer(int location,GLenum type, const void *values, int tupleSize, int stride = 0);
private:
    int compileShader(AAssetManager*  m_pAssetManager,const char* sPath, GLint sType);
private:
    GLuint m_shaderProgram;
};


#endif //OPENGLTEST_GLSHADER_H
