//
//  zoom_in_out.cc
//  OpenGL
//
//  Created by wlanjie on 2019/6/6.
//  Copyright © 2019年 com.wlanjie.opengl. All rights reserved.
//

#include "zoom_in_out.h"

ZoomInOut::ZoomInOut(int width, int height) : FrameBuffer(width, height, "shader/vertex_shader.vs", "shader/zoom_in_out.fs") {
    
    scalePercent[0] = 1.0;
    scalePercent[1] = 1.07;
    scalePercent[2] = 1.14;
    scalePercent[3] = 1.21;
    scalePercent[4] = 1.26;
    scalePercent[5] = 1.32;
    scalePercent[6] = 1.45;
    scalePercent[7] = 1.53;
    scalePercent[8] = 1.66;
    scalePercent[9] = 1.79;
    scalePercent[10] = 1.96;
    scalePercent[11] = 1.84;
    scalePercent[12] = 1.76;
    scalePercent[13] = 1.67;
    scalePercent[14] = 1.45;
    scalePercent[15] = 1.28;
    
    scalePercentIndex = 0;
    
    setOutput(width, height);
    this->width = width;
    this->height = height;
}

void ZoomInOut::RunOnDrawTasks() {
    float scale = scalePercent[scalePercentIndex % 16];
    scalePercentIndex++;
    SetFloat("scalePercent", scale);
}
