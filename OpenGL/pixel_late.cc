//
//  pixel_late.cc
//  OpenGL
//
//  Created by wlanjie on 2018/11/16.
//  Copyright © 2018年 com.wlanjie.opengl. All rights reserved.
//

#include "pixel_late.h"
#define GLM_ENABLE_EXPERIMENTAL
#include <transform.hpp>
#include <type_ptr.hpp>

#define PIXEL 0.05

PixelLate::PixelLate(int width, int height) : FrameBuffer(width, height, "shader/vertex_shader.vs", "shader/pixel_late_fragment.fs") {
    setOutput(width, height);
    this->width = width;
    this->height = height;
}

PixelLate::~PixelLate() {
    
}

void PixelLate::RunOnDrawTasks() {
    FrameBuffer::RunOnDrawTasks();
    float singlePixelSpacing = 0;
    if (width != 0) {
        singlePixelSpacing = 1.0 / width;
    } else {
        singlePixelSpacing = 1.0 / 2048.0;
    }
    
    if (singlePixelSpacing < PIXEL) {
        singlePixelSpacing = PIXEL;
    } else {
//        singlePixelSpacing = newValue;
    }
    SetFloat("fractionalWidthOfPixel", singlePixelSpacing);
    SetFloat("aspectRatio", height * 1.0f / width);
//    setFloat("aspectRatio", width * 1.0f / height);
}

void PixelLate::OnDrawArrays() {
    
}

GLuint PixelLate::onDrawFrame(int textureId) {
    return FrameBuffer::onDrawFrame(textureId);
}
