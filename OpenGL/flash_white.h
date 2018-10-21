//
//  flash_white.h
//  OpenGL
//
//  Created by wlanjie on 2018/10/20.
//  Copyright © 2018年 com.wlanjie.opengl. All rights reserved.
//

#ifndef flash_white_h
#define flash_white_h

#include "shader_program.h"
#include "opengl.h"

class FlashWhite {
public:
    FlashWhite(int width, int height);
    ~FlashWhite();
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


#endif /* flash_white_h */

