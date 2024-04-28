//
// Created by 96302 on 2024-04-23.
//

#include "Buffer.h"

Buffer::Buffer(Type type, UsagePattern usage):m_bufferType(type),m_usage(usage),m_buffSize(0)
{

}

Buffer::~Buffer() {
    glDeleteBuffers(1,&m_buffID);
}
void Buffer::Create()
{
    glGenBuffers(1, &m_buffID);
}

void Buffer::Bind()
{
    glBindBuffer(m_bufferType, m_buffID);
    LOGD("BUFF ID: %d",m_buffID);
}

void Buffer::Release()
{
    glBindBuffer(m_bufferType, 0);
}

void Buffer::SetBufferData(const GLvoid *data, GLsizeiptr size)
{
    if (size > m_buffSize) {
        m_buffSize = size;
        glBufferData(m_bufferType, size, data, m_usage);
    } else {
        glBufferSubData(m_bufferType, 0, size, data);
    }
}