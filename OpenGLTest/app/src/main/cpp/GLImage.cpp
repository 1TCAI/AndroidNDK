//
// Created by 96302 on 2024-04-23.
//

#include "GLImage.h"

//使用stbimage 必须定义这个。头文件和这个宏定义必须在cpp文件中。不能在h文件中
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


GLImage::GLImage():m_width(0),m_height(0),m_type(0),m_pImgData(NULL)
{}

GLImage::~GLImage() {
    if(m_pImgData)
    {
        free(m_pImgData);
    }
}


void GLImage::ReadFromFile(uint8_t * fileName)
{
    int			type = 0;
    int			width = 0;
    int			height = 0;

    //stbi_set_flip_vertically_on_load(true);

    uint8_t* picData = stbi_load((char const *)fileName, &width, &height, &type, STBI_rgb_alpha);

    int imgSize = width * height * 4;

    if(imgSize >0 && picData != nullptr){

        m_pImgData =(uint8_t*) malloc(imgSize);
        memcpy(m_pImgData,picData,imgSize);

        m_width =width;
        m_height =height;
        m_type = type;
    }

    stbi_image_free(picData);

}

void GLImage::ReadFromBuffer(uint8_t* dataBuff,int length)
{
    int			type = 0;
    int			width = 0;
    int			height = 0;

    stbi_set_flip_vertically_on_load(true);

    uint8_t* picData = stbi_load_from_memory((uint8_t const *)dataBuff, length, &width, &height, &type, 0);

    int imgSize = width * height * 4;

    if(imgSize >0 && picData != nullptr){

        m_pImgData =(uint8_t*) malloc(imgSize);
        memcpy(m_pImgData,picData,imgSize);

        m_width =width;
        m_height =height;
        m_type = type;
    }

    stbi_image_free(picData);

}


int GLImage::GetWidth() const
{
    return m_width;
}

int GLImage::GetHeight() const
{
    return m_height;
}

int GLImage::GetType() const
{
    return m_type;
}

uint8_t * GLImage::GetData() const
{
    return m_pImgData;
}