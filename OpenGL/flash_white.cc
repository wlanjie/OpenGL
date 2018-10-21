//
//  flash_white.cc
//  OpenGL
//
//  Created by wlanjie on 2018/10/20.
//  Copyright © 2018年 com.wlanjie.opengl. All rights reserved.
//

#include "flash_white.h"

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

#define MAX_FRAMES 38
#define SKIP_FRAMES 19

FlashWhite::FlashWhite(int width, int height) {
    this->width = width;
    this->height = height;
    frames = 0;
    progress = 0;
    shader = new ShaderProgram(defaultVertexShader, flashWriteFragmentShader);
    
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

FlashWhite::~FlashWhite() {
    if (shader) {
        delete shader;
        shader = nullptr;
    }
    
    glDeleteTextures(1, &textureId);
    glDeleteFramebuffers(1, &frameBufferId);
}

GLuint FlashWhite::processImage(int textureId) {
    shader->use();
    
    if (frames <= MAX_FRAMES) {
        progress = frames * 1.0f / MAX_FRAMES;
    } else {
        progress = 2.0f - frames * 1.0f / MAX_FRAMES;
    }
    progress = (float) frames / MAX_FRAMES;
    if (progress > 1) {
        progress = 0;
    }
    frames++;
    if (frames > MAX_FRAMES) {
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
    
//    glUniformMatrix4fv(mvpMatrixUniform, 1, GL_FALSE, glm::value_ptr(scaleMatrix));
    auto offsetUniform = shader->uniformIndex("exposureColor");
    glUniform1f(offsetUniform, progress);
    printf("progress = %f\n", progress);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureId);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    return this->textureId;
}
