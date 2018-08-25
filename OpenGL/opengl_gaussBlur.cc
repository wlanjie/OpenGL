//
//  opengl_gaussBlur.cc
//
//  Created by Hualong.Jiao on 27/09/2017.
//  Copyright © 2017 SureVideo. All rights reserved.
//
#include <stdlib.h>
#include <stdio.h>
#include <memory.h>
#include <string>
#include <math.h>
#import <sys/time.h>

#ifdef ANDROID
#include <android/log.h>
#endif

#include "opengl_common.h"
#include "opengl_gaussBlur.h"
 
opengl_gaussBlur::opengl_gaussBlur(uint16_t colorFormat) {
    _sSecondVerticesCoord[0] = -1.0;
    _sSecondVerticesCoord[1] = -1.0;
    
    _sSecondVerticesCoord[2] = 1.0;
    _sSecondVerticesCoord[3] = -1.0;
    
    _sSecondVerticesCoord[4] = -1.0;
    _sSecondVerticesCoord[5] = 1.0;
    
    _sSecondVerticesCoord[6] = 1.0;
    _sSecondVerticesCoord[7] = 1.0;
    
    ///////////////
    
    _sSecondTextureCoord[0] = 0.0;
    _sSecondTextureCoord[1] = 0.0;
    
    _sSecondTextureCoord[2] = 1.0;
    _sSecondTextureCoord[3] = 0.0;
    
    _sSecondTextureCoord[4] = 0.0;
    _sSecondTextureCoord[5] = 1.0;
    
    _sSecondTextureCoord[6] = 1.0;
    _sSecondTextureCoord[7] = 1.0;
    
    _scaleFactorX = 1.0;
    _scaleFactorY = 1.0;
    
    _viewWidth = static_cast<uint32_t>((_dstWidth>>_downSamplingPow2) * _scaleFactorX+0.5);
    _viewHeight = static_cast<uint32_t>((_dstHeight>>_downSamplingPow2) * _scaleFactorY+0.5);
    
    _blurRadiusInPixels = 0;
    _downSamplingPow2 = 1;
    _prgSecondName = 0;
    
    this->setBlurStrength(0.1);
    
    return;
}
    
opengl_gaussBlur::~opengl_gaussBlur(void)
{
    if (_prgSecondName) {
        glDeleteProgram(_prgSecondName);
        CHECK_OPENGL_RESULT_LOG("Delete program failed.\n");
        _prgSecondName = 0;
    }
    
    if (_srcSecondTex) {
        glDeleteTextures(1, &_srcSecondTex);
        CHECK_OPENGL_RESULT_LOG("Delete source texture failed.\n");
        _srcSecondTex = 0;
    }
    
    if (_dstSecondTex) {
        glDeleteTextures(1, &_dstSecondTex);
        CHECK_OPENGL_RESULT_LOG("Delete source texture failed.\n");
        _dstSecondTex = 0;
    }
    
    if (_dstThirdTex) {
        glDeleteTextures(1, &_dstThirdTex);
        CHECK_OPENGL_RESULT_LOG("Delete source texture failed.\n");
        _dstThirdTex = 0;
    }
    
    if (_dstFourthTex) {
        glDeleteTextures(1, &_dstFourthTex);
        CHECK_OPENGL_RESULT_LOG("Delete source texture failed.\n");
        _dstFourthTex = 0;
    }
    
    if (_fboSecondHandle) {
        glDeleteFramebuffers(1,&_fboSecondHandle);
        CHECK_OPENGL_RESULT_LOG("Delete FBO framebuffer failed.\n");
        _fboSecondHandle = 0;
    }
    
    if (_fboThirdHandle) {
        glDeleteFramebuffers(1,&_fboThirdHandle);
        CHECK_OPENGL_RESULT_LOG("Delete FBO framebuffer failed.\n");
        _fboThirdHandle = 0;
    }
    
    if (_fboFourthHandle) {
        glDeleteFramebuffers(1,&_fboFourthHandle);
        CHECK_OPENGL_RESULT_LOG("Delete FBO framebuffer failed.\n");
        _fboFourthHandle = 0;
    }
    
    return;
}
 
