//
// Created by 96302 on 2024-04-23.
//

#include "Render.h"
Render::Render():m_pAssetManager(NULL)
{
    m_pVBO = new Buffer(Buffer::VertexBuffer,Buffer::StaticDraw);
    m_pEBO = new Buffer(Buffer::IndexBuffer,Buffer::StaticDraw);
    m_pVAO = new GLVAO();

    m_pOpenGLShader = new GLShader();

}

Render::~Render() {
    glDeleteTextures(6,m_texID);
    delete m_pVBO;
    m_pVBO = NULL;
    delete m_pEBO;
    m_pEBO = NULL;


    glDeleteTextures(6,m_texID);

}

void Render::InitGL() {
    glClearColor(0.0,0.0,0.0,1.0);
    glClearDepthf(1.0);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    if(m_pAssetManager != NULL){
        loadTextureResources(m_pAssetManager);
        loadShaderResources(m_pAssetManager);

    }
    setupRenderingObject();
//    setupFrameBufferObject();


}

void Render::PaintGL() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//    renderTriangle();
    renderObject();

}

void Render::ResizeGL(int w, int h) {
    glViewport(0,0,w,h);
}

void Render::SetupAssetManager(AAssetManager *pManager) {
    if(pManager == NULL){
        return;
    }
    m_pAssetManager = pManager;
}

void Render::loadTextureResources(AAssetManager *pManager) {
    for(int i=0; i<6; i++){

        char nameBuff[6];
        memset(nameBuff,0,sizeof(nameBuff));
        sprintf(nameBuff,"%d.png",i+1);
        nameBuff[5]='\0';
        LOGD("Image Name:%s",nameBuff);
        GLTexture glTexture;
        m_texID[i] = glTexture.CreateGLTextureFromFile(pManager,nameBuff);
    }
}

void Render::loadShaderResources(AAssetManager *pManager) {
    m_pOpenGLShader->InitShadersFromFile(pManager,"vertex.vert","fragment.frag");
//    m_pOpenGLShader->InitShadersFromFile(pManager,"cubev.glsl","cubef.glsl");

}



void Render::renderTriangle() {
    Float7 triangleVert[] ={
            {0,       0.5,    -1,  1,  0,  0,1.0},
            {-0.5,   -0.5,    -1,  0,  1,  0,1.0},
            {0.5,    -0.5,    -1,  0,  0,  1,1.0},
    };


    m_angle += 0.01f;

    glm::mat4x4  cubeMat;
    glm::mat4x4  cubeTransMat = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -3));
    glm::mat4x4  cubeRotMat = glm::rotate(glm::mat4(1.0f),m_angle,glm::vec3(1.0f, 1.0f, 1.0) );
    glm::mat4x4  cubeScaleMat = glm::scale(glm::mat4(1.0f),glm::vec3(0.3f, 0.2f, 0.3) );

    glm::mat4 projMat = glm::perspective(glm::radians(60.0f), (float)9/(float)18, 0.1f, 1000.0f);
    cubeMat = projMat* cubeTransMat ;
    m_pOpenGLShader->Bind();

    m_pOpenGLShader->SetUniformValue("u_mat",cubeMat);

    m_pOpenGLShader->EnableAttributeArray("a_position");
    m_pOpenGLShader->EnableAttributeArray("a_color");

    m_pOpenGLShader->SetAttributeBuffer("a_position",GL_FLOAT,triangleVert,3,sizeof(Float7));
    m_pOpenGLShader->SetAttributeBuffer("a_color",GL_FLOAT,&triangleVert[0].r,4,sizeof(Float7));

    glDrawArrays(GL_TRIANGLES,0,3);

    m_pOpenGLShader->DisableAttributeArray("a_position");
    m_pOpenGLShader->DisableAttributeArray("a_color");

    m_pOpenGLShader->Release();
}

void Render::setupFrameBufferObject() {
    int viewport[4]= {0};
    glGetIntegerv( GL_VIEWPORT, viewport );
    int sWidth = viewport[2];
    int sHeight = viewport[3];
    //LOGD("VIEWPORT: %d %d",viewport[2],viewport[3]);
    if(sWidth > 0 && sHeight >0){
        m_pFBO->GenerateFBOWithSize(sWidth,sHeight);
    }
}

