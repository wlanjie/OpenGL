//
// Created by wlanjie on 2018/3/19.
//

#include "framebuffer.h"
#include "opengl_rendering.h"

Framebuffer::Framebuffer(GLSize* size,
                         bool textureOnly,
                         GLint minFilter,
                         GLint magFilter,
                         GLint wrapS,
                         GLint wrapT,
                         GLint internalFormat,
                         GLenum format,
                         GLenum type,
                         GLuint overriddenTexture) :
        texture(0),
        textureOverride(false),
        framebuffer(0) {

    this->size = size;
    OpenGLRendering rendering;
    GLuint newTexture = 0;
    if (overriddenTexture) {
        textureOverride = true;
        texture = newTexture;
    } else {
        textureOverride = false;
        texture = rendering.generateTexture(minFilter, magFilter, wrapS, wrapT);
    }
    if (!textureOnly) {
        framebuffer = rendering.generateFramebufferForTexture(texture, size->getWidth(), size->getHeight(), internalFormat, format, type);
    }
}

Framebuffer::~Framebuffer() {
    if (!textureOverride) {
        glDeleteTextures(1, &texture);
    }
    if (framebuffer) {
        glDeleteFramebuffers(1, &framebuffer);
    }
}

void Framebuffer::activateFramebufferForRendering() {
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    glViewport(0, 0, size->getWidth(), size->getHeight());
}
