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

static const char* defaultVertexMatrixShader =
    "attribute vec2 position;\n"
    "attribute vec4 inputTextureCoordinate;\n"
    "varying vec2 textureCoordinate;\n"
    "uniform mat4 mvpMatrix;\n"
    "void main() {\n"
    "   gl_Position = mvpMatrix * vec4(position, 0.1, 1.0);\n"
    "   textureCoordinate = (inputTextureCoordinate).xy;\n"
    "}";

static const char* defaultFragmentShader =
    "varying vec2 textureCoordinate;\n"
    "uniform sampler2D inputImageTexture;\n"
    "void main() {\n"
    "   vec4 textureColor = texture2D(inputImageTexture, textureCoordinate);\n"
    "   gl_FragColor = textureColor;\n"
    "}\n";

static const char* scaleFragmentShader =
    "varying vec2 textureCoordinate;\n"
    "uniform sampler2D inputImageTexture;\n"
    "void main() {\n"
    "   vec2 offset = vec2(0.05, 0.05);\n"
    "   float scale = 1.1;\n"
    "   vec2 coordinate;\n"
    "   coordinate.x = (textureCoordinate.x - offset.x) / scale;\n"
    "   coordinate.y = (textureCoordinate.y - offset.y) / scale;\n"
    "   vec4 textureColor = texture2D(inputImageTexture, coordinate);\n"
    "   gl_FragColor = textureColor;\n"
    "}\n";

static const char* soulOutFragmentShader =
    "";

static const char* shakeFragmentShader =
    "varying vec2 textureCoordinate;\n"
    "uniform sampler2D inputImageTexture;\n"
    "uniform float textureCoordinateOffset;\n"
    "void main() {\n"
    "   vec4 blue = texture2D(inputImageTexture, textureCoordinate);\n"
    "   vec4 green = texture2D(inputImageTexture, vec2(textureCoordinate.x, textureCoordinate.y + textureCoordinateOffset));"
    "   vec4 red = texture2D(inputImageTexture, vec2(textureCoordinate.x - textureCoordinateOffset, textureCoordinate.y - textureCoordinateOffset));"
    "   gl_FragColor = vec4(red.x, green.y, blue.z, blue.w);"
    "}";

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

static const GLfloat mirrorVertexOne[] = {
    // 左上角
    -1.0f, -1.0f,
    1.0f, 1.0f,
    -1.0f, 1.0f,

    -1.0f, -1.0f,
    1.0f, -1.0f,
    1.0f, 1.0f,
};

static const GLfloat mirrorTextureOne[] = {
    // 左上角
    1.0f, 0.0f,
    0.0f, 1.0f,
    1.0f, 1.0f,
    
    1.0f, 0.0f,
    0.0f, 0.0f,
    0.0f, 1.0f,
};

static const GLfloat mirrorTextureVOne[] = {
    // 左上角
    0.0f, 1.0f,
    1.0f, 0.0f,
    0.0f, 0.0f,
    
    0.0f, 1.0f,
    1.0f, 1.0f,
    1.0f, 0.0f,
};

static const GLfloat mirrorVertexTwo[] = {
    -1.0f, -1.0f,
    0.0f, 1.0f,
    -1.0f, 1.0f,
    
    -1.0f, -1.0f,
    0.0f, -1.0f,
    0.0f, 1.0f,
    
    0.0f, -1.0f,
    1.0f, 1.0f,
    0.0f, 1.0f,
    
    0.0f, -1.0f,
    1.0f, -1.0f,
    1.0f, 1.0f
};

static const GLfloat mirrorTextureTwo[] = {
    // 左上角
    0.0f, 0.0f,
    0.5f, 1.0f,
    0.0f, 1.0f,
    
    0.0f, 0.0f,
    0.5f, 0.0f,
    0.5f, 1.0f,
    
    // 右上角
    0.5f, 0.0f,
    0.0f, 1.0f,
    0.5f, 1.0f,
    
    0.5f, 0.0f,
    0.0f, 0.0f,
    0.0f, 1.0f,
};

static const GLfloat mirrorVertexVTwo[] = {
    // 上面
    -1.0f, 0.0f,
    1.0f, 1.0f,
    -1.0f, 1.0f,
    
    -1.0f, 0.0f,
    1.0f, 0.0f,
    1.0f, 1.0f,
    
    // 下面
    -1.0f, -1.0f,
    1.0f, 0.0f,
    -1.0f, 0.0f,
    
    -1.0f, -1.0f,
    1.0f, -1.0f,
    1.0f, 0.0f
};

static const GLfloat mirrorTextureVTwo[] = {
    // 上面
    0.0f, 0.5f,
    1.0f, 1.0f,
    0.0f, 1.0f,
    
    0.0f, 0.5f,
    1.0f, 0.5f,
    1.0f, 1.0f,
    
    // 下面
    0.0f, 1.0f,
    1.0f, 0.5f,
    0.0f, 0.5f,

    0.0f, 1.0f,
    1.0f, 1.0f,
    1.0f, 0.5f,
};

static const GLfloat mirrorTriangleVertex[] = {
    
    // 左上角
    -1.0f, 0.0f,
    0.0f, 1.0f,
    -1.0f, 1.0f,
    
    -1.0f, 0.0f,
    0.0f, 0.0f,
    0.0f, 1.0f,
    
    // 右上角
    0.0f, 0.0f,
    1.0f, 1.0f,
    0.0f, 1.0f,
    
    0.0f, 0.0f,
    1.0f, 0.0f,
    1.0f, 1.0f,
    
    // 左下角
    -1.0f, -1.0f,
    0.0f, 0.0f,
    -1.0f, 0.0f,
    
    -1.0f, -1.0f,
    0.0f, -1.0f,
    0.0f, 0.0f,
    
    // 右下角
    0.0f, -1.0f,
    1.0f, 0.0f,
    0.0f, 0.0f,
    
    0.0f, -1.0f,
    1.0f, -1.0f,
    1.0f, 0.0f,
    
};

static const GLfloat mirrorTriangleTexture[] = {
    // 左上角
    0.0f, 0.0f,
    1.0f, 1.0f,
    0.0f, 1.0f,
    
    0.0f, 0.0f,
    1.0f, 0.0f,
    1.0f, 1.0f,
    
    // 右上角
    1.0f, 0.0f,
    0.0f, 1.0f,
    1.0f, 1.0f,
    
    1.0f, 0.0f,
    0.0f, 0.0f,
    0.0f, 1.0f,
    
    // 左下角
    //    1.0f, 1.0f,
    0.0f, 1.0f,
    1.0f, 0.0f,
    0.0f, 0.0f,
    
    0.0f, 1.0f,
    1.0f, 1.0f,
    1.0f, 0.0f,
    
    // 右下角
    1.0f, 1.0f,
    0.0f, 0.0f,
    1.0f, 0.0f,
    
    1.0f, 1.0f,
    0.0f, 1.0f,
    0.0f, 0.0f
};

#endif //OPENGL_OPENGL_H
