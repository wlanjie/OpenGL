//
//  red_blue.cc
//  OpenGL
//
//  Created by wlanjie on 2019/6/6.
//  Copyright © 2019年 com.wlanjie.opengl. All rights reserved.
//

#include "red_blue.h"
#include "stb_image.h"

RedBlue::RedBlue(int width, int height) : FrameBuffer(width, height, "shader/red_blue.vs", "shader/red_blue.fs") {
    
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
    
    int nrChannels;
    stbi_set_flip_vertically_on_load(true);
    int image_width;
    int image_height;
    
    unsigned char *data = stbi_load("resources/textures/keyboard.jpg", &image_width, &image_height, &nrChannels, 0);
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
    
    int look_up_image_width;
    int look_up_image_height;
    
    unsigned char *look_up_data = stbi_load("resources/textures/lookup_vertigo.png", &look_up_image_width, &look_up_image_height, &nrChannels, 0);
    printf("width = %d height = %d\n", look_up_image_width, look_up_image_height);
    
    glGenTextures(1, &lookup_texture_id_);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, lookup_texture_id_);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, look_up_image_width, look_up_image_height, 0, GL_RGB, GL_UNSIGNED_BYTE, look_up_data);
    glGenerateMipmap(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);
    stbi_image_free(look_up_data);
}

void RedBlue::RunOnDrawTasks() {
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, lookup_texture_id_);
    SetInt("inputImageTextureLookup", 1);
    
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, textureId);
    SetInt("inputImageTextureLast", 2);
}
