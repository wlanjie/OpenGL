//
//  scale.cc
//  OpenGL
//
//  Created by wlanjie on 2018/11/15.
//  Copyright © 2018年 com.wlanjie.opengl. All rights reserved.
//

#include "scale.h"
#include "shader.h"
#define GLM_ENABLE_EXPERIMENTAL
#include <transform.hpp>
#include <type_ptr.hpp>

#define MAX_FRAMES 60
#define SKIP_FRAMES 30
#define MAX_SCALE 1.5

Scale::Scale(int width, int height) : OpenGL(width, height, defaultVertexMatrixShader, defaultFragmentShader) {
    progress = 0;
    frames = 0;
    scale = 1.0;
    maxScale = false;
}

Scale::~Scale() {
    
}

void Scale::RunOnDrawTasks() {
    if (frames <= MAX_FRAMES) {
        progress = frames * 1.0 / SKIP_FRAMES;
    } else {
        progress = 2.0 - frames * 1.0 / SKIP_FRAMES;
    }
    
    scale = 1.0f + 0.3f * progress;
    glm::mat4 scaleMatrix = glm::scale(glm::vec3(scale, scale, scale));
    
    SetUnifromMatrix4f("mvpMatrix", 1, glm::value_ptr(scaleMatrix));
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
}

void Scale::OnDrawArrays() {
    
}

GLuint Scale::onDrawFrame(GLuint textureId) {
    ProcessImage(textureId, defaultVertexCoordinates, defaultTextureCoordinate);
    return 0;
}
