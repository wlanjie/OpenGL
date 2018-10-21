//
//  glitch_filter.cc
//  OpenGL
//
//  Created by wlanjie on 2018/10/20.
//  Copyright © 2018年 com.wlanjie.opengl. All rights reserved.
//

#include "glitch_filter.h"
#define GLM_ENABLE_EXPERIMENTAL
#include <transform.hpp>
#include <type_ptr.hpp>

#define MAX_FRAMES 8
#define SKIP_FRAMES 19

GlitchFilter::GlitchFilter(int width, int height) {
    this->width = width;
    this->height = height;
    frames = 0;
    progress = 0;
    driftSequence = new float[MAX_FRAMES];
    driftSequence[0] = 0.0f;
    driftSequence[1] = 0.03f;
    driftSequence[2] = 0.032f;
    driftSequence[3] = 0.035f;
    driftSequence[4] = 0.03f;
    driftSequence[5] = 0.032f;
    driftSequence[6] = 0.031f;
    driftSequence[7] = 0.029f;
    driftSequence[8] = 0.025f;
    
    jitterSequence = new float[MAX_FRAMES];
    jitterSequence[0] = 0.0f;
    jitterSequence[1] = 0.03f;
    jitterSequence[2] = 0.01f;
    jitterSequence[3] = 0.02f;
    jitterSequence[4] = 0.05f;
    jitterSequence[5] = 0.055f;
    jitterSequence[6] = 0.03f;
    jitterSequence[7] = 0.02f;
    jitterSequence[7] = 0.025f;
    
    threshHoldSequence = new float[MAX_FRAMES];
    threshHoldSequence[0] = 1.0f;
    threshHoldSequence[1] = 0.965f;
    threshHoldSequence[2] = 0.9f;
    threshHoldSequence[3] = 0.9f;
    threshHoldSequence[4] = 0.9f;
    threshHoldSequence[5] = 0.6f;
    threshHoldSequence[6] = 0.8f;
    threshHoldSequence[7] = 0.5f;
    threshHoldSequence[8] = 0.5f;
    
    shader = new ShaderProgram(defaultVertexShader, glitchFilterFragmentShader);
    
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

GlitchFilter::~GlitchFilter() {
    if (shader) {
        delete shader;
        shader = nullptr;
    }
    
    glDeleteTextures(1, &textureId);
    glDeleteFramebuffers(1, &frameBufferId);
}

GLuint GlitchFilter::processImage(int textureId) {
    shader->use();
    
    glBindFramebuffer(GL_FRAMEBUFFER, frameBufferId);
    glViewport(0, 0, width, height);
    glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    auto positionAttribute = shader->attributeIndex("position");
    glVertexAttribPointer(positionAttribute, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), defaultVertexCoordinates);
    auto textureCoordinateAttribute = shader->attributeIndex("inputTextureCoordinate");
    glVertexAttribPointer(textureCoordinateAttribute, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), defaultTextureCoordinate);

    frames++;
    if (frames > MAX_FRAMES) {
        frames = 0;
    }
    auto driftUniform = shader->uniformIndex("colorDrift");
    glUniform1f(driftUniform, driftSequence[frames]);
    auto jitterUniform = shader->uniformIndex("scanLineJitter");
    glUniform2fv(jitterUniform, 1, new float[2]{jitterSequence[frames], threshHoldSequence[frames]});
    
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureId);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    return this->textureId;
}

