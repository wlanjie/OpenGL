//
//  soul_out.cc
//  OpenGL
//
//  Created by wlanjie on 2018/9/27.
//  Copyright © 2018年 com.wlanjie.opengl. All rights reserved.
//
#define GLM_ENABLE_EXPERIMENTAL
#include <transform.hpp>
#include <type_ptr.hpp>
#include "soul_out.h"
#define MAX_FRAMS 30
#define SKIP_FRAMS 8

SoulOut::SoulOut(int width, int height) {
    this->width = width;
    this->height = height;
    progress = 0;
    frames = 0;
    shader = new ShaderProgram(defaultVertexMatrixShader, soulOutFragmentShader);
    
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

SoulOut::~SoulOut() {
    if (shader) {
        delete shader;
        shader = nullptr;
    }
    
    glDeleteTextures(1, &textureId);
    glDeleteFramebuffers(1, &frameBufferId);
}

GLuint SoulOut::processImage(int textureId) {
    shader->use();
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_DST_ALPHA);

    progress = (float) frames / MAX_FRAMS;
    if (progress > 1) {
        progress = 0;
    }
    
    frames++;
    if (frames > MAX_FRAMS + SKIP_FRAMS) {
        frames = 0;
    }
    
    float backAlpha = 1.0f;
    float alpha = 0.0f;
    if (progress > 0) {
        alpha = 0.2f - progress * 0.2;
        backAlpha = 1 - alpha;
    }
    
    glBindFramebuffer(GL_FRAMEBUFFER, frameBufferId);
    glViewport(0, 0, width, height);
    glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    auto positionAttribute = shader->attributeIndex("position");
    glVertexAttribPointer(positionAttribute, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), defaultVertexCoordinates);
    auto textureCoordinateAttribute = shader->attributeIndex("inputTextureCoordinate");
    glVertexAttribPointer(textureCoordinateAttribute, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), defaultTextureCoordinate);
    
    auto alphaUniform = shader->uniformIndex("alpha");
    glUniform1f(alphaUniform, backAlpha);
    
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureId);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    
    if (progress > 0) {
        glUniform1f(alphaUniform, alpha);
        float scale = 1.0f + 1.0f * progress;
        glm::mat4 scaleMatrix = glm::scale(glm::vec3(scale, scale, scale));
        auto mvpMatrixUniform = shader->uniformIndex("mvpMatrix");
        glUniformMatrix4fv(mvpMatrixUniform, 1, GL_FALSE, glm::value_ptr(scaleMatrix));
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    }
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glDisable(GL_BLEND);
    return this->textureId;
}
