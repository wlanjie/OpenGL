//
//  flash_white.cc
//  OpenGL
//
//  Created by wlanjie on 2018/11/15.
//  Copyright © 2018年 com.wlanjie.opengl. All rights reserved.
//

#include "flash_white.h"
#include "shader.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <transform.hpp>
#include <type_ptr.hpp>

#define MAX_FRAMES 8
#define SKIP_FRAMES 4

FlashWhite::FlashWhite(int width, int height) : OpenGL(width, height, defaultVertexShader, flashWriteFragmentShader) {
    progress = 0;
    frames = 0;
}

FlashWhite::~FlashWhite() {
    
}

void FlashWhite::runOnDrawTasks(GLuint programId) {
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
    auto offsetUniform = glGetUniformLocation(programId, "exposureColor");
    glUniform1f(offsetUniform, progress);
}

void FlashWhite::onDrawArrays() {
    
}

GLuint FlashWhite::onDrawFrame(GLuint textureId) {
    return processImage(textureId, defaultVertexCoordinates, defaultTextureCoordinate);
}
