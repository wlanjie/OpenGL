//
//  color.h
//  OpenGL
//
//  Created by wlanjie on 2018/9/11.
//  Copyright © 2018年 com.wlanjie.opengl. All rights reserved.
//

#ifndef color_h
#define color_h

#include <time.h>
#include "shader_program.h"
#include "opengl.h"

class Color {
public:
    Color(int width, int height);
    Color();
    GLuint processImage(int textureId);
    
private:
    ShaderProgram* shader;
    GLuint textureId;
    GLuint frameBufferId;
    int width;
    int height;
    time_t startTime;
    GLfloat resolution[2];
};

#endif /* color_h */
