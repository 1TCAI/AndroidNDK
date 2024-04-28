//
// Created by 96302 on 2024-04-27.
//

#ifndef OPENGLTEST_GLVAO_H
#define OPENGLTEST_GLVAO_H

#include "GLCommonDef.h"
class GLVAO {

public:
    GLVAO();

    ~GLVAO();

    void Bind();
    void Release();
    void Create();


private:

    GLuint  m_vaoId;
};


#endif //OPENGLTEST_GLVAO_H
