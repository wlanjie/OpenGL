//
//  blur.h
//  OpenGL
//
//  Created by wlanjie on 2018/6/3.
//  Copyright © 2018年 com.wlanjie.opengl. All rights reserved.
//

#ifndef blur_h
#define blur_h

#include "framebuffer.h"
#include "opengl_rendering.h"
#include <string>

class Blur {
public:
    Blur(const char* filename, bool smoothlyScaleOutput = false);
    ~Blur();
    int processImage(bool synchronously = false);
    
private:
    void setBlurStrength(float strength);
    
    GLuint frameBufferId;
    GLuint textureId;
    
    GLuint secondFrameBufferId;
    GLuint secondTextureId;
    
    GLuint thirdFrameBufferId;
    GLuint thirdTextureId;
    
    GLfloat secondVertexCoord[8];
    GLfloat secondTextureCoord[8];
    
    float scaleFactorX;
    float scaleFactorY;
    
    uint32_t viewWidth;
    uint32_t viewHeight;
    uint32_t dstWidth;
    uint32_t dstHeight;
    
    ShaderProgram* secondShader;
    ShaderProgram* shader;
    Framebuffer *imageFramebuffer;
    OpenGLRendering* rendering;
    Color* backgroundColor;
    std::vector<InputTextureProperties*> inputTextures;
    GLuint vbo;
    GLSize* size;
    
    uint32_t _blurRadiusInPixels;
    uint32_t _downSamplingPow2;
    std::string vertexSecondShader;
    std::string fragmentSecondShader;
    
    int vertexCoordSecondAttrib;
    int textureCoordSecondAttrib;
    int textureWidthOffset;
    int textureHeightOffset;
    int samplerSecond;
    int _dstSecondTex;
    
    void vertexShaderForOptimizedBlurOfRadius(uint32_t blurRadius, float sigma);
    void fragmentShaderForOptimizedBlurOfRadius(uint32_t blurRadius, float sigma);
};

#endif /* blur_h */
