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

FlashWhite::FlashWhite(int width, int height) : FrameBuffer(width, height, "shader/vertex_shader.vs", "shader/flash_white_fragment.fs") {
    progress = 0;
    frames = 0;
    setOutput(width, height);
    flash_write[0] = 0.33;
    flash_write[1] = 0.66;
    flash_write[2] = 1.0;
    flash_write[3] = 0.66;
    flash_write[4] = 0.33;
    flash_write[5] = 0.0;
    flash_write_index_ = 0;
}

FlashWhite::~FlashWhite() {
    
}

void FlashWhite::RunOnDrawTasks() {
    
    SetFloat("alphaTimeLine", flash_write[flash_write_index_ % 6]);
    flash_write_index_++;
    
//    FrameBuffer::runOnDrawTasks();
//    if (frames <= MAX_FRAMES) {
//        progress = frames * 1.0f / MAX_FRAMES;
//    } else {
//        progress = 2.0f - frames * 1.0f / MAX_FRAMES;
//    }
//    progress = (float) frames / MAX_FRAMES;
//    if (progress > 1) {
//        progress = 0;
//    }
//    frames++;
//    if (frames > MAX_FRAMES) {
//        frames = 0;
//    }
////    float scale = 1.0f + 0.2f * progress;
////    glm::mat4 scaleMatrix = glm::scale(glm::vec3(scale, scale, 1.0f));
//    setFloat("exposureColor", progress);
}

GLuint FlashWhite::onDrawFrame(GLuint textureId) {
    return FrameBuffer::onDrawFrame(textureId);
}
