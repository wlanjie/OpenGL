//
//  shake.cc
//  OpenGL
//
//  Created by wlanjie on 2018/11/15.
//  Copyright © 2018年 com.wlanjie.opengl. All rights reserved.
//

#include "shake.h"
#include "shader.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <transform.hpp>
#include <type_ptr.hpp>

#define MAX_FRAMES 8
#define SKIP_FRAMES 4

Shake::Shake(int width, int height) : OpenGL(width, height, defaultVertexMatrixShader, shakeFragmentShader) {
    frames = 0;
    progress = 0.0f;
}

Shake::~Shake() {
    
}

GLuint Shake::onDrawFrame(int textureId) {
    processImage(textureId, defaultVertexCoordinates, defaultTextureCoordinate);
    return 0;
}

void Shake::runOnDrawTasks() {
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
    setUnifromMatrix4f("mvpMatrix", 1, glm::value_ptr(scaleMatrix));
    setFloat("textureCoordinateOffset", 0.01f * progress);
}

void Shake::onDrawArrays() {
    
}
