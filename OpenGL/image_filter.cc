//
//  image_filter.cpp
//  OpenGL
//
//  Created by wlanjie on 2018/11/10.
//  Copyright © 2018 com.wlanjie.opengl. All rights reserved.
//

#include "image_filter.h"
#include <cstdlib>

namespace OpenGLImage {
    
    ImageFilter::ImageFilter() :
        program(0),
        vertexShader(0),
        fragmentShader(0),
        initialized(false),
        outputWidth(0),
        outputHeight(0) {
            
        this->vertexShader = const_cast<char*>(defaultVertexShader);
        this->fragmentShader = const_cast<char*>(defaultFragmentShader);
    }
 
    ImageFilter::ImageFilter(const char* vertexShader, const char* fragmentShader) :
            program(0),
            vertexShader(0),
            fragmentShader(0),
            initialized(false),
            outputWidth(0),
            outputHeight(0) {
                
            this->vertexShader = const_cast<char*>(vertexShader);
            this->fragmentShader = const_cast<char*>(fragmentShader);
    }
    
    ImageFilter::~ImageFilter() {

    }
    
    void ImageFilter::onInit() {
        program = glCreateProgram();
        GLuint vertexShaderIndex = compileShader(vertexShader, VERTEX);
        GLuint fragmentShaderIndex = compileShader(fragmentShader, FRAGMENT);
        link();
        
        this->position = glGetAttribLocation(program, "position");
        this->inputTextureCoordinate = glGetAttribLocation(program, "inputTextureCoordinate");
        this->inputImageTexture = glGetUniformLocation(program, "inputImageTexture");
        glDeleteShader(vertexShaderIndex);
        glDeleteShader(fragmentShaderIndex);
        initialized = true;
    }
    
    void ImageFilter::onInitialized() {
        
    }
    
    void ImageFilter::destroy() {
        if (program) {
            glDeleteProgram(program);
            program = 0;
        }
    }
    
    void ImageFilter::onDestroy() {
        
    }
    
    void ImageFilter::onOutputSizeChanged(const int width, const int height) {
        outputWidth = width;
        outputHeight = height;
    }
    
    void ImageFilter::onDraw(const int textureId, const float *cubeBuffer, const float *textureBuffer) {
        if (!initialized) {
            return;
        }
        glUseProgram(program);
        runPedingOnDrawTasks();
        glVertexAttribPointer(program, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), cubeBuffer);
        glEnableVertexAttribArray(position);
        glVertexAttribPointer(program, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), textureBuffer);
        glEnableVertexAttribArray(inputTextureCoordinate);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textureId);
        glUniform1i(inputImageTexture, 0);
        onDrawArrayPre();
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        glDisableVertexAttribArray(position);
        glDisableVertexAttribArray(inputTextureCoordinate);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
    
    bool ImageFilter::isInitialized() {
        return initialized;
    }
    
    int ImageFilter::getOutputWidth() {
        return outputWidth;
    }
    
    int ImageFilter::getOutputHeight() {
        return outputHeight;
    }
    
    int ImageFilter::getProgram() {
        return program;
    }
    
    int ImageFilter::getAttributePosition() {
        return position;
    }
    
    int ImageFilter::getAttributeTextureCoordinate() {
        return inputTextureCoordinate;
    }
    
    int ImageFilter::getUniformTexture() {
        return inputImageTexture;
    }
    
    void ImageFilter::onDrawArrayPre() {
        
    }
    
    void ImageFilter::runPedingOnDrawTasks() {
        
    }
    
    void ImageFilter::setInteger(const int location, const int value) {
        glUniform1i(location, value);
    }
    
    void ImageFilter::setFloat(const int location, const float value) {
        glUniform1f(location, value);
    }
    
    void ImageFilter::setFloatVec2(const int location, const float *value) {
        glUniform2fv(location, 1, value);
    }
    
    void ImageFilter::setFloatVec3(const int location, const float *value) {
        glUniform3fv(location, 1, value);
    }
    
    void ImageFilter::setFloatVec4(const int location, const float *value) {
        glUniform4fv(location, 1, value);
    }
    
    void ImageFilter::setFloatArray(const int location, const int size, const float *value) {
        glUniform1fv(location, size, value);
    }
    
    void ImageFilter::setPoint(const int location, const float x, const float y) {
        float vec2[2];
        vec2[0] = x;
        vec2[1] = y;
        glUniform2fv(location, 1, vec2);
    }
    
    void ImageFilter::setUniformMatrix3f(const int location, const float *matrix) {
        glUniformMatrix3fv(location, 1, GL_FALSE, matrix);
    }
    
    void ImageFilter::setUniformMatrix4f(const int location, const float *matrix) {
        glUniformMatrix4fv(location, 1, GL_FALSE, matrix);
    }
    
    GLuint ImageFilter::compileShader(const char* shaderString, ShaderType type) {
        GLuint shader = 0;
        switch (type) {
            case VERTEX:
                shader = glCreateShader(GL_VERTEX_SHADER);
                break;
                
            case FRAGMENT:
                shader = glCreateShader(GL_FRAGMENT_SHADER);
                break;
        }
        glShaderSource(shader, 1, &shaderString, nullptr);
        glCompileShader(shader);
        GLint compiled = GL_FALSE;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
        if (!compiled) {
            GLint infoLen;
            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);
            if (infoLen) {
                auto *buf = (char *) malloc((size_t) infoLen);
                if (buf) {
                    glGetShaderInfoLog(shader, infoLen, nullptr, buf);
                    printf("Could not compile %d:\n%s\n", shader, buf);
                    free(buf);
                }
                glDeleteShader(shader);
            }
        }
        glAttachShader(program, shader);
        return shader;
    }
    
    void ImageFilter::link() {
        glLinkProgram(program);
        GLint linkStatus = GL_FALSE;
        glGetProgramiv(program, GL_LINK_STATUS, &linkStatus);
        if (linkStatus == GL_FALSE) {
            GLint infoLen;
            glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLen);
            if (infoLen) {
                auto *buf = (char *) malloc((size_t) infoLen);
                if (buf) {
                    glGetProgramInfoLog(program, infoLen, nullptr, buf);
                    printf("%s", buf);
                    free(buf);
                }
                glDeleteProgram(program);
                program = 0;
            }
        }
    }
    
    void ImageFilter::checkGlError(const char* op) {
        for (GLint error = glGetError(); error; error = glGetError()) {
            printf("after %s() glError (0x%x)\n", op, error);
        }
    }
}
