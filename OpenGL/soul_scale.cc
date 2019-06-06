//
//  soul_scale.cc
//  OpenGL
//
//  Created by wlanjie on 2019/6/6.
//  Copyright © 2019年 com.wlanjie.opengl. All rights reserved.
//

#include "soul_scale.h"
#include "stb_image.h"

SoulScale::SoulScale(int width, int height) : FrameBuffer(width, height, "shader/vertex_shader.vs", "shader/soul_scale.fs") {
    mixturePercent[0] = 0.411498;
    mixturePercent[1] = 0.340743;
    mixturePercent[2] = 0.283781;
    mixturePercent[3] = 0.237625;
    mixturePercent[4] = 0.199993;
    mixturePercent[5] = 0.169133;
    mixturePercent[6] = 0.143688;
    mixturePercent[7] = 0.122599;
    mixturePercent[8] = 0.037117;
    mixturePercent[9] = 0.028870;
    mixturePercent[10] = 0.022595;
    mixturePercent[11] = 0.017788;
    mixturePercent[12] = 0.010000;
    mixturePercent[13] = 0.010000;
    mixturePercent[14] = 0.010000;
    mixturePercent[15] = 0.010000;
    
    scalePercent[0] = 1.084553;
    scalePercent[1] = 1.173257;
    scalePercent[2] = 1.266176;
    scalePercent[3] = 1.363377;
    scalePercent[4] = 1.464923;
    scalePercent[5] = 1.570877;
    scalePercent[6] = 1.681300;
    scalePercent[7] = 1.796254;
    scalePercent[8] = 1.915799;
    scalePercent[9] = 2.039995;
    scalePercent[10] = 2.168901;
    scalePercent[11] = 2.302574;
    scalePercent[12] = 2.302574;
    scalePercent[13] = 2.302574;
    scalePercent[14] = 2.302574;
    scalePercent[15] = 2.302574;
    
    mixturePercentIndex = 0;
    scalePercentIndex = 0;
    
    setOutput(width, height);
    this->width = width;
    this->height = height;
    
    int nrChannels;
    stbi_set_flip_vertically_on_load(true);
    int image_width;
    int image_height;
    
    unsigned char *data = stbi_load("resources/textures/meinv.jpg", &image_width, &image_height, &nrChannels, 0);
    printf("width = %d height = %d\n", image_width, image_height);
    
    glGenTextures(1, &textureId);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureId);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image_width, image_height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);
    stbi_image_free(data);
}

SoulScale::~SoulScale() {
    
}

void SoulScale::runOnDrawTasks() {
    float mixture = mixturePercent[mixturePercentIndex % 16];
    mixturePercentIndex++;
    
    float scale = scalePercent[scalePercentIndex % 16];
    scalePercentIndex++;
    
    setFloat("mixturePercent", mixture);
    setFloat("scalePercent", scale);
    
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, textureId);
    setInt("inputImageTexture2", 1);
}

void SoulScale::onDrawArrays() {

}

GLuint SoulScale::onDrawFrame(int textureId) {
    return FrameBuffer::onDrawFrame(textureId);
}
