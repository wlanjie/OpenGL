//
//  opengl_gaussBlur.h
//
//  Created by Hualong.Jiao on 27/09/2017.
//  Copyright © 2017 SureVideo. All rights reserved.
//
#pragma once

#include <string>

using namespace std;

class opengl_gaussBlur {
public:
    
    opengl_gaussBlur(uint16_t colorFormat);
    virtual int32_t process(void);
    virtual ~opengl_gaussBlur(void);
    
    virtual int32_t setupFBO(void);
    virtual int32_t setDstResolution(uint16_t width,uint16_t height);

    uint32_t setBlurStrength(float strength);
    uint32_t setBlurRegion(float left, float top, float right, float bottom);
protected:
    GLuint _prgSecondName;

    GLint _verticesCoordSecondAttribLoc;
    GLint _texturesCoordSecondAttribLoc;
    GLint _samplerSecondLoc;
    GLint _texelWidthOffsetLoc;
    GLint _texelHeightOffsetLoc;
    GLint _singleStepOffsetSecondLoc;
    
    GLuint _srcSecondTex;
    GLfloat _sSecondVerticesCoord[8];
    GLfloat _sSecondTextureCoord[8];
    GLfloat texelWidthOffset;
    GLfloat texelHeightOffset;

    GLuint _fboSecondHandle;
    GLuint _fboThirdHandle;
    GLuint _fboFourthHandle;

    GLuint _dstSecondTex;
    GLuint _dstThirdTex;
    GLuint _dstFourthTex;
    
    uint16_t _dstWidth;
    uint16_t _dstHeight;

private:
    void vertexShaderForOptimizedBlurOfRadius(uint32_t blurRadius, float sigma);
    void fragmentShaderForOptimizedBlurOfRadius(uint32_t blurRadius, float sigma);
    void switchShader(void);

    string _vertexSecondShaderText;
    string _fragmentSecondShaderText;
    
    uint32_t _blurRadiusInPixels;
    uint32_t _downSamplingPow2;
    
    float _scaleFactorX;
    float _scaleFactorY;
    uint32_t _viewWidth;
    uint32_t _viewHeight;
};
