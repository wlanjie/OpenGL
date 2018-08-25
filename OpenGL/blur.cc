//
//  blur.cc
//  OpenGL
//
//  Created by wlanjie on 2018/6/3.
//  Copyright © 2018年 com.wlanjie.opengl. All rights reserved.
//
#include "stb_image.h"
#include "blur.h"

#define MAX_STRING_SIZE     4096
#define MIN(x,y)            ( (x) < (y) ? (x) : (y) )

GLfloat blurVertex[] = {
    -1.0f, -1.0f,
    1.0f, -1.0f,
    -1.0f, 1.0f,
    1.0f, 1.0f,
};

GLfloat blurTextureCoordinates[] = {
    0.0f, 0.0f,
    1.0f, 0.0f,
    0.0f, 1.0f,
    1.0f, 1.0f
};

auto blurVertexShader =
"attribute vec4 position;\n "
"attribute vec4 inputTextureCoordinate;\n \n "
"varying vec2 textureCoordinate;\n \n "
"void main()\n {\n     "
"gl_Position = position;\n     "
"textureCoordinate = inputTextureCoordinate.xy;\n "
"}\n ";

auto blurFragmentSource =
"varying vec2 textureCoordinate;\n"
"uniform sampler2D inputImageTexture;\n"
"uniform sampler2D blurTexture;\n"
"void main() {\n"
"   vec4 textureColor = texture2D(inputImageTexture, textureCoordinate);\n"
"   vec4 blurTextureColor = texture2D(inputImageTexture, textureCoordinate);\n"
"   gl_FragColor = mix(textureColor, blurTextureColor, textureColor.a);\n"
"}\n";

auto renderFragment =
"varying vec2 textureCoordinate;\n"
"uniform sampler2D inputImageTexture;\n"
"void main() {\n"
"   vec4 textureColor = texture2D(inputImageTexture, textureCoordinate);\n"
"   gl_FragColor = textureColor;\n"
"}\n";


Blur::Blur(const char* filename, bool smoothlyScaleOutput) : imageFramebuffer(nullptr) {
    secondVertexCoord[0] = -1.0;
    secondVertexCoord[1] = -1.0;
    
    secondVertexCoord[2] = 1.0;
    secondVertexCoord[3] = -1.0;
    
    secondVertexCoord[4] = -1.0;
    secondVertexCoord[5] = 1.0;
    
    secondVertexCoord[6] = 1.0;
    secondVertexCoord[7] = 1.0;
    
    ///////////////
    
    secondTextureCoord[0] = 0.0;
    secondTextureCoord[1] = 0.0;
    
    secondTextureCoord[2] = 1.0;
    secondTextureCoord[3] = 0.0;
    
    secondTextureCoord[4] = 0.0;
    secondTextureCoord[5] = 1.0;
    
    secondTextureCoord[6] = 1.0;
    secondTextureCoord[7] = 1.0;
    
    _blurRadiusInPixels = 0;
    _downSamplingPow2 = 1;
    
    scaleFactorX = 1.0;
    scaleFactorY = 1.0;
    
    dstWidth = 540;
    dstHeight = 960;
    viewWidth = 540;
    viewHeight = 960;
//    viewWidth = static_cast<uint32_t>((dstWidth>>_downSamplingPow2) * scaleFactorX+0.5);
//    viewHeight = static_cast<uint32_t>((dstHeight>>_downSamplingPow2) * scaleFactorY+0.5);
    
    rendering = new OpenGLRendering();
    backgroundColor = Color::black();
    setBlurStrength(0.5f);
    secondShader = new ShaderProgram(vertexSecondShader.c_str(), fragmentSecondShader.c_str());
    shader = new ShaderProgram(blurVertexShader, renderFragment);
    
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char *data = stbi_load(filename, &width, &height, &nrChannels, 0);
    
    size = new GLSize(width, height);
    glGenFramebuffers(1, &frameBufferId);
    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glBindFramebuffer(GL_FRAMEBUFFER, frameBufferId);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureId, 0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    
    glGenFramebuffers(1, &secondFrameBufferId);
    glGenTextures(1, &secondTextureId);
    glBindTexture(GL_TEXTURE_2D, secondTextureId);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glBindFramebuffer(GL_FRAMEBUFFER, secondFrameBufferId);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, secondTextureId, 0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    
    glGenFramebuffers(1, &thirdFrameBufferId);
    glGenTextures(1, &thirdTextureId);
    glBindTexture(GL_TEXTURE_2D, thirdTextureId);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glBindFramebuffer(GL_FRAMEBUFFER, thirdFrameBufferId);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, thirdTextureId, 0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    
    _dstSecondTex = rendering->generateTexture(GL_LINEAR, GL_LINEAR, GL_REPEAT, GL_REPEAT);
}

