//
//  blend.cc
//  OpenGL
//
//  Created by wlanjie on 2018/11/15.
//  Copyright © 2018年 com.wlanjie.opengl. All rights reserved.
//

#include "blend.h"
#include "shader.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <transform.hpp>
#include <type_ptr.hpp>

Blend::Blend(int width, int height) : OpenGL(width, height, defaultVertexMatrixShader, shakeFragmentShader) {
    progress = 0;
    frames = 0;
}

Blend::~Blend() {
    
}

void Blend::runOnDrawTasks(GLuint programId) {
    float scale = 1.0f + 0.2f * progress;
    glm::mat4 scaleMatrix = glm::scale(glm::vec3(scale, scale, 1.0f));
    auto mvpMatrixUniform = glGetUniformLocation(programId, "mvpMatrix");
    glUniformMatrix4fv(mvpMatrixUniform, 1, GL_FALSE, glm::value_ptr(scaleMatrix));
    auto offsetUniform = glGetUniformLocation(programId, "textureCoordinateOffset");
    printf("progress = %f\n", 0.01 * progress);
    glUniform1f(offsetUniform, 0.01f * progress);
}

void Blend::onDrawArrays() {
    
}

GLuint Blend::onDrawFrame(GLuint textureId) {
    return processImage(textureId, defaultVertexCoordinates, defaultTextureCoordinate);
}
