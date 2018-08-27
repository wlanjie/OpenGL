//
//  mirror.h
//  OpenGL
//
//  Created by wlanjie on 2018/8/26.
//  Copyright © 2018年 com.wlanjie.opengl. All rights reserved.
//

#ifndef mirror_h
#define mirror_h

#include <OpenGL/OpenGL.h>
#include <OpenGL/gl.h>
#include "opengl.h"
#include "shader_program.h"

class Mirror {
public:
    Mirror(int width, int height);
    ~Mirror();
    GLuint processImage(GLuint textureId);
private:
    ShaderProgram* shader;
    GLuint textureId;
    GLuint frameBufferId;
    int width;
    int height;
};


#endif /* mirror_h */