#define MAX_STRING_SIZE     4096
#define M_PI                3.14159265358979323846264338327950288
#define MIN(x,y)            ( (x) < (y) ? (x) : (y) )

void opengl_gaussBlur::vertexShaderForOptimizedBlurOfRadius(uint32_t blurRadius, float sigma)
{
        if (blurRadius < 1)
        {
            return;
        }
        
        // First, generate the normal Gaussian weights for a given sigma
        float *standardGaussianWeights = new float[blurRadius + 1];
        float sumOfWeights = 0.0;
        for (uint32_t currentGaussianWeightIndex = 0; currentGaussianWeightIndex < blurRadius + 1; currentGaussianWeightIndex++)
        {
            standardGaussianWeights[currentGaussianWeightIndex] = (1.0 / sqrt(2.0 * M_PI * pow(sigma, 2.0))) * exp(-pow(currentGaussianWeightIndex, 2.0) / (2.0 * pow(sigma, 2.0)));
            
            if (currentGaussianWeightIndex == 0)
            {
                sumOfWeights += standardGaussianWeights[currentGaussianWeightIndex];
            }
            else
            {
                sumOfWeights += 2.0 * standardGaussianWeights[currentGaussianWeightIndex];
            }
        }
        
        // Next, normalize these weights to prevent the clipping of the Gaussian curve at the end of the discrete samples from reducing luminance
        for (uint32_t currentGaussianWeightIndex = 0; currentGaussianWeightIndex < blurRadius + 1; currentGaussianWeightIndex++)
        {
            standardGaussianWeights[currentGaussianWeightIndex] = standardGaussianWeights[currentGaussianWeightIndex] / sumOfWeights;
        }
        
        // From these weights we calculate the offsets to read interpolated values from
        uint32_t numberOfOptimizedOffsets = MIN(blurRadius / 2 + (blurRadius % 2), 7);
        float *optimizedGaussianOffsets = new float[numberOfOptimizedOffsets];
        
        for (uint32_t currentOptimizedOffset = 0; currentOptimizedOffset < numberOfOptimizedOffsets; currentOptimizedOffset++)
        {
            GLfloat firstWeight = standardGaussianWeights[currentOptimizedOffset*2 + 1];
            GLfloat secondWeight = standardGaussianWeights[currentOptimizedOffset*2 + 2];
            
            GLfloat optimizedWeight = firstWeight + secondWeight;
            
            optimizedGaussianOffsets[currentOptimizedOffset] = (firstWeight * (currentOptimizedOffset*2 + 1) + secondWeight * (currentOptimizedOffset*2 + 2)) / optimizedWeight;
        }
    
        char tmpStr[MAX_STRING_SIZE];
    
        sprintf(tmpStr,"\
                     attribute vec4 position;\n\
                     attribute vec4 inputTextureCoordinate;\n\
                     \n\
                     uniform float texelWidthOffset;\n\
                     uniform float texelHeightOffset;\n\
                     \n\
                     varying vec2 blurCoordinates[%lu];\n\
                     \n\
                     void main()\n\
                     {\n\
                     gl_Position = position;\n\
                     \n\
                     vec2 singleStepOffset = vec2(texelWidthOffset, texelHeightOffset);\n",(unsigned long)(1 + (numberOfOptimizedOffsets * 2)));

        string shaderString(tmpStr);
    
        // Inner offset loop
        sprintf(tmpStr,"blurCoordinates[0] = inputTextureCoordinate.xy;\n");
    
        shaderString.append(tmpStr);
    
        for (uint32_t currentOptimizedOffset = 0; currentOptimizedOffset < numberOfOptimizedOffsets; currentOptimizedOffset++)
        {
            sprintf(tmpStr,"\
                         blurCoordinates[%lu] = inputTextureCoordinate.xy + singleStepOffset * %f;\n\
                         blurCoordinates[%lu] = inputTextureCoordinate.xy - singleStepOffset * %f;\n",(unsigned long)((currentOptimizedOffset * 2) + 1), optimizedGaussianOffsets[currentOptimizedOffset], (unsigned long)((currentOptimizedOffset * 2) + 2), optimizedGaussianOffsets[currentOptimizedOffset]);
            
            shaderString.append(tmpStr);
        }

        // Footer
        shaderString.append("}\n");
    
        delete [] optimizedGaussianOffsets;
        delete [] standardGaussianWeights;
    
        _vertexSecondShaderText = shaderString;
    
        return;
}
    
