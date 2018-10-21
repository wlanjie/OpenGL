//
//  scale.h
//  OpenGL
//
//  Created by wlanjie on 2018/8/27.
//  Copyright © 2018年 com.wlanjie.opengl. All rights reserved.
//

#ifndef scale_h
#define scale_h

#include "shader_program.h"
#include "opengl.h"

class Scale {
public:
    Scale(int width, int height);
    ~Scale();
    GLuint processImage(int textureId);
    
private:
    ShaderProgram* shader;
    GLuint textureId;
    GLuint frameBufferId;
    int width;
    int height;
    float progress;
    int frames;
    float scale;
    bool maxScale;
};

#endif /* scale_h */
