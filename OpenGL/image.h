//
// Created by wlanjie on 2018/3/20.
//

#ifndef OPENGL_IMAGE_H
#define OPENGL_IMAGE_H

#include "shader_program.h"
#include "opengl.h"

class Image {
public:
    Image(int width, int height);
    ~Image();
    GLuint processImage(int textureId);

private:
    ShaderProgram* shader;
    GLuint textureId;
    GLuint frameBufferId;
    int width;
    int height;
};


#endif //OPENGL_IMAGE_H