void opengl_gaussBlur::fragmentShaderForOptimizedBlurOfRadius(uint32_t blurRadius, float sigma)
{
        if (blurRadius < 1)
        {
            return;
        }
        
        // First, generate the normal Gaussian weights for a given sigma
        float *standardGaussianWeights = new float[blurRadius + 1];
        float sumOfWeights = 0.0;
        for (uint32_t currentGaussianWeightIndex = 0; currentGaussianWeightIndex < blurRadius + 1; currentGaussianWeightIndex++)
        {
            standardGaussianWeights[currentGaussianWeightIndex] = (1.0 / sqrt(2.0 * M_PI * pow(sigma, 2.0))) * exp(-pow(currentGaussianWeightIndex, 2.0) / (2.0 * pow(sigma, 2.0)));
            
            if (currentGaussianWeightIndex == 0)
            {
                sumOfWeights += standardGaussianWeights[currentGaussianWeightIndex];
            }
            else
            {
                sumOfWeights += 2.0 * standardGaussianWeights[currentGaussianWeightIndex];
            }
        }
        
        // Next, normalize these weights to prevent the clipping of the Gaussian curve at the end of the discrete samples from reducing luminance
        for (uint32_t currentGaussianWeightIndex = 0; currentGaussianWeightIndex < blurRadius + 1; currentGaussianWeightIndex++)
        {
            standardGaussianWeights[currentGaussianWeightIndex] = standardGaussianWeights[currentGaussianWeightIndex] / sumOfWeights;
        }
        
        // From these weights we calculate the offsets to read interpolated values from
        uint32_t numberOfOptimizedOffsets = MIN(blurRadius / 2 + (blurRadius % 2), 7);
        uint32_t trueNumberOfOptimizedOffsets = blurRadius / 2 + (blurRadius % 2);
    
        char tmpStr[MAX_STRING_SIZE];

        sprintf(tmpStr,"\
                     precision highp float;\n\
                     uniform sampler2D inputImageTexture;\n\
                     uniform float texelWidthOffset;\n\
                     uniform float texelHeightOffset;\n\
                     \n\
                     varying vec2 blurCoordinates[%lu];\n\
                     \n\
                     void main()\n\
                     {\n\
                     vec4 sum = vec4(0.0);\n",(unsigned long)(1 + (numberOfOptimizedOffsets * 2)));
    
        string shaderString(tmpStr);

        // Inner texture loop
        sprintf(tmpStr,"sum += texture2D(inputImageTexture, blurCoordinates[0]) * %f;\n", standardGaussianWeights[0]);
    
        shaderString.append(tmpStr);

        for (uint32_t currentBlurCoordinateIndex = 0; currentBlurCoordinateIndex < numberOfOptimizedOffsets; currentBlurCoordinateIndex++)
        {
            float firstWeight = standardGaussianWeights[currentBlurCoordinateIndex * 2 + 1];
            float secondWeight = standardGaussianWeights[currentBlurCoordinateIndex * 2 + 2];
            float optimizedWeight = firstWeight + secondWeight;
            
            sprintf(tmpStr,"sum += texture2D(inputImageTexture, blurCoordinates[%lu]) * %f;\n", (unsigned long)((currentBlurCoordinateIndex * 2) + 1), optimizedWeight);
            shaderString.append(tmpStr);
            
            sprintf(tmpStr,"sum += texture2D(inputImageTexture, blurCoordinates[%lu]) * %f;\n", (unsigned long)((currentBlurCoordinateIndex * 2) + 2), optimizedWeight);
            shaderString.append(tmpStr);
        }
        
        // If the number of required samples exceeds the amount we can pass in via varyings, we have to do dependent texture reads in the fragment shader
        if (trueNumberOfOptimizedOffsets > numberOfOptimizedOffsets)
        {
            sprintf(tmpStr,"vec2 singleStepOffset = vec2(texelWidthOffset, texelHeightOffset);\n");
            shaderString.append(tmpStr);
            
            for (uint32_t currentOverlowTextureRead = numberOfOptimizedOffsets; currentOverlowTextureRead < trueNumberOfOptimizedOffsets; currentOverlowTextureRead++)
            {
                float firstWeight = standardGaussianWeights[currentOverlowTextureRead * 2 + 1];
                float secondWeight = standardGaussianWeights[currentOverlowTextureRead * 2 + 2];
                
                float optimizedWeight = firstWeight + secondWeight;
                float optimizedOffset = (firstWeight * (currentOverlowTextureRead * 2 + 1) + secondWeight * (currentOverlowTextureRead * 2 + 2)) / optimizedWeight;
                
                sprintf(tmpStr,"sum += texture2D(inputImageTexture, blurCoordinates[0] + singleStepOffset * %f) * %f;\n", optimizedOffset, optimizedWeight);
                shaderString.append(tmpStr);

                sprintf(tmpStr,"sum += texture2D(inputImageTexture, blurCoordinates[0] - singleStepOffset * %f) * %f;\n", optimizedOffset, optimizedWeight);
                shaderString.append(tmpStr);
            }
        }
        
        // Footer
        shaderString.append("\
                         gl_FragColor = sum;\n\
                         }\n");

        delete [] standardGaussianWeights;
    
        _fragmentSecondShaderText = shaderString;

        return;
}

