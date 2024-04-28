//
// Created by 96302 on 2024-04-23.
//

#ifndef OPENGLTEST_GLIMAGE_H
#define OPENGLTEST_GLIMAGE_H


#include <cstdlib>  //uint8_t
class GLImage {

public:

    GLImage();

    ~GLImage();
    int         GetWidth() const;
    int         GetHeight() const;
    int         GetType() const ;
    uint8_t *       GetData() const;

    void        ReadFromFile(uint8_t* fileName);
    void        ReadFromBuffer(uint8_t* dataBuff,int length);

private:
    int         m_width;
    int         m_height;
    int         m_type;
    uint8_t*       m_pImgData;

};


#endif //OPENGLTEST_GLIMAGE_H
