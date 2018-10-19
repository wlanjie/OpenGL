//
//  shake.cc
//  OpenGL
//
//  Created by wlanjie on 2018/10/19.
//  Copyright © 2018年 com.wlanjie.opengl. All rights reserved.
//

#define GLM_ENABLE_EXPERIMENTAL

#include "shake.h"
#include <transform.hpp>
#include <type_ptr.hpp>

#define MAX_FRAMES 8
#define SKIP_FRAMES 4

Shake::Shake(int width, int height) {
    this->width = width;
    this->height = height;
    frames = 0;
    progress = 0;
    shader = new ShaderProgram(defaultVertexMatrixShader, shakeFragmentShader);
    
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

Shake::Shake() {
    if (shader) {
        delete shader;
        shader = nullptr;
    }
    
    glDeleteTextures(1, &textureId);
    glDeleteFramebuffers(1, &frameBufferId);
}

GLuint Shake::processImage(int textureId) {
    shader->use();
    
    progress = (float) frames / MAX_FRAMES;
    if (progress > 1) {
        progress = 0;
    }
    frames++;
    if (frames > MAX_FRAMES + SKIP_FRAMES) {
        frames = 0;
    }
    float scale = 1.0f + 0.2f * progress;
    glm::mat4 scaleMatrix = glm::scale(glm::vec3(scale, scale, 1.0f));
    
    glBindFramebuffer(GL_FRAMEBUFFER, frameBufferId);
    glViewport(0, 0, width, height);
    glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    auto positionAttribute = shader->attributeIndex("position");
    glVertexAttribPointer(positionAttribute, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), defaultVertexCoordinates);
    auto textureCoordinateAttribute = shader->attributeIndex("inputTextureCoordinate");
    glVertexAttribPointer(textureCoordinateAttribute, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), defaultTextureCoordinate);
    auto mvpMatrixUniform = shader->uniformIndex("mvpMatrix");
    auto matrix = glm::value_ptr(scaleMatrix);
    
    for (int i = 0; i < 16; i++) {
        printf("i = %f\n", matrix[i]);
    }
    glUniformMatrix4fv(mvpMatrixUniform, 1, GL_FALSE, glm::value_ptr(scaleMatrix));
    auto offsetUniform = shader->uniformIndex("textureCoordinateOffset");
    glUniform1f(offsetUniform, 0.01f * progress);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureId);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    return this->textureId;
}
