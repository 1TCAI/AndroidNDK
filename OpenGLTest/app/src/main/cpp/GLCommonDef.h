//
// Created by 96302 on 2024-04-23.
//

#ifndef OPENGLTEST_GLCOMMONDEF_H
#define OPENGLTEST_GLCOMMONDEF_H

#include "NDKLog.h"

//#include <GLES/gl.h>
//#include <GLES2/gl2.h>
#include <GLES3/gl3.h>

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>


#include <assert.h>
#include <android/asset_manager_jni.h>
#include <android/asset_manager.h>

struct  Float5{
    float x;
    float y;
    float z;
    float u;
    float v;
};

struct  Float7{
    float x;
    float y;
    float z;
    float r;
    float g;
    float b;
    float a;
};

#endif //OPENGLTEST_GLCOMMONDEF_H
