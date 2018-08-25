//
// Created by wlanjie on 2018/3/20.
//

#ifndef OPENGL_IMAGE_H
#define OPENGL_IMAGE_H

#include "framebuffer.h"
#include "opengl_rendering.h"

class Image {
public:
    Image(const char* filename, bool smoothlyScaleOutput = false);
    ~Image();
    void processImage(bool synchronously = false);

private:
    ShaderProgram* shader;
    Framebuffer *imageFramebuffer;
    OpenGLRendering* rendering;
    Color* backgroundColor;
    std::vector<InputTextureProperties*> inputTextures;
    GLuint vbo;
    GLSize* size;
};


#endif //OPENGL_IMAGE_H
