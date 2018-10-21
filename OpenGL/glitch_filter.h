//
//  glitch_filter.h
//  OpenGL
//
//  Created by wlanjie on 2018/10/20.
//  Copyright © 2018年 com.wlanjie.opengl. All rights reserved.
//

#ifndef glitch_filter_h
#define glitch_filter_h

#include "shader_program.h"
#include "opengl.h"

class GlitchFilter {
public:
    GlitchFilter(int width, int height);
    ~GlitchFilter();
    GLuint processImage(int textureId);
    
private:
    ShaderProgram* shader;
    GLuint textureId;
    GLuint frameBufferId;
    int width;
    int height;
    int frames;
    float progress;
    float* driftSequence;
    float* jitterSequence;
    float* threshHoldSequence;
};


#endif /* glitch_filter_h */
