//
//  blend.cc
//  OpenGL
//
//  Created by wlanjie on 2018/11/15.
//  Copyright © 2018年 com.wlanjie.opengl. All rights reserved.
//

#include "blend.h"
#include "shader.h"
#include "stb_image.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <transform.hpp>
#include <type_ptr.hpp>

#define MAX_FRAMS 26

Blend::Blend(int width, int height) : OpenGL(width, height, defaultVertexShader, blendFragmentShader) {
    progress = 0;
    frames = 0;
    
    int w;
    int h;
    int channels;
    
    unsigned char *data = stbi_load("/Users/wlanjie/project/OpenGL/resources/textures/meinv.jpg", &w, &h, &channels, 0);
    glGenTextures(1, &blendTextureId);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, blendTextureId);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);
    stbi_image_free(data);
}

Blend::~Blend() {
    
}

void Blend::RunOnDrawTasks() {
    if (frames >= MAX_FRAMS) {
        frames = 0;
    }
    
//    float scale = 1.0f + 0.2f * progress;
//    glm::mat4 scaleMatrix = glm::scale(glm::vec3(scale, scale, 1.0f));
//    auto mvpMatrixUniform = glGetUniformLocation(programId, "mvpMatrix");
//    glUniformMatrix4fv(mvpMatrixUniform, 1, GL_FALSE, glm::value_ptr(scaleMatrix));
//    auto offsetUniform = glGetUniformLocation(programId, "textureCoordinateOffset");
//    printf("progress = %f\n", 0.01 * progress);
//    glUniform1f(offsetUniform, 0.01f * progress);
    SetFloat("strength", frames * 1.0f / MAX_FRAMS);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, blendTextureId);
    SetInt("maskTexture", 1);
    frames++;
}

void Blend::OnDrawArrays() {
    
}

GLuint Blend::onDrawFrame(GLuint textureId) {
    ProcessImage(textureId, defaultVertexCoordinates, defaultTextureCoordinate);
    return 0;
}