void opengl_gaussBlur::switchShader(void)
{
    int32_t retProgram = 0;
    
    retProgram = linkShader(_vertexSecondShaderText.c_str(), _fragmentSecondShaderText.c_str());
    
    if (retProgram < 0) {
        return;
    }
    
    if (_prgSecondName) {
        glDeleteProgram(_prgSecondName);
        CHECK_OPENGL_RESULT_LOG("Delete program failed.\n");
        _prgSecondName = 0;
    }
    
    //populate pgm
    _prgSecondName = (GLuint)retProgram;
    
    _verticesCoordSecondAttribLoc = glGetAttribLocation(_prgSecondName, "position");
    _texturesCoordSecondAttribLoc = glGetAttribLocation(_prgSecondName, "inputTextureCoordinate");
    _texelWidthOffsetLoc = glGetUniformLocation(_prgSecondName, "texelWidthOffset");
    _texelHeightOffsetLoc = glGetUniformLocation(_prgSecondName, "texelHeightOffset");    
    _samplerSecondLoc = glGetUniformLocation(_prgSecondName, "inputImageTexture");
    
    return;
}
    
uint32_t opengl_gaussBlur::setBlurStrength(float strength)
{
    // 7.0 is the limit for blur size for hardcoded varying offsets
    float newValue = 60.0*strength;
    uint32_t radius = round(newValue);

    radius >>= _downSamplingPow2;
    
    if (radius != _blurRadiusInPixels)
    {
        _blurRadiusInPixels = radius; // For now, only do integral sigmas
        
        uint32_t calculatedSampleRadius = 0;
        if (_blurRadiusInPixels >= 1) // Avoid a divide-by-zero error here
        {
            // Calculate the number of pixels to sample from by setting a bottom limit for the contribution of the outermost pixel
            float minimumWeightToFindEdgeOfSamplingArea = 1.0/256.0;
            calculatedSampleRadius = floor(sqrt(-2.0 * pow(_blurRadiusInPixels, 2.0) * log(minimumWeightToFindEdgeOfSamplingArea * sqrt(2.0 * M_PI * pow(_blurRadiusInPixels, 2.0))) ));
            calculatedSampleRadius += calculatedSampleRadius % 2; // There's nothing to gain from handling odd radius sizes, due to the optimizations I use
            
            vertexShaderForOptimizedBlurOfRadius(calculatedSampleRadius, _blurRadiusInPixels);
            fragmentShaderForOptimizedBlurOfRadius(calculatedSampleRadius, _blurRadiusInPixels);
            
            switchShader();
        }
    }
    
    return 0;
}
    