Blur::~Blur() {
    if (imageFramebuffer) {
        delete imageFramebuffer;
        imageFramebuffer = nullptr;
    }
    if (rendering) {
        delete rendering;
        rendering = nullptr;
    }
    if (shader) {
        delete shader;
        shader = nullptr;
    }
    if (size) {
        delete size;
        size = nullptr;
    }
}

void Blur::setBlurStrength(float strength) {
    float newValue = 60.0 * strength;
    uint32_t radius = round(newValue);
    
    radius >>= _downSamplingPow2;
    if (radius != _blurRadiusInPixels) {
        _blurRadiusInPixels = radius;
        uint32_t calculatedSampleRadius = 0;
        
        if (_blurRadiusInPixels >= 1) {
            float minimumWeightToFindEdgeOfSamplingArea = 1.0f / 256.0;
            calculatedSampleRadius = floor(sqrt(-2.0 * pow(_blurRadiusInPixels, 2.0) * log(minimumWeightToFindEdgeOfSamplingArea * sqrt(2.0 * M_PI * pow(_blurRadiusInPixels, 2.0))) ));
            calculatedSampleRadius += calculatedSampleRadius % 2;
            
            vertexShaderForOptimizedBlurOfRadius(calculatedSampleRadius, _blurRadiusInPixels);
            fragmentShaderForOptimizedBlurOfRadius(calculatedSampleRadius, _blurRadiusInPixels);
        }
    }
}

void Blur::vertexShaderForOptimizedBlurOfRadius(uint32_t blurRadius, float sigma) {
    if (blurRadius < 1) {
        return;
    }
    float* standardGaussianWeights = new float[blurRadius + 1];
    float sumOfWeights = 0.0;
    for (uint32_t currentGaussianWeightIndex = 0; currentGaussianWeightIndex < blurRadius + 1; currentGaussianWeightIndex++) {
        standardGaussianWeights[currentGaussianWeightIndex] = (1.0 / sqrt(2.0 * M_PI * pow(sigma, 2.0))) * exp(-pow(currentGaussianWeightIndex, 2.0) / (2.0 * pow(sigma, 2.0)));
        if (currentGaussianWeightIndex == 0) {
            sumOfWeights += standardGaussianWeights[currentGaussianWeightIndex];
        } else {
            sumOfWeights += 2.0 * standardGaussianWeights[currentGaussianWeightIndex];
        }
    }
    
    for (uint32_t currentGaussianWeightIndex = 0; currentGaussianWeightIndex < blurRadius + 1; currentGaussianWeightIndex++) {
        standardGaussianWeights[currentGaussianWeightIndex] = standardGaussianWeights[currentGaussianWeightIndex] / sumOfWeights;
    }
    
    uint32_t numberOfOptimizedOffsets = MIN(blurRadius / 2 + (blurRadius % 2), 7);
    float* optimizedGaussianOffsets = new float[numberOfOptimizedOffsets];
    for (uint32_t currentOptimizedOffset = 0; currentOptimizedOffset < numberOfOptimizedOffsets; currentOptimizedOffset++) {
        float firstWeight = standardGaussianWeights[currentOptimizedOffset * 2 + 1];
        float secondWeight = standardGaussianWeights[currentOptimizedOffset * 2 + 1];
        float optimizedWeight = firstWeight + secondWeight;
        
        optimizedGaussianOffsets[currentOptimizedOffset] = (firstWeight * (currentOptimizedOffset * 2 + 1) + secondWeight * (currentOptimizedOffset * 2 + 2)) / optimizedWeight;
    }
    
    char tmpStr[MAX_STRING_SIZE];
    sprintf(tmpStr, "\
            attribute vec4 position;\n\
            attribute vec4 inputTextureCoordinate;\n\
            uniform float textureWidthOffset;\n\
            uniform float textureHeightOffset;\n\
            varying vec2 blurCoordinates[%lu];\n\
            void main() {\n\
            gl_Position = position;\n\
            vec2 singleOffset = vec2(textureWidthOffset, textureHeightOffset);\n",
            (unsigned long)(1 + (numberOfOptimizedOffsets * 2)));
    
    std::string shaderString(tmpStr);
    sprintf(tmpStr, "blurCoordinates[0] = inputTextureCoordinate.xy;\n");
    shaderString.append(tmpStr);
    
    for (uint32_t currentOptimizedOffset = 0; currentOptimizedOffset < numberOfOptimizedOffsets; currentOptimizedOffset++) {
        sprintf(tmpStr, "blurCoordinates[%lu] = inputTextureCoordinate.xy + singleOffset * %f;\n\
                blurCoordinates[%lu] = inputTextureCoordinate.xy - singleOffset * %f;\n",
                (unsigned long) ((currentOptimizedOffset * 2) + 1), optimizedGaussianOffsets[currentOptimizedOffset], (unsigned long)((currentOptimizedOffset * 2) + 2), optimizedGaussianOffsets[currentOptimizedOffset]);
        shaderString.append(tmpStr);
    }
    
    shaderString.append("}\n");
    
    delete[] optimizedGaussianOffsets;
    delete[] standardGaussianWeights;
    vertexSecondShader = shaderString;
    
//    printf("vertex = %s\n", vertexSecondShader.c_str());
}

