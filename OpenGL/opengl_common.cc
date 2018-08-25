//
//  opengl_common.cc
//
//  Created by Hualong.Jiao on 27/09/2017.
//  Copyright © 2017 SureVideo. All rights reserved.
//
#include <stdlib.h>
#include <stdio.h>
#include <memory.h>
#import <sys/time.h>

#ifdef ANDROID
#include <android/log.h>
#endif

#include "opengl_common.h"

const char* GetRenderErrorCodeString(GLenum errorCode)
{
    switch (errorCode) {
        case GL_NO_ERROR:
            return "GL_NO_ERROR";
            
        case GL_INVALID_ENUM:
            return "GL_INVALID_ENUM";
            
        case GL_INVALID_VALUE:
            return "GL_INVALID_VALUE";
            
        case GL_INVALID_OPERATION:
            return "GL_INVALID_OPERATION";
            
        case GL_OUT_OF_MEMORY:
            return "GL_OUT_OF_MEMORY";
            
        default:
            return "UncommonError";
    }
}

GLuint loadShader(GLenum shaderType, const char *pSource)
{
    GLuint shader;
    GLint compiled;
    
    // Create the shader object
    shader = glCreateShader ( shaderType );
    
    if ( shader == 0 ) {
        CHECK_OPENGL_RESULT_LOG("opengl CreateShader failed.\n");
        return 0;
    }
    
    // Load the shader source
    glShaderSource ( shader, 1, &pSource, NULL );
    
    // Compile the shader
    glCompileShader ( shader );
    
    // Check the compile status
    glGetShaderiv ( shader, GL_COMPILE_STATUS, &compiled );
    
    if ( !compiled )
    {
        GLint infoLen = 0;
        
        glGetShaderiv ( shader, GL_INFO_LOG_LENGTH, &infoLen );
        
        if ( infoLen > 1 )
        {
            char* infoLog = (char *)malloc (sizeof(char) * infoLen );
            
            glGetShaderInfoLog ( shader, infoLen, NULL, infoLog );
            //LOGE("shader error log: %s", infoLog);
            
            free ( infoLog );
        }
        
        glDeleteShader ( shader );
        return 0;
    }
    
    return shader;
}

void getProgramLog(GLuint program, char *log)
{
    GLint logLength;
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLength);
    
    if (logLength) {
        char *buf = (char *) malloc(logLength);
        
        if (buf) {
            glGetProgramInfoLog(program, logLength, NULL, buf);
            //LOGE("program error log: %s", buf);
            free(buf);
        } else {
            //LOGE("ERROR: malloc return 0");
        }
    }
}

GLuint createTexture(GLuint textId, GLint internalformat, GLenum format, const int width, const int height, uint8_t *data)
{
    if (!textId) {
        glGenTextures(1, &textId);
    }
    
    glBindTexture(GL_TEXTURE_2D, textId);
    CHECK_OPENGL_RESULT_LOG("opengl bind texture failed.\n");
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    
    glTexImage2D(GL_TEXTURE_2D, 0, internalformat, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    
    return textId;
}

int32_t bindTextureWithContext(GLuint clrFmtSrc,uint32_t dstWidth, uint32_t dstHeight,GLuint *fboId,GLuint *txtId)
{
    int32_t retCode = 0;
    GLint defaultFBO = 0;
    
//    glGetIntegerv(GL_FRAMEBUFFER_BINDING, &defaultFBO);
//    CHECK_OPENGL_RESULT_LOG("opengl get frame buffer binding.\n");
    
    if (!(*fboId)) {
        glGenFramebuffers(1, fboId);
    }
    
    glBindFramebuffer(GL_FRAMEBUFFER, *fboId);
    CHECK_OPENGL_RESULT_LOG("opengl bind frame buffer failed.\n");
        
    if (*txtId) {
        glDeleteTextures(1, txtId);
        CHECK_OPENGL_RESULT_LOG("Delete destination texture failed.\n");
        *txtId = 0;
    }
        
    if (clrFmtSrc != CF_Y) {
        *txtId = createTexture(*txtId, GL_RGBA, GL_RGBA, dstWidth, dstHeight, 0);
    } else {
#ifdef _USE_OPENGL20_
        *txtId = createTexture(*txtId, GL_RED_EXT, GL_RED_EXT, dstWidth, dstHeight, 0);
#else
        *txtId = createTexture(*txtId, GL_LUMINANCE, GL_LUMINANCE, dstWidth, dstHeight, 0);
#endif
    }
        
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, *txtId, 0);
    CHECK_OPENGL_RESULT_LOG("opengl dstTex frame buffer texture2d.\n");
        
    // FBO status check
    GLenum status;
    status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    switch(status) {
        case GL_FRAMEBUFFER_COMPLETE:
            //LOGI("fbo complete");
            retCode = 0;
            break;
                
        case GL_FRAMEBUFFER_UNSUPPORTED:
            //LOGI("fbo unsupported");
            retCode = -2;
            break;
                
        default:
            // programming error; will fail on all hardware
            // LOGI("Framebuffer Error");
            retCode = -3;
            break;
    }
        
    glBindFramebuffer(GL_FRAMEBUFFER, defaultFBO);
    CHECK_OPENGL_RESULT_LOG("opengl bind frame buffer to default.\n");
    
    return retCode;
}

#if MAX_NUM_LOADTEXT_PBOS > 0

GLuint createTextureWithPBO(GLuint textId, GLint internalformat, GLenum format, const int width, const int height, uint8_t *data, GLuint *pboHandles, uint32_t pboSize, uint32_t currPBOIdx, uint32_t nextPBOIdx)
{
    if (!textId) {
        glGenTextures(1, &textId);
        
        glBindTexture(GL_TEXTURE_2D, textId);
        CHECK_OPENGL_RESULT_LOG("opengl bind texture failed.\n");
        
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    } else {
        glBindTexture(GL_TEXTURE_2D, textId);
        CHECK_OPENGL_RESULT_LOG("opengl bind texture failed.\n");
    }
    
    glBindBuffer(GL_PIXEL_UNPACK_BUFFER, pboHandles[currPBOIdx]);
    glTexImage2D(GL_TEXTURE_2D, 0, internalformat, width, height, 0, format, GL_UNSIGNED_BYTE, NULL);
    
    if (currPBOIdx != nextPBOIdx) {
        glBindBuffer(GL_PIXEL_UNPACK_BUFFER, pboHandles[nextPBOIdx]);
    }
    
    glBufferData(GL_PIXEL_UNPACK_BUFFER, pboSize, NULL, GL_STREAM_DRAW);
    
    GLvoid *pixelUnpackBuffer = glMapBufferRange(GL_PIXEL_UNPACK_BUFFER, 0, pboSize, GL_MAP_WRITE_BIT);
    if (!pixelUnpackBuffer) {
        printf("map buffer range failed.\n");
        exit(-1);
    }
    memcpy(pixelUnpackBuffer, data, pboSize);
    glUnmapBuffer(GL_PIXEL_UNPACK_BUFFER);
    
    glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
    
    return textId;
}

#endif
