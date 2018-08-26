//
// Created by wlanjie on 2018/8/26.
//

#ifndef OPENGL_OPENGL_H
#define OPENGL_OPENGL_H

#include <OpenGL/OpenGL.h>
#include <OpenGL/gl.h>

#define WIDTH 480
#define HEIGHT 854

static const char* defaultVertexShader =
    "attribute vec4 position;\n "
    "attribute vec4 inputTextureCoordinate;\n"
    "varying vec2 textureCoordinate;\n"
    "void main() {\n     "
    "   gl_Position = position;\n     "
    "   textureCoordinate = inputTextureCoordinate.xy;\n "
    "}\n ";

static const char* defaultFragmentShader =
    "varying vec2 textureCoordinate;\n"
    "uniform sampler2D inputImageTexture;\n"
    "void main() {\n"
    "   vec4 textureColor = texture2D(inputImageTexture, textureCoordinate);\n"
    "   gl_FragColor = textureColor;\n"
    "}\n";

static const GLfloat defaultVertexCoordinates[] = {
    -1.0f, -1.0f,
    1.0f, -1.0f,
    -1.0f, 1.0f,
    1.0f, 1.0f,
};

static const GLfloat defaultTextureCoordinate[] = {
    0.0f, 0.0f,
    1.0f, 0.0f,
    0.0f, 1.0f,
    1.0f, 1.0f
};

#endif //OPENGL_OPENGL_H