uint32_t opengl_gaussBlur::setBlurRegion(float left, float top, float right, float bottom)
{
    left = (left < 0)? 0 : ( (left > 1) ? 1 : left );
    top = (top < 0)? 0 : ( (top > 1) ? 1 : top );
    right = (right < left)? left : ( (right > 1) ? 1 : right );
    bottom = (bottom < top)? top : ( (bottom > 1) ? 1 : bottom );
    
    _sSecondVerticesCoord[0] = (left - 0.5) * 2.0;
    _sSecondVerticesCoord[1] = (top - 0.5) * 2.0;

    _sSecondVerticesCoord[2] = (right - 0.5) * 2.0;
    _sSecondVerticesCoord[3] = (top - 0.5) * 2.0;

    _sSecondVerticesCoord[4] = (left - 0.5) * 2.0;
    _sSecondVerticesCoord[5] = (bottom - 0.5) * 2.0;

    _sSecondVerticesCoord[6] = (right - 0.5) * 2.0;
    _sSecondVerticesCoord[7] = (bottom - 0.5) * 2.0;

    ///////////////

    _sSecondTextureCoord[0] = left;
    _sSecondTextureCoord[1] = top;

    _sSecondTextureCoord[2] = right;
    _sSecondTextureCoord[3] = top;

    _sSecondTextureCoord[4] = left;
    _sSecondTextureCoord[5] = bottom;

    _sSecondTextureCoord[6] = right;
    _sSecondTextureCoord[7] = bottom;
    
    _scaleFactorX = right - left;
    _scaleFactorY = bottom - top;
    
    _viewWidth = static_cast<uint32_t>((_dstWidth>>_downSamplingPow2) * _scaleFactorX+0.5);
    _viewHeight = static_cast<uint32_t>((_dstHeight>>_downSamplingPow2) * _scaleFactorY+0.5);
    
    setupFBO();
    
    return 0;
}
    
int32_t opengl_gaussBlur::setupFBO(void)
{
    int32_t retCode = 0;
        
    retCode = opengl_imgproc_baseclass::setupFBO();
        
    if (retCode < 0) {
        return retCode;
    }
        
    retCode = opengl_imgproc_baseclass::bindTextureWithResolution(&_fboSecondHandle,&_dstSecondTex,_viewWidth,_viewHeight);
    
    if (retCode < 0) {
        return retCode;
    }
    
    retCode = opengl_imgproc_baseclass::bindTextureWithResolution(&_fboThirdHandle,&_dstThirdTex,_viewWidth,_viewHeight);
    
    if (retCode < 0) {
        return retCode;
    }
    
    retCode = opengl_imgproc_baseclass::bindTexture(&_fboFourthHandle,&_dstFourthTex);

    return retCode;
}
    
int32_t opengl_gaussBlur::setDstResolution(uint16_t width,uint16_t height)
{
    if (width > 0x3fff || width == 0
        || height > 0x3fff || height == 0 ||
        (_dstWidth == width && _dstHeight == height)) {
        return 0;
    }
        
    _dstWidth = width;
    _dstHeight = height;
    
    _viewWidth = static_cast<uint32_t>((_dstWidth>>_downSamplingPow2) * _scaleFactorX+0.5);
    _viewHeight = static_cast<uint32_t>((_dstHeight>>_downSamplingPow2) * _scaleFactorY+0.5);
        
    setupFBO();
        
    return 0;
}

