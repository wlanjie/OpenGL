//
//  mirror.hpp
//  OpenGL
//
//  Created by wlanjie on 2018/6/2.
//  Copyright © 2018年 com.wlanjie.opengl. All rights reserved.
//

#ifndef mirror_h
#define mirror_h

#include "framebuffer.h"
#include "opengl_rendering.h"

class Mirror {
public:
    Mirror(const char* filename, bool smoothlyScaleOutput = false);
    ~Mirror();
    int processImage(bool synchronously = false);
    
private:
    ShaderProgram* shader;
    Framebuffer *imageFramebuffer;
    OpenGLRendering* rendering;
    Color* backgroundColor;
    std::vector<InputTextureProperties*> inputTextures;
    GLuint vbo;
    GLSize* size;
};


#endif /* mirror_hpp */
