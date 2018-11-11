//
//  blend.h
//  OpenGL
//
//  Created by wlanjie on 2018/10/27.
//  Copyright © 2018年 com.wlanjie.opengl. All rights reserved.
//

#ifndef blend_h
#define blend_h

#include "shader_program.h"
#include "opengl.h"

class Blend {
public:
    Blend(int width, int height);
    ~Blend();
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


#endif /* blend_h */
