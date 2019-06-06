//
//  huan_ying.cc
//  OpenGL
//
//  Created by wlanjie on 2019/6/6.
//  Copyright © 2019年 com.wlanjie.opengl. All rights reserved.
//

#include "huan_ying.h"
#include "stb_image.h"

HuanYing::HuanYing(int width, int height) : FrameBuffer(width, height, "shader/vertex_shader.vs", "shader/huanying.fs") {
    setOutput(width, height);
    this->width = width;
    this->height = height;
    
    int nrChannels;
    stbi_set_flip_vertically_on_load(true);
    int image_width;
    int image_height;
    
    unsigned char *data = stbi_load("resources/textures/huanying/color.png", &image_width, &image_height, &nrChannels, 0);
    printf("width = %d height = %d\n", width, height);
    
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

HuanYing::~HuanYing() {
    
}

void HuanYing::RunOnDrawTasks() {
    FrameBuffer::RunOnDrawTasks();
    
}

void HuanYing::OnDrawArrays() {
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, textureId);
    SetInt("inputImageTexture2", 1);
}

GLuint HuanYing::onDrawFrame(int textureId) {
    return FrameBuffer::onDrawFrame(textureId);
}
