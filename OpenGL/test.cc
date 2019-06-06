//
//  test.cc
//  OpenGL
//
//  Created by wlanjie on 2018/11/16.
//  Copyright © 2018年 com.wlanjie.opengl. All rights reserved.
//

#include "test.h"
#include "stb_image.h"

#define MAX_FRAMS 300

Test::Test(int width, int height) : FrameBuffer(width, height, "shader/vertex_shader.vs", "shader/test_fragment.fs") {
    int nrChannels;
    stbi_set_flip_vertically_on_load(true);
    int w;
    int h;
    frames = 0;
    
    unsigned char *data = stbi_load("resources/textures/meinv.jpg", &w, &h, &nrChannels, 0);
    printf("width = %d height = %d\n", width, height);
    
    glGenTextures(1, &textureId);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureId);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);
    stbi_image_free(data);
}

void Test::RunOnDrawTasks() {
   
}

void Test::OnDrawArrays() {
    if (frames >= MAX_FRAMS) {
        frames = 0;
    }
    SetFloat("strength", frames * 1.0f / MAX_FRAMS);
    frames++;
    
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, textureId);
    SetInt("maskTexture", 1);
}

GLuint Test::onDrawFrame(int textureId) {
    return FrameBuffer::onDrawFrame(textureId);
}
