//
//  scale.cc
//  OpenGL
//
//  Created by wlanjie on 2018/8/27.
//  Copyright © 2018年 com.wlanjie.opengl. All rights reserved.
//

#include "scale.h"
#define GLM_ENABLE_EXPERIMENTAL
#include <transform.hpp>
#include <type_ptr.hpp>


#define MAX_FRAMES 60
#define SKIP_FRAMES 30
#define MAX_SCALE 1.5

Scale::Scale(int width, int height) {
    this->width = width;
    this->height = height;
    progress = 0;
    frames = 0;
    scale = 1.0;
    maxScale = false;
    shader = new ShaderProgram(defaultVertexMatrixShader, defaultFragmentShader);
    
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

Scale::~Scale() {
    if (shader) {
        delete shader;
        shader = nullptr;
    }
    
    glDeleteTextures(1, &textureId);
    glDeleteFramebuffers(1, &frameBufferId);
}

GLuint Scale::processImage(int textureId) {
    shader->use();
    glBindFramebuffer(GL_FRAMEBUFFER, frameBufferId);
    glViewport(0, 0, width, height);
    glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    auto positionAttribute = shader->attributeIndex("position");
    glVertexAttribPointer(positionAttribute, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), defaultVertexCoordinates);
    auto textureCoordinateAttribute = shader->attributeIndex("inputTextureCoordinate");
    glVertexAttribPointer(textureCoordinateAttribute, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), defaultTextureCoordinate);
    auto scaleUniform = shader->uniformIndex("scale");

    if (frames <= MAX_FRAMES) {
        progress = frames * 1.0 / SKIP_FRAMES;
    } else {
        progress = 2.0 - frames * 1.0 / SKIP_FRAMES;
    }
    
    scale = 1.0f + 0.3f * progress;
    glm::mat4 scaleMatrix = glm::scale(glm::vec3(scale, scale, scale));
    printf("scale = %f progress = %f\n", scale, progress);
    
    auto mvpMatrixUniform = shader->uniformIndex("mvpMatrix");
    glUniformMatrix4fv(mvpMatrixUniform, 1, GL_FALSE, glm::value_ptr(scaleMatrix));
   
    if (maxScale) {
        frames--;
        if (frames < 0) {
            frames = 0;
            maxScale = false;
        }
    } else {
         frames++;
        if (frames >= MAX_FRAMES) {
            maxScale = true;
        }
    }
    
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureId);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    return this->textureId;
}
