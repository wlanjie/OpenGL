//
// Created by wlanjie on 2018/3/19.
//

#ifndef OPENGL_FRAMEBUFFER_H
#define OPENGL_FRAMEBUFFER_H

#include <OpenGL/gl.h>
#include "size.h"

class Framebuffer {
public:
    Framebuffer(GLSize* size,
                bool textureOnly = false,
                GLint minFilter = GL_LINEAR,
                GLint magFilter = GL_LINEAR,
                GLint wrapS = GL_CLAMP_TO_EDGE,
                GLint wrapT = GL_CLAMP_TO_EDGE,
                GLint internalFormat = GL_RGBA,
                GLenum format = GL_BGRA,
                GLenum type = GL_UNSIGNED_BYTE,
                GLuint overriddenTexture = 0);
    ~Framebuffer();

    void activateFramebufferForRendering();
    GLuint getTexture() { return texture; }
    GLuint getFramebuffer() { return framebuffer; }
private:
    GLSize* size;
    GLuint texture;
    GLuint framebuffer;
    bool textureOverride;
};


#endif //OPENGL_FRAMEBUFFER_H
