//
//  frame_buffer.cc
//  OpenGL
//
//  Created by wlanjie on 2018/11/16.
//  Copyright © 2018年 com.wlanjie.opengl. All rights reserved.
//

#include "frame_buffer.h"

FrameBuffer::FrameBuffer(int width, int height, const char* vertex, const char* fragment) : OpenGL(vertex, fragment) {
    glGenTextures(1, &textureId);
    glGenFramebuffers(1, &frameBufferId);
    glBindTexture(GL_TEXTURE_2D, textureId);
    glBindFramebuffer(GL_FRAMEBUFFER, frameBufferId);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureId, 0);
    
    int status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (status != GL_FRAMEBUFFER_COMPLETE) {
        printf("frame buffer error\n");
    }
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

FrameBuffer::~FrameBuffer() {
    glDeleteTextures(1, &textureId);
    glDeleteFramebuffers(1, &frameBufferId);
}

void FrameBuffer::RunOnDrawTasks() {
}

void FrameBuffer::OnDrawArrays() {
    
}

GLuint FrameBuffer::onDrawFrame(GLuint textureId) {
    glBindFramebuffer(GL_FRAMEBUFFER, frameBufferId);
    ProcessImage(textureId);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    return this->textureId;
}

GLuint FrameBuffer::onDrawFrame(GLuint textureId, const GLfloat *vertexCoordinate, const GLfloat *textureCoordinate) {
    glBindFramebuffer(GL_FRAMEBUFFER, frameBufferId);
    ProcessImage(textureId, vertexCoordinate, textureCoordinate);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    return this->textureId;
}
