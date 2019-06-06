//
//  fake_3d.cc
//  OpenGL
//
//  Created by wlanjie on 2019/6/6.
//  Copyright © 2019年 com.wlanjie.opengl. All rights reserved.
//

#include "fake_3d.h"


Fake3D::Fake3D(int width, int height) : FrameBuffer(width, height, "shader/vertex_shader.vs", "shader/fake_3d.fs") {

    scalePercent[0] = 1.0;
    scalePercent[1] = 1.07;
    scalePercent[2] = 1.1;
    scalePercent[3] = 1.13;
    scalePercent[4] = 1.17;
    scalePercent[5] = 1.2;
    scalePercent[6] = 1.2;
    scalePercent[7] = 1.0;
    scalePercent[8] = 1.0;
    scalePercent[9] = 1.0;
    scalePercent[10] = 1.0;
    scalePercent[11] = 1.0;
    scalePercent[12] = 1.0;
    scalePercent[13] = 1.0;
    scalePercent[14] = 1.0;
    scalePercent[15] = 1.28;
    
    scalePercentIndex = 0;
    
    setOutput(width, height);
    this->width = width;
    this->height = height;
}

void Fake3D::RunOnDrawTasks() {
    float scale = scalePercent[scalePercentIndex % 15];
    scalePercentIndex++;
    SetFloat("scale", scale);
}