void Render::setupRenderingObject() {
    if(m_pOpenGLShader == NULL){
        return;
    }

    const Float5 cubeVertexs[]  = {
            {  -1.0,-1.0, 1.0 ,  0.0, 0.0 },
            {  -1.0, 1.0, 1.0 ,  0.0, 1.0 },
            {  1.0, -1.0,  1.0 , 1.0, 0.0 },
            {   1.0, 1.0, 1.0 ,  1.0, 1.0 },

            {   1.0,-1.0, -1.0,   0,  0  },
            {   1.0, 1.0, -1.0,   0,  1  },
            {   -1.0,-1.0, -1.0,   1,  0 },
            {   -1.0, 1.0, -1.0,   1,  1 },


            {   -1.0, -1.0, -1.0,  0,  0 },
            {   -1.0, 1.0, -1.0,   0,  1 },
            {   -1.0, -1.0,  1.0,  1,  0 },
            {   -1.0, 1.0, 1.0,    1,  1 },

            {   1.0,-1.0,  1.0,    0,  0 },
            {   1.0, 1.0,  1.0,    0,  1 },
            {   1.0, -1.0,  -1.0,  1,  0 },
            {   1.0, 1.0, -1.0,    1,  1 },

            {   -1.0, 1.0,  1.0,   0,  0 },
            {   -1.0, 1.0,  -1.0,  0,  1 },
            {   1.0, 1.0, 1.0,     1,  0 },
            {   1.0, 1.0, -1.0,    1,  1 },

            {   -1.0, -1.0, -1.0,  0,  0 },
            {   -1.0, -1.0, 1.0,   0,  1 },
            {   1.0, -1.0, -1.0,   1,  0 },
            {   1.0, -1.0, 1.0,    1,  1 }
    };

    const short cubeIndexs[]= {
            0, 1, 2,  2, 1, 3,
            4, 5, 6,  6, 5, 7,
            8, 9, 10, 10, 9,11,
            12,13,14, 14,13,15,
            16,17,18, 18,17,19,
            20,21,22, 22,21,23,
    };

    m_pVAO->Create();
    m_pVAO->Bind();

    m_pVBO->Create();
    m_pVBO->Bind();
    m_pVBO->SetBufferData(cubeVertexs,sizeof(cubeVertexs));


    m_pEBO->Create();
    m_pEBO->Bind();
    m_pEBO->SetBufferData(cubeIndexs,sizeof(cubeIndexs));

    int offset = 0;

    m_pOpenGLShader->SetAttributeBuffer(0,GL_FLOAT, (void *)offset, 3, sizeof(Float5));
    m_pOpenGLShader->EnableAttributeArray(0);

    offset += 3 * sizeof(float);

    m_pOpenGLShader->SetAttributeBuffer(1,GL_FLOAT, (void *)offset, 2, sizeof(Float5));
    m_pOpenGLShader->EnableAttributeArray(1);

    m_pVAO->Release();
    m_pVBO->Release();
    m_pEBO->Release();
}

void Render::renderObject() {
    m_angle += 0.05f;

    glm::mat4x4  objectMat;
    glm::mat4x4  objectTransMat = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -3));
    glm::mat4x4  objectRotMat = glm::rotate(glm::mat4(1.0f),m_angle,glm::vec3(1.0f, 1.0f, 1.0) );
    glm::mat4x4  objectScaleMat = glm::scale(glm::mat4(1.0f),glm::vec3(0.3f, 0.2f, 0.3) );

    glm::mat4 projMat = glm::perspective(glm::radians(60.0f), (float)9/(float)18, 0.1f, 1000.0f);
    objectMat = projMat* objectTransMat * objectScaleMat* objectRotMat ;

    m_pOpenGLShader->Bind();

    m_pOpenGLShader->SetUniformValue("u_mat",objectMat);


    m_pVAO->Bind();

    for(int i=0; i<6; i++){

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D,m_texID[i]);
        int offset = i * 6 * sizeof(unsigned  short);
        glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_SHORT,(void *)offset);
        glBindTexture(GL_TEXTURE_2D,0);
    }

    m_pOpenGLShader->Release();
    m_pVAO->Release();
}

