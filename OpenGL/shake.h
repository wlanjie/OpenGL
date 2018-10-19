//
//  shake.h
//  OpenGL
//
//  Created by wlanjie on 2018/10/19.
//  Copyright © 2018年 com.wlanjie.opengl. All rights reserved.
//
// 抖音抖动效果
#ifndef shake_h
#define shake_h

#include "shader_program.h"
#include "opengl.h"

class Shake {
public:
    Shake(int width, int height);
    Shake();
    GLuint processImage(int textureId);
    
private:
    ShaderProgram* shader;
    GLuint textureId;
    GLuint frameBufferId;
    int width;
    int height;
    int frames;
    float progress;
};

#endif /* shake_h */
