//
//  sticker_2d.cc
//  OpenGL
//
//  Created by wlanjie on 2019/6/6.
//  Copyright © 2019年 com.wlanjie.opengl. All rights reserved.
//

#include "sticker_2d.h"

Sticker2D::Sticker2D(int width, int height) : FrameBuffer(width, height, "shader/vertex_shader.vs", "shader/sticker_2d.fs") {

    colorDisx[0] = 1.1;
    colorDisx[1] = 1.15;
    colorDisx[2] = 0.97;
    colorDisx[3] = 0.95;
    colorDisx[4] = 1.05;
    colorDisx[5] = 1.1;
    colorDisx[6] = 1.02;
    colorDisx[7] = 0.93;
    colorDisx[8] = 1.04;
    
    timeStamp[0] = 0.0;
    timeStamp[1] = 0.0;
    timeStamp[2] = 0.0;
    timeStamp[3] = 0.0;
    timeStamp[4] = 0.0;
    timeStamp[5] = 0.0;
    timeStamp[6] = 0.0;
    timeStamp[7] = 0.0;
    timeStamp[8] = 0.0;
    timeStamp[9] = 0.0;
    timeStamp[10] = 0.0;
    timeStamp[11] = 0.0;
    timeStamp[12] = 0.0;
    timeStamp[13] = 0.0;
    timeStamp[14] = 0.0;
    timeStamp[15] = 1.0;
    timeStamp[16] = 1.0;
    timeStamp[17] = 1.0;
    timeStamp[18] = 1.0;
    timeStamp[19] = 1.0;
    timeStamp[20] = 1.0;
    timeStamp[21] = 0.0;
    timeStamp[22] = 0.0;
    timeStamp[23] = 0.0;
    timeStamp[24] = 0.0;
    timeStamp[25] = 0.0;
    timeStamp[26] = 0.0;
    timeStamp[27] = 0.0;
    timeStamp[28] = 0.0;
    timeStamp[29] = 0.0;
    timeStamp[30] = 0.0;
    timeStamp[31] = 0.0;
    timeStamp[32] = 0.0;
    timeStamp[33] = 0.0;
    timeStamp[34] = 0.0;
    timeStamp[35] = 2.0;
    timeStamp[36] = 2.0;
    timeStamp[37] = 2.0;
    timeStamp[38] = 2.0;
    timeStamp[39] = 2.0;
    timeStamp[40] = 2.0;
    timeStamp[41] = 2.0;
    timeStamp[42] = 2.0;
    timeStamp[43] = 2.0;
    timeStamp[44] = 0.0;
    timeStamp[45] = 0.0;
    timeStamp[46] = 0.0;
    timeStamp[47] = 0.0;
    timeStamp[48] = 0.0;
    timeStamp[49] = 0.0;
    timeStamp[50] = 0.0;
    timeStamp[51] = 0.0;
    timeStamp[52] = 0.0;
    timeStamp[53] = 0.0;
    timeStamp[54] = 0.0;
    timeStamp[55] = 3.0;
    timeStamp[56] = 3.0;
    timeStamp[57] = 3.0;
    timeStamp[58] = 3.0;
    timeStamp[59] = 3.0;
    timeStamp[60] = 3.0;
    timeStamp[61] = 3.0;
    timeStamp[62] = 3.0;
    timeStamp[63] = 3.0;
    timeStamp[64] = 3.0;
    timeStamp[65] = 3.0;
    timeStamp[66] = 3.0;
    timeStamp[67] = 3.0;
    timeStamp[68] = 3.0;
    timeStamp[69] = 3.0;
    timeStamp[70] = 4.0;
    timeStamp[71] = 5.0;
    timeStamp[72] = 6.0;
    timeStamp[73] = 0.0;
    timeStamp[74] = 0.0;
    timeStamp[75] = 0.0;
    timeStamp[76] = 0.0;
    timeStamp[77] = 0.0;
    timeStamp[78] = 0.0;
    timeStamp[79] = 0.0;
    timeStamp[80] = 0.0;
    timeStamp[81] = 0.0;
    timeStamp[82] = 0.0;
    timeStamp[83] = 0.0;
    timeStamp[84] = 0.0;
    timeStamp[85] = 0.0;
    timeStamp[86] = 0.0;
    timeStamp[87] = 0.0;
    timeStamp[88] = 0.0;
    timeStamp[89] = 0.0;
    
    colorDisxIndex = 0;
    timeStampIndex = 0;
    
    setOutput(width, height);
    this->width = width;
    this->height = height;
}

Sticker2D::~Sticker2D() {
    
}

void Sticker2D::runOnDrawTasks() {
    float color = colorDisx[colorDisxIndex % 9];
    colorDisxIndex++;
    
    float time = timeStamp[timeStampIndex % 90];
    timeStampIndex++;
    
    setFloat("timeStamp", time);
    setFloat("colorDis_X", color);
    
}

void Sticker2D::onDrawArrays() {
    
}

GLuint Sticker2D::onDrawFrame(int textureId) {
    return FrameBuffer::onDrawFrame(textureId);
}
