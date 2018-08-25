//
// Created by wlanjie on 2018/3/18.
//

#ifndef OPENGL_OPENGL_H
#define OPENGL_OPENGL_H
//
//#define STB_IMAGE_IMPLEMENTATION
//#include "stb_image.h"
//
//#include <OpenGL/OpenGL.h>
//
//auto vertexSource =
//"attribute vec4 position;\n "
//"attribute vec4 inputTextureCoordinate;\n \n "
//"varying vec2 textureCoordinate;\n \n "
//"void main()\n {\n     "
//"gl_Position = position;\n     "
//"textureCoordinate = inputTextureCoordinate.xy;\n "
//"}\n ";
//
//auto fragmentMaskSource =
//"varying vec2 textureCoordinate;\n"
//"uniform sampler2D inputImageTexture;\n"
//"void main() {\n"
//"   gl_FragColor = texture2D(inputImageTexture, textureCoordinate);\n"
//"   float radius = distance(textureCoordinate, vec2(0.5, 0.5)); \n"
//"   float alpha = (radius >= 0.495)? 0.5 : ((radius <= 0.490)? 0.0 : (radius-0.490)*200.0);  \n"
//"   gl_FragColor = mix(gl_FragColor, vec4(1.0, 1.0, 1.0, 1.0), alpha);"
//"}\n";
//
//auto fragmentDianJi =
//"varying vec2 textureCoordinate;\n"
//"uniform sampler2D inputImageTexture;\n"
//"void main() {\n"
//"   vec4 textureColor = texture2D(inputImageTexture, textureCoordinate);\n"
//"   gl_FragColor = vec4((1.0 - textureColor.rgb), textureColor.a);\n"
//"}\n";
//
//auto fragmentGreySource =
//"varying vec2 textureCoordinate;\n"
//"uniform sampler2D inputImageTexture;\n"
//"const vec3 grey = vec3(0.3, 0.59, 0.11);\n"
//"void main() {\n"
//"   vec4 textureColor = texture2D(inputImageTexture, textureCoordinate);\n"
//"   float luminance = dot(textureColor.rgb, grey);\n"
//"   gl_FragColor = vec4(vec3(luminance), textureColor.a);\n"
//"}\n";
//
//auto fragmentSource =
//"varying vec2 textureCoordinate;\n"
//"uniform sampler2D inputImageTexture;\n"
//"void main() {\n"
//"   vec4 textureColor = texture2D(inputImageTexture, textureCoordinate);\n"
//"   gl_FragColor = textureColor;\n"
//"}\n";
//
//GLfloat vertex[] = {
//    -1.0f, -1.0f,
//    1.0f, -1.0f,
//    -1.0f, 1.0f,
//    1.0f, 1.0f,
//};
//
//GLfloat textureCoordinates[] = {
//    0.0f, 0.0f,
//    1.0f, 0.0f,
//    0.0f, 1.0f,
//    1.0f, 1.0f
//};
//
//GLfloat triangleVertex[] = {
//
//    // 左上角
//    -1.0f, 0.0f,
//    0.0f, 1.0f,
//    -1.0f, 1.0f,
//
//    -1.0f, 0.0f,
//    0.0f, 0.0f,
//    0.0f, 1.0f,
//
//    // 右上角
//    0.0f, 0.0f,
//    1.0f, 1.0f,
//    0.0f, 1.0f,
//
//    0.0f, 0.0f,
//    1.0f, 0.0f,
//    1.0f, 1.0f,
//
//    // 左下角
//    -1.0f, -1.0f,
//    0.0f, 0.0f,
//    -1.0f, 0.0f,
//
//    -1.0f, -1.0f,
//    0.0f, -1.0f,
//    0.0f, 0.0f,
//
//    // 右下角
//    0.0f, -1.0f,
//    1.0f, 0.0f,
//    0.0f, 0.0f,
//
//    0.0f, -1.0f,
//    1.0f, -1.0f,
//    1.0f, 0.0f,
//
//};
//
//GLfloat triangleTexture[] = {
//    // 左上角
//    0.0f, 0.0f,
//    1.0f, 1.0f,
//    0.0f, 1.0f,
//
//    0.0f, 0.0f,
//    1.0f, 0.0f,
//    1.0f, 1.0f,
//
//    // 右上角
//    1.0f, 0.0f,
//    0.0f, 1.0f,
//    1.0f, 1.0f,
//
//    1.0f, 0.0f,
//    0.0f, 0.0f,
//    0.0f, 1.0f,
//
//    // 左下角
//    //    1.0f, 1.0f,
//    0.0f, 1.0f,
//    1.0f, 0.0f,
//    0.0f, 0.0f,
//
//    0.0f, 1.0f,
//    1.0f, 1.0f,
//    1.0f, 0.0f,
//
//    // 右下角
//    1.0f, 1.0f,
//    0.0f, 0.0f,
//    1.0f, 0.0f,
//
//    1.0f, 1.0f,
//    0.0f, 1.0f,
//    0.0f, 0.0f
//};

#endif //OPENGL_OPENGL_H
