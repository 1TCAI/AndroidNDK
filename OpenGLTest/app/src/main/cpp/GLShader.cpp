//
// Created by 96302 on 2024-04-23.
//

#include "GLShader.h"

GLShader::GLShader() {}

GLShader::~GLShader() {

}
void GLShader::InitShadersFromFile(AAssetManager* pManager,const char* vPath, const char* fPath)
{
    GLuint  vertexId = 0;
    GLuint  fragId = 0;

    vertexId   = compileShader(pManager,vPath ,GL_VERTEX_SHADER);
    fragId     = compileShader(pManager,fPath ,GL_FRAGMENT_SHADER);

    char           message[512];
    int            status = 0;

    m_shaderProgram = glCreateProgram();    //创建一个程序
    if (vertexId != -1)
    {
        glAttachShader(m_shaderProgram, vertexId);  //把v加入
    }
    if (fragId != -1)
    {
        glAttachShader(m_shaderProgram, fragId);  //把f加入
    }

    glLinkProgram(m_shaderProgram);

    glGetProgramiv(m_shaderProgram, GL_LINK_STATUS, &status);
    if (!status)
    {
        glGetProgramInfoLog(m_shaderProgram, 512, NULL, message);
        LOGF("Get shaderProgram failed: %s",message);
    }

    LOGD("ShaderProgram sucess !!!\n");

    glDeleteShader(vertexId);
    glDeleteShader(fragId);

}
int GLShader::compileShader(AAssetManager*  pManager,const char* fName, GLint sType)
{

    AAsset* file = AAssetManager_open(pManager,fName, AASSET_MODE_BUFFER);
    size_t shaderSize = AAsset_getLength(file);

    char* sContentBuff = (char*)malloc(shaderSize);
    AAsset_read(file, sContentBuff, shaderSize);
    LOGD("SHADERS: %s",sContentBuff);
    unsigned int   shaderID = 0;
    char           message[512]={0};
    int            status = 0;

    shaderID = glCreateShader(sType);
    glShaderSource(shaderID, 1, &sContentBuff, (const GLint *)&shaderSize);
    glCompileShader(shaderID);

    glGetShaderiv(shaderID, GL_COMPILE_STATUS, &status);
    if (!status)
    {
        glGetShaderInfoLog(shaderID, 512, NULL, message);
        LOGF("Compile Shader Status failed: %s",message);
    }

    if(sContentBuff != NULL){
        free(sContentBuff);
        sContentBuff = NULL;
    }

    AAsset_close(file);
    return shaderID;
}


void GLShader::Bind()
{
    glUseProgram(m_shaderProgram);
}

void GLShader::Release()
{
    glUseProgram(0);
}

void GLShader::SetUniformValue(const char* name, int iValue)
{
    glUniform1i(glGetUniformLocation(m_shaderProgram, name), iValue);

}

void GLShader::SetUniformValue(const char* name, GLfloat fValue)
{
    glUniform1f(glGetUniformLocation(m_shaderProgram, name), fValue);
}

void GLShader::SetUniformValue(const char* name, glm::vec3 vec3Value)
{
    glUniform3fv(glGetUniformLocation(m_shaderProgram, name), 1, glm::value_ptr(vec3Value));
}

void GLShader::SetUniformValue(const char* name, glm::mat4 matValue)
{
    glUniformMatrix4fv(glGetUniformLocation(m_shaderProgram, name) , 1 , GL_FALSE , glm::value_ptr(matValue));
}

void GLShader::EnableAttributeArray(const char *name)
{
    GLuint location = glGetAttribLocation(m_shaderProgram, name);
    glEnableVertexAttribArray(location);
}

void GLShader::DisableAttributeArray(const char *name)
{
    GLuint location = glGetAttribLocation(m_shaderProgram, name);
    glDisableVertexAttribArray(location);
}

void GLShader::SetAttributeBuffer(const char* name,GLenum type, const void *values, int tupleSize, int stride)
{
    GLuint location = glGetAttribLocation(m_shaderProgram, name);
    glVertexAttribPointer(location,tupleSize,type,GL_FALSE,stride,values);
}

void GLShader::SetAttributeBuffer(int location, GLenum type, const void *values, int tupleSize,
                                  int stride) {
    glVertexAttribPointer(location,tupleSize,type,GL_FALSE,stride,values);

}

void GLShader::EnableAttributeArray(int location) {
    glEnableVertexAttribArray(location);

}
