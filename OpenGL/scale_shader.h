//
//  scale_shader.h
//  OpenGL
//
//  Created by wlanjie on 2018/11/10.
//  Copyright © 2018 com.wlanjie.opengl. All rights reserved.
//

#ifndef scale_shader_h
#define scale_shader_h

#include "shader_program.h"
#include "opengl.h"

class ScaleShader {
public:
    ScaleShader(int width, int height);
    ~ScaleShader();
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

#endif /* scale_shader_h */
