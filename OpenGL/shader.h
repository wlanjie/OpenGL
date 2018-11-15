//
//  shader.h
//  OpenGL
//
//  Created by wlanjie on 2018/11/15.
//  Copyright © 2018年 com.wlanjie.opengl. All rights reserved.
//

#ifndef shader_h
#define shader_h

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
"uniform float scale;\n"
"uniform float offsetX;\n"
"uniform float offsetY;\n"
"void main() {\n"
"   vec2 offset = vec2(offsetX, offsetY);\n"
"   vec2 coordinate;\n"
"   coordinate.x = (textureCoordinate.x ) / scale;\n"
"   coordinate.y = (textureCoordinate.y ) / scale;\n"
"   vec4 textureColor = texture2D(inputImageTexture, coordinate);\n"
"   gl_FragColor = textureColor;\n"
"}\n";

static const char* soulOutFragmentShader =
"varying vec2 textureCoordinate;\n"
"uniform sampler2D inputImageTexture;\n"
"uniform float alpha;\n"
"void main() {\n"
"   gl_FragColor = vec4(texture2D(inputImageTexture, textureCoordinate).rgb, alpha);\n"
"}";

static const char* shakeFragmentShader =
"varying vec2 textureCoordinate;\n"
"uniform sampler2D inputImageTexture;\n"
"uniform float textureCoordinateOffset;\n"
"void main() {\n"
"   vec4 blue = texture2D(inputImageTexture, textureCoordinate);\n"
"   vec4 green = texture2D(inputImageTexture, vec2(textureCoordinate.x + textureCoordinateOffset, textureCoordinate.y + textureCoordinateOffset));"
"   vec4 red = texture2D(inputImageTexture, vec2(textureCoordinate.x - textureCoordinateOffset, textureCoordinate.y - textureCoordinateOffset));"
"   gl_FragColor = vec4(red.r, green.g, blue.b, blue.a);"
"}";

static const char* flashWriteFragmentShader =
"varying vec2 textureCoordinate;\n"
"uniform sampler2D inputImageTexture;\n"
"uniform float exposureColor;\n"
"void main() {\n"
"   vec4 textureColor = texture2D(inputImageTexture, textureCoordinate);\n"
"   gl_FragColor = vec4(textureColor.r + exposureColor, textureColor.g + exposureColor, textureColor.b + exposureColor, textureColor.a);\n"
"}\n";

static const char* glitchFilterFragmentShader =
"varying vec2 textureCoordinate;\n"
"uniform sampler2D inputImageTexture;\n"
"uniform vec2 scanLineJitter;\n"
"uniform float colorDrift;\n"
"float rand(float x, float y) {\n"
"   return fract(sin(dot(vec2(x, y), vec2(12.9898, 78.233))) * 43758.5453);\n"
"}\n"
"void main() {;\n"
"   float x = textureCoordinate.x;\n"
"   float y = textureCoordinate.y;\n"
"   float jitter = rand(y, 0.0) * 2.0 - 1.0;\n"
"   float drift = colorDrift;\n"
"   float offsetParam = step(scanLineJitter.y, abs(jitter));\n"
"   jitter = jitter * offsetParam * scanLineJitter.x;\n"
"   vec4 color1 = texture2D(inputImageTexture, fract(vec2(x + jitter, y)));\n"
"   vec4 color2 = texture2D(inputImageTexture, fract(vec2(x + jitter + y * drift, y)));\n"
"   gl_FragColor = vec4(color1.r, color2.g, color1.b, 1.0);\n"
"}";

static const char* blendFragmentShader =
    "varying vec2 textureCoordinate;\n"
    "uniform sampler2D inputImageTexture;\n"
    "uniform sampler2D maskTexture;\n"
    "uniform float strength;\n"
    "void main() {\n"
    "   float mask = (textureCoordinate.x < 1.0 - strength) ? 0.0 : 1.0;\n"
    "   gl_FragColor = texture2D(inputImageTexture, textureCoordinate);\n"
    "   gl_FragColor = mix(gl_FragColor, texture2D(maskTexture, textureCoordinate), mask);\n"
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

#endif /* shader_h */