int32_t opengl_gaussBlur::process(void)
{
    GLint defaultFBO = 0;
    
//    glGetIntegerv(GL_FRAMEBUFFER_BINDING, &defaultFBO);
//    CHECK_OPENGL_RESULT_LOG("opengl get frame buffer binding.\n");
    glClearColor(0.0625, 0.5, 0.5, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    ////////////////////////////////////////////////////////////////////////////////
    if (_blurRadiusInPixels >= 1) {
        glBindFramebuffer(GL_FRAMEBUFFER, _fboSecondHandle);
        CHECK_OPENGL_RESULT_LOG("opengl bind frame buffer failed.\n");
        
        glViewport(0,0,_viewWidth,_viewHeight);
        
        glUseProgram(_prgSecondName);
        CHECK_OPENGL_RESULT_LOG("opengl use program failed.\n");
        
        glActiveTexture(GL_TEXTURE0+0);
        CHECK_OPENGL_RESULT_LOG("opengl active y texture failed.\n");
        
        glBindTexture(GL_TEXTURE_2D, _srcTex[0]);
        glUniform1i(_samplerSecondLoc, 0);
        
        glVertexAttribPointer(_texturesCoordSecondAttribLoc, 2, GL_FLOAT, GL_FALSE, 2*sizeof(GLfloat), _sSecondTextureCoord);
        CHECK_OPENGL_RESULT_LOG("opengl create texture coord failed.\n");
        glEnableVertexAttribArray(_texturesCoordSecondAttribLoc);
        CHECK_OPENGL_RESULT_LOG("opengl enable texture attr array coord failed.\n");
        
        glVertexAttribPointer(_verticesCoordSecondAttribLoc, 2, GL_FLOAT, GL_FALSE, 2*sizeof(GLfloat), _sVerticesCoord);
        CHECK_OPENGL_RESULT_LOG("opengl create vertices failed.\n");
        glEnableVertexAttribArray(_verticesCoordSecondAttribLoc);
        CHECK_OPENGL_RESULT_LOG("opengl enable vertex attr array vertices failed.\n");
        
        texelWidthOffset = 1.0f/(_dstWidth>>_downSamplingPow2);
        texelHeightOffset = 0.0f;
        
        glUniform1f(_texelWidthOffsetLoc,texelWidthOffset);
        glUniform1f(_texelHeightOffsetLoc,texelHeightOffset);
        
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        CHECK_OPENGL_RESULT_LOG("opengl draw array failed.\n");
        
        glFlush();
        
        /////////////////////////////////////////////
        
        glBindFramebuffer(GL_FRAMEBUFFER, _fboThirdHandle);
        CHECK_OPENGL_RESULT_LOG("opengl bind frame buffer failed.\n");
        
        glViewport(0,0, _viewWidth,_viewHeight);
        
        glUseProgram(_prgSecondName);
        CHECK_OPENGL_RESULT_LOG("opengl use program failed.\n");
        
        glActiveTexture(GL_TEXTURE0+0);
        CHECK_OPENGL_RESULT_LOG("opengl active y texture failed.\n");
        
        glBindTexture(GL_TEXTURE_2D, _dstSecondTex);
        glUniform1i(_samplerSecondLoc, 0);
        
        glVertexAttribPointer(_texturesCoordSecondAttribLoc, 2, GL_FLOAT, GL_FALSE, 2*sizeof(GLfloat), _sTextureCoord);
        CHECK_OPENGL_RESULT_LOG("opengl create texture coord failed.\n");
        glEnableVertexAttribArray(_texturesCoordSecondAttribLoc);
        CHECK_OPENGL_RESULT_LOG("opengl enable texture attr array coord failed.\n");
        
        glVertexAttribPointer(_verticesCoordSecondAttribLoc, 2, GL_FLOAT, GL_FALSE, 2*sizeof(GLfloat), _sVerticesCoord);
        CHECK_OPENGL_RESULT_LOG("opengl create vertices failed.\n");
        glEnableVertexAttribArray(_verticesCoordSecondAttribLoc);
        CHECK_OPENGL_RESULT_LOG("opengl enable vertex attr array vertices failed.\n");
        
        texelWidthOffset = 0.0f;
        texelHeightOffset = 1.0f/_viewHeight;
        
        glUniform1f(_texelWidthOffsetLoc,texelWidthOffset);
        glUniform1f(_texelHeightOffsetLoc,texelHeightOffset);
        
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        CHECK_OPENGL_RESULT_LOG("opengl draw array failed.\n");
        
        glFlush();
        
        /////////////////////////////////////////////////////////////////////////////
        
        glBindFramebuffer(GL_FRAMEBUFFER, _fboHandle);
        CHECK_OPENGL_RESULT_LOG("opengl bind frame buffer failed.\n");
        
        glViewport(0,0, _dstWidth, _dstHeight);
        
        glUseProgram(_prgName);
        CHECK_OPENGL_RESULT_LOG("opengl use program failed.\n");
        
        if (_scaleFactorX < 0.99999 || _scaleFactorY < 0.99999) {
            glActiveTexture(GL_TEXTURE0+0);
            CHECK_OPENGL_RESULT_LOG("opengl active y texture failed.\n");
        
            glBindTexture(GL_TEXTURE_2D, _srcTex[0]);
            glUniform1i(_samplerLoc[0], 0);
        
            glVertexAttribPointer(_texturesCoordAttribLoc, 2, GL_FLOAT, GL_FALSE, 2*sizeof(GLfloat), _sTextureCoord);
            CHECK_OPENGL_RESULT_LOG("opengl create texture coord failed.\n");
            glEnableVertexAttribArray(_texturesCoordAttribLoc);
            CHECK_OPENGL_RESULT_LOG("opengl enable texture attr array coord failed.\n");
        
            glVertexAttribPointer(_verticesCoordAttribLoc, 2, GL_FLOAT, GL_FALSE, 2*sizeof(GLfloat), _sVerticesCoord);
            CHECK_OPENGL_RESULT_LOG("opengl create vertices failed.\n");
            glEnableVertexAttribArray(_verticesCoordAttribLoc);
            CHECK_OPENGL_RESULT_LOG("opengl enable vertex attr array vertices failed.\n");
        
            glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
            CHECK_OPENGL_RESULT_LOG("opengl draw array failed.\n");
        }
        
        glActiveTexture(GL_TEXTURE0+0);
        CHECK_OPENGL_RESULT_LOG("opengl active y texture failed.\n");
        
        glBindTexture(GL_TEXTURE_2D, _dstThirdTex);
        glUniform1i(_samplerLoc[0], 0);
        
        glVertexAttribPointer(_texturesCoordAttribLoc, 2, GL_FLOAT, GL_FALSE, 2*sizeof(GLfloat), _sTextureCoord);
        CHECK_OPENGL_RESULT_LOG("opengl create texture coord failed.\n");
        glEnableVertexAttribArray(_texturesCoordAttribLoc);
        CHECK_OPENGL_RESULT_LOG("opengl enable texture attr array coord failed.\n");
        
        glVertexAttribPointer(_verticesCoordAttribLoc, 2, GL_FLOAT, GL_FALSE, 2*sizeof(GLfloat), _sSecondVerticesCoord);
        CHECK_OPENGL_RESULT_LOG("opengl create vertices failed.\n");
        glEnableVertexAttribArray(_verticesCoordAttribLoc);
        CHECK_OPENGL_RESULT_LOG("opengl enable vertex attr array vertices failed.\n");
        
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        CHECK_OPENGL_RESULT_LOG("opengl draw array failed.\n");
        
        glFlush();
    } else {
        /////////////////////////////////////////////////////////////////////////////
        glBindFramebuffer(GL_FRAMEBUFFER, _fboHandle);
        CHECK_OPENGL_RESULT_LOG("opengl bind frame buffer failed.\n");
        
        glViewport(0,0, _dstWidth, _dstHeight);
        
        glUseProgram(_prgName);
        CHECK_OPENGL_RESULT_LOG("opengl use program failed.\n");
        
        glActiveTexture(GL_TEXTURE0+0);
        CHECK_OPENGL_RESULT_LOG("opengl active y texture failed.\n");
        
        glBindTexture(GL_TEXTURE_2D, _srcTex[0]);
        glUniform1i(_samplerLoc[0], 0);
        
        glVertexAttribPointer(_texturesCoordAttribLoc, 2, GL_FLOAT, GL_FALSE, 2*sizeof(GLfloat),  _sTextureCoord);
        CHECK_OPENGL_RESULT_LOG("opengl create texture coord failed.\n");
        glEnableVertexAttribArray(_texturesCoordAttribLoc);
        CHECK_OPENGL_RESULT_LOG("opengl enable texture attr array coord failed.\n");
        
        glVertexAttribPointer(_verticesCoordAttribLoc, 2, GL_FLOAT, GL_FALSE, 2*sizeof(GLfloat), _sVerticesCoord);
        CHECK_OPENGL_RESULT_LOG("opengl create vertices failed.\n");
        glEnableVertexAttribArray(_verticesCoordAttribLoc);
        CHECK_OPENGL_RESULT_LOG("opengl enable vertex attr array vertices failed.\n");
        
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        CHECK_OPENGL_RESULT_LOG("opengl draw array failed.\n");
        
        glFlush();
    }
    
    glBindFramebuffer(GL_FRAMEBUFFER, defaultFBO);
    CHECK_OPENGL_RESULT_LOG("opengl bind frame buffer to default failed.\n");
            
    return 0;
}
