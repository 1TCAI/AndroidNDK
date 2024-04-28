//
// Created by 96302 on 2024-04-27.
//

#include "GLVAO.h"

GLVAO::GLVAO() {}

GLVAO::~GLVAO() {
    glDeleteVertexArrays(1,&m_vaoId);

}

void GLVAO::Bind() {
    glBindVertexArray(m_vaoId);

}

void GLVAO::Release() {
    glBindVertexArray(0);

}

void GLVAO::Create() {
    glGenVertexArrays(1, &m_vaoId); //这个最好不要在构造函数中。

}
