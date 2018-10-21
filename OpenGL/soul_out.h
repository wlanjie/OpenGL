//
//  soul_out.h
//  OpenGL
//
//  Created by wlanjie on 2018/9/27.
//  Copyright © 2018年 com.wlanjie.opengl. All rights reserved.
//

#ifndef soul_out_h
#define soul_out_h

// 灵魂出窍
#include "shader_program.h"
#include "opengl.h"

class SoulOut {
public:
    SoulOut(int width, int height);
    ~SoulOut();
    GLuint processImage(int textureId);
    
private:
    ShaderProgram* shader;
    GLuint textureId;
    GLuint frameBufferId;
    int width;
    int height;
    float progress;
    int frames;
};

#endif /* soul_out_h */