void Blur::fragmentShaderForOptimizedBlurOfRadius(uint32_t blurRadius, float sigma) {
    if (blurRadius < 1) {
        return;
    }
    float *standardGaussianWeights = new float[blurRadius + 1];
    float sumOfWeights = 0.0;
    for (uint32_t currentGaussianWeightIndex = 0; currentGaussianWeightIndex < blurRadius + 1; currentGaussianWeightIndex++) {
        standardGaussianWeights[currentGaussianWeightIndex] = (1.0 / sqrt(2.0 * M_PI * pow(sigma, 2.0))) * exp(-pow(currentGaussianWeightIndex, 2.0) / (2.0 * pow(sigma, 2.0)));
        if (currentGaussianWeightIndex == 0) {
            sumOfWeights += standardGaussianWeights[currentGaussianWeightIndex];
        } else {
            sumOfWeights += 2.0 * standardGaussianWeights[currentGaussianWeightIndex];
        }
    }
    
    for (uint32_t currentGaussianWeightIndex = 0; currentGaussianWeightIndex < blurRadius + 1; currentGaussianWeightIndex++) {
        standardGaussianWeights[currentGaussianWeightIndex] = standardGaussianWeights[currentGaussianWeightIndex] / sumOfWeights;
    }
    
    uint32_t numberOfOptimizedOffsets = MIN(blurRadius / 2 + (blurRadius % 2), 7);
    uint32_t trueNumberOfOptimizedOffsets = blurRadius / 2 + (blurRadius % 2);
    char tmpStr[MAX_STRING_SIZE];
    sprintf(tmpStr, "\
            uniform sampler2D inputImageTexture;\n\
            uniform float textureWidthOffset;\n\
            uniform float textureHeightOffset;\n\
            varying vec2 blurCoordinates[%lu];\n\
            void main() {\n\
            vec4 sum = vec4(0.0);\n",
            (unsigned long)(1 + (numberOfOptimizedOffsets * 2)));
    std::string shaderString(tmpStr);
    sprintf(tmpStr, "sum += texture2D(inputImageTexture, blurCoordinates[0]) * %f;\n", standardGaussianWeights[0]);
    shaderString.append(tmpStr);
    for (uint32_t currentBlurCoordinateIndex = 0; currentBlurCoordinateIndex < numberOfOptimizedOffsets; currentBlurCoordinateIndex++) {
        float firstWeight = standardGaussianWeights[currentBlurCoordinateIndex * 2 + 1];
        float secondWeight = standardGaussianWeights[currentBlurCoordinateIndex * 2 + 2];
        float optimizedWeight = firstWeight + secondWeight;
        sprintf(tmpStr, "sum += texture2D(inputImageTexture, blurCoordinates[%lu]) * %f;\n",(unsigned long)((currentBlurCoordinateIndex * 2) + 1), optimizedWeight);
        shaderString.append(tmpStr);
        sprintf(tmpStr, "sum += texture2D(inputImageTexture, blurCoordinates[%lu]) * %f;\n", (unsigned long) ((currentBlurCoordinateIndex * 2) + 2), optimizedWeight);
        shaderString.append(tmpStr);
    }
    if (trueNumberOfOptimizedOffsets > numberOfOptimizedOffsets) {
        sprintf(tmpStr, "vec2 singleStepOffset = vec2(textureWidthOffset, textureHeightOffset);\n");
        shaderString.append(tmpStr);
        
        for (uint32_t currentOverlowTextureRead = numberOfOptimizedOffsets; currentOverlowTextureRead < trueNumberOfOptimizedOffsets; currentOverlowTextureRead++) {
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
    shaderString.append("\
                        gl_FragColor = sum;\n\
                        }\n");
    delete[] standardGaussianWeights;
    fragmentSecondShader = shaderString;
//    printf("fragment = %s\n", fragmentSecondShader.c_str());
}

int Blur::processImage(bool synchronously) {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    secondShader->use();
    auto inputImageTextureIndex = secondShader->uniformIndex("inputImageTexture");
    auto positionIndex = secondShader->attributeIndex("position");
    auto inputTextureCoordinateIndex = secondShader->attributeIndex("inputTextureCoordinate");
    auto textureWidthOffsetIndex = secondShader->uniformIndex("textureWidthOffset");
    auto textureHeightOffsetIndex = secondShader->uniformIndex("textureHeightOffset");
    
    if (_blurRadiusInPixels >= 1) {
        glBindFramebuffer(GL_FRAMEBUFFER, secondFrameBufferId);
        glViewport(0, 0, viewWidth, viewHeight);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textureId);
        glUniform1i(inputImageTextureIndex, 0);
        glVertexAttribPointer(inputTextureCoordinateIndex, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GL_FLOAT), secondTextureCoord);
        glEnableVertexAttribArray(inputTextureCoordinateIndex);
        glVertexAttribPointer(positionIndex, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GL_FLOAT), blurVertex);
        glEnableVertexAttribArray(positionIndex);
        
        auto textureWidthOffset = 1.0f / (dstWidth >> _downSamplingPow2);
        auto textureHeightOffset = 0.0f;
        glUniform1f(textureWidthOffsetIndex, textureWidthOffset);
        glUniform1f(textureHeightOffsetIndex, textureHeightOffset);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        glFlush();
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        
        
        
        glBindFramebuffer(GL_FRAMEBUFFER, thirdFrameBufferId);
        glViewport(0,0, viewWidth,viewHeight);
        glActiveTexture(GL_TEXTURE0+0);
        
        glBindTexture(GL_TEXTURE_2D, secondTextureId);
        glUniform1i(inputImageTextureIndex, 0);
        
        glVertexAttribPointer(positionIndex, 2, GL_FLOAT, GL_FALSE, 2*sizeof(GLfloat), blurVertex);
        glEnableVertexAttribArray(positionIndex);
        
        glVertexAttribPointer(inputTextureCoordinateIndex, 2, GL_FLOAT, GL_FALSE, 2*sizeof(GLfloat), blurTextureCoordinates);
        glEnableVertexAttribArray(inputTextureCoordinateIndex);
        
        textureWidthOffset = 0.0f;
        textureHeightOffset = 1.0f/ viewHeight;
        
        glUniform1f(textureWidthOffsetIndex,textureWidthOffset);
        glUniform1f(textureHeightOffsetIndex,textureHeightOffset);
        
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        glFlush();
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        
        shader->use();
        glViewport(0, 0, dstWidth, dstHeight);
        auto inputImageTextureLocation = shader->uniformIndex("inputImageTexture");
        auto positionLocation = secondShader->attributeIndex("position");
        auto inputTextureCoordinateLocation = secondShader->attributeIndex("inputTextureCoordinate");
        if (scaleFactorX < 0.99999 || scaleFactorY < 0.99999) {
            glActiveTexture(GL_TEXTURE0+0);

            glBindTexture(GL_TEXTURE_2D, textureId);
            glUniform1i(inputImageTextureLocation, 0);

            glVertexAttribPointer(inputTextureCoordinateLocation, 2, GL_FLOAT, GL_FALSE, 2*sizeof(GLfloat), blurTextureCoordinates);
            glEnableVertexAttribArray(inputTextureCoordinateLocation);

            glVertexAttribPointer(positionLocation, 2, GL_FLOAT, GL_FALSE, 2*sizeof(GLfloat), blurVertexShader);
            glEnableVertexAttribArray(positionLocation);

            glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        }

        glActiveTexture(GL_TEXTURE0+0);

        glBindTexture(GL_TEXTURE_2D, thirdTextureId);
        glUniform1i(inputImageTextureLocation, 0);
        glVertexAttribPointer(inputTextureCoordinateLocation, 2, GL_FLOAT, GL_FALSE, 2*sizeof(GLfloat), blurTextureCoordinates);
        glEnableVertexAttribArray(inputTextureCoordinateLocation);

        glVertexAttribPointer(positionLocation, 2, GL_FLOAT, GL_FALSE, 2*sizeof(GLfloat), blurVertex);
        glEnableVertexAttribArray(positionLocation);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        glFlush();
    } else {
        
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    
    return thirdTextureId;
}
