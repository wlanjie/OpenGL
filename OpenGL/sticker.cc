//
//  sticker.cc
//  OpenGL
//
//  Created by wlanjie on 2018/11/15.
//  Copyright © 2018年 com.wlanjie.opengl. All rights reserved.
//

#include "sticker.h"
#include "shader.h"
#include "stb_image.h"

Sticker::Sticker(int width, int height) : OpenGL(width, height, defaultVertexShader, defaultFragmentShader) {
    this->width = width;
    this->height = height;
    shader = new ShaderProgram(defaultVertexShader, defaultFragmentShader);
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
    fragment[0] = 0.0f;
    fragment[1] = 0.0f;
    fragment[2] = 1.0f;
    fragment[3] = 0.0f;
    fragment[4] = 0.0f;
    fragment[5] = 1.0f;
    fragment[6] = 1.0f;
    fragment[7] = 1.0f;
    
    vertex[0] = -1.0f;
    vertex[1] = 0.0f;
    vertex[2] = 0.5f;
    vertex[3] = 0.0f;
    vertex[4] = -1.0f;
    vertex[5] = 1.0f;
    vertex[6] = 0.5f;
    vertex[7] = 1.0f;
    
    int w;
    int h;
    int channels;
    GLuint textureId;
    
    unsigned char *data = stbi_load("/Users/wlanjie/project/OpenGL/resources/textures/meinv.jpg", &w, &h, &channels, 0);
    glGenTextures(1, &textureId);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureId);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);
    stbi_image_free(data);
}

Sticker::~Sticker() {
    
}

void Sticker::runOnDrawTasks(GLuint programId) {
    
}

void Sticker::onDrawArrays() {
    glActiveTexture(GL_TEXTURE1);
    
}

GLuint Sticker::onDrawFrame() {
//    processImage(textureId, defaultVertexCoordinates, defaultTextureCoordinate);
//    shader->use();
//    glBindFramebuffer(GL_FRAMEBUFFER, frameBufferId);
//    glViewport(0, 0, width, height);
//    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
//    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//    auto positionAttribute = shader->attributeIndex("position");
//    glVertexAttribPointer(positionAttribute, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), vertex);
//    auto textureCoordinateAttribute = shader->attributeIndex("inputTextureCoordinate");
//    glVertexAttribPointer(textureCoordinateAttribute, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), fragment);
//    auto uniformTexture = shader->uniformIndex("inputImageTexture");
//    
//    glActiveTexture(GL_TEXTURE0);
//    glBindTexture(GL_TEXTURE_2D, 0);
//    glUniform1i(uniformTexture, 0);
//    
//    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
//    glDisableVertexAttribArray(positionAttribute);
//    glDisableVertexAttribArray(textureCoordinateAttribute);
//    glBindTexture(GL_TEXTURE_2D, 0);
//    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    return this->textureId;
}
