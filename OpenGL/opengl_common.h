//
//  opengl_common.h
//
//  Created by Hualong.Jiao on 27/09/2017.
//  Copyright © 2017 SureVideo. All rights reserved.
//
#pragma once

#ifdef _USE_OPENGL20_

#if defined ANDROID
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#else
#include <OpenGLES/ES2/gl.h>
#include <OpenGLES/ES2/glext.h>
#endif

#else

#if defined ANDROID
#include <GLES3/gl3.h>
#include <EGL/egl.h>
#include <EGL/eglext.h>
#else
#include <OpenGL/OpenGL.h>
#include <OpenGl/gl.h>
#endif

#endif

#define _DEBUG_LOG  0
    
#if _DEBUG_LOG
    
#ifdef ANDROID
    
#define CHECK_OPENGL_RESULT_LOG(msg) \
{ \
GLenum result = glGetError(); \
if (result != GL_NO_ERROR) \
{ \
__android_log_print(ANDROID_LOG_ERROR, "SureVideo","Error code: %s, %d\n", GetRenderErrorCodeString(result), result); \
__android_log_print(ANDROID_LOG_ERROR, "SureVideo","%s", msg); \
} \
} \

#define LOGE(msg) \
{ \
__android_log_print(ANDROID_LOG_ERROR, "SureVideo","%s", msg); \
} \

#else
    
#define CHECK_OPENGL_RESULT_LOG(msg) \
{ \
GLenum result = glGetError(); \
if (result != GL_NO_ERROR) \
{ \
printf("Error code: %s, %d\n", GetRenderErrorCodeString(result), result); \
printf("%s", msg); \
} \
} \

#define LOGE(msg) \
{ \
printf("%s", msg); \
} \

#endif
    
#else
    
#define CHECK_OPENGL_RESULT_LOG(msg)
#define LOGE(msg)
    
#endif
    
enum {
    CF_RGBA,
    CF_RGB,
    CF_YUV,
    CF_Y,
    CF_YCBCR,
    CF_YUVA,
    CF_DEFAULT,
    CF_COUNT
};

const char* GetRenderErrorCodeString(GLenum errorCode);
GLuint      loadShader(GLenum shaderType, const char *pSource);
void        getProgramLog(GLuint program, char *log);
GLuint      createTexture(GLuint textId, GLint internalformat, GLenum format, const int width, const int height, uint8_t *data);
int32_t     bindTextureWithContext(GLuint clrFmtSrc, uint32_t dstWidth, uint32_t dstHeight, GLuint *fboId, GLuint *txtId);
#if MAX_NUM_LOADTEXT_PBOS > 0
GLuint      createTextureWithPBO(GLuint textId, GLint internalformat, GLenum format, const int width, const int height, uint8_t *data, GLuint *pboHandles, uint32_t pboSize, uint32_t currPBOIdx, uint32_t nextPBOIdx);
#endif

static const char _sVertexShaderText[] =
    "attribute vec4 position;     \n"
    "attribute vec2 vtexcoord;    \n"
    "varying vec2 varTexcoord;    \n"
    "void main (void)             \n"
    "{                            \n"
    "   gl_Position = position;   \n"
    "   varTexcoord = vtexcoord;  \n"
    "}                            \n";

static const char _sRgbInputFragmentShaderText[] =
    "precision highp float;                                     \n"
    "varying vec2 varTexcoord;                                  \n"
    "uniform sampler2D text_rgb;                                \n"
    "void main (void)                                           \n"
    "{                                                          \n"
    " const mat3 rgb2yuv = mat3(0.257, -0.148, 0.439,           \n"
    "                           0.504, -0.291, -0.368,          \n"
    "                           0.098,  0.439, -0.071);         \n"
    "   gl_FragColor = texture2D(text_rgb, varTexcoord);        \n"
    "   vec3 yuv = rgb2yuv * gl_FragColor.rgb + vec3(0.0625,0.5,0.5);  \n"
    "   gl_FragColor.rgb = clamp(yuv,0.0,1.0);                  \n"
    "}                                                          \n";

static const char _sYuvaInputFragmentShaderText[] =
    "precision highp float;                                     \n"
    "varying vec2 varTexcoord;                                  \n"
    "uniform sampler2D text_rgb;                                \n"
    "void main (void)                                           \n"
    "{                                                          \n"
    "   gl_FragColor = texture2D(text_rgb, varTexcoord);        \n"
    "}                                                          \n";

static const char _sYuvInputFragmentShaderText[] =
    "precision highp float;                                       \n"
    "varying vec2 varTexcoord;                                    \n"
    "uniform sampler2D text_y;                                    \n"
    "uniform sampler2D text_u;                                    \n"
    "uniform sampler2D text_v;                                    \n"
    "void main (void)                                             \n"
    "{                                                            \n"
    "   gl_FragColor.r = texture2D(text_y, varTexcoord).r;        \n"
    "   gl_FragColor.g = texture2D(text_u, varTexcoord).r;        \n"
    "   gl_FragColor.b = texture2D(text_v, varTexcoord).r;        \n"
    "   gl_FragColor.a = 1.0;                                     \n"
    "}                                                            \n";

static const char _sYCbCrInputFragmentShaderText[] =
    "precision highp float;                                       \n"
    "varying vec2 varTexcoord;                                    \n"
    "uniform sampler2D text_y;                                    \n"
    "uniform sampler2D text_uv;                                   \n"
    "void main (void)                                             \n"
    "{                                                            \n"
    "   gl_FragColor.r = texture2D(text_y,varTexcoord).x;         \n"
    "   gl_FragColor.gb = texture2D(text_uv,varTexcoord).zw;      \n"
    "   gl_FragColor.a = 1.0;                                     \n"
    "}                                                            \n";

/////////////////////////////////////////////////////////

static const char _sYuv2RgbFragmentShaderText[] =
    "precision highp float;                                     \n"
    "varying vec2 varTexcoord;                                  \n"
    "uniform sampler2D text_rgb;                                \n"
    "void main (void)                                           \n"
    "{                                                          \n"
    "   const mat3 yuv2rgb = mat3(1.164,  1.164,  1.164,        \n"
    "                           0.000, -0.392,  2.017,          \n"
    "                           1.596, -0.813,  0.0);           \n"
    "   gl_FragColor = texture2D(text_rgb,varTexcoord);         \n"
    "   vec3 yuv = gl_FragColor.rgb - vec3(0.0625,0.5,0.5);     \n"
    "   gl_FragColor.rgb = yuv2rgb * yuv;                       \n"
    "   gl_FragColor.rgb = clamp(gl_FragColor.rgb,0.0,1.0);     \n"
    "}                                                          \n";
