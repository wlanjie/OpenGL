//
//  opengl.cc
//  OpenGL
//
//  Created by wlanjie on 2018/11/14.
//  Copyright © 2018年 com.wlanjie.opengl. All rights reserved.
//

#include "opengl.h"
#include "shader.h"
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <iostream>

void checkGlError(const char* op) {
    for (GLint error = glGetError(); error; error = glGetError()) {
        printf("after %s() glError (0x%x)\n", op, error);
    }
}

OpenGL::OpenGL(const char* vertex, const char* fragment) {
    this->width = 0;
    this->height = 0;
    createProgram(vertex, fragment);
}

OpenGL::OpenGL(int width, int height, const char* vertex, const char* fragment) {
    this->width = width;
    this->height = height;
    createProgram(vertex, fragment);
}

void OpenGL::setOutput(int width, int height) {
    this->width = width;
    this->height = height;
}

void OpenGL::createProgram(const char* vertex, const char* fragment) {
    std::string vertexCode;
    std::string fragmentCode;
    std::ifstream vertexShaderFile;
    std::ifstream fragmentShaderFile;
    vertexShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fragmentShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try {
        vertexShaderFile.open(vertex);
        fragmentShaderFile.open(fragment);
        std::stringstream vertexStream;
        std::stringstream fragmentStream;
        vertexStream << vertexShaderFile.rdbuf();
        fragmentStream << fragmentShaderFile.rdbuf();
        vertexShaderFile.close();
        fragmentShaderFile.close();
        vertexCode = vertexStream.str();
        fragmentCode = fragmentStream.str();
    } catch (std::ifstream::failure e) {
        
    }
    
    program = glCreateProgram();
    auto vertexShader = glCreateShader(GL_VERTEX_SHADER);
    auto fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    compileShader(vertexCode.c_str(), vertexShader);
    compileShader(fragmentCode.c_str(), fragmentShader);
    glAttachShader(program, vertexShader);
    checkGlError("glAttachVertexShader");
    glAttachShader(program, fragmentShader);
    checkGlError("glAttachVertexShader");
    link();
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

OpenGL::~OpenGL() {
    if (program != 0) {
        glDeleteProgram(program);
        program = 0;
    }
}

void OpenGL::runOnDrawTasks() {
}

void OpenGL::onDrawArrays() {
    
}

void OpenGL::processImage(GLuint textureId) {
    processImage(textureId, defaultVertexCoordinates, defaultTextureCoordinate);
}

void OpenGL::processImage(GLuint textureId, const GLfloat* vertexCoordinate, const GLfloat* textureCoordinate) {
    glUseProgram(program);
    if (width > 0 && height > 0) {
        glViewport(0, 0, width, height);
    }
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    runOnDrawTasks();
    auto positionAttribute = glGetAttribLocation(program, "position");
    glEnableVertexAttribArray(positionAttribute);
    glVertexAttribPointer(positionAttribute, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), vertexCoordinate);
    auto textureCoordinateAttribute = glGetAttribLocation(program, "inputTextureCoordinate");
    glEnableVertexAttribArray(textureCoordinateAttribute);
    glVertexAttribPointer(textureCoordinateAttribute, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), textureCoordinate);
    
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureId);
    setInt("inputImageTexture", 0);
    onDrawArrays();
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glDisableVertexAttribArray(positionAttribute);
    glDisableVertexAttribArray(textureCoordinateAttribute);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void OpenGL::compileShader(const char *shaderString, GLuint shader) {
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
}

void OpenGL::link() {
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

void OpenGL::setInt(const char* name, int value) {
    auto location = glGetUniformLocation(program, name);
    glUniform1i(location, value);
}

void OpenGL::setFloat(const char* name, float value) {
    auto location = glGetUniformLocation(program, name);
    glUniform1f(location, value);
}

void OpenGL::setFloatVec2(const char* name, int size, const GLfloat* value) {
    auto location = glGetUniformLocation(program, name);
    glUniform2fv(location, size, value);
}

void OpenGL::setFloatVec3(const char* name, int size, const GLfloat* value) {
    auto location = glGetUniformLocation(program, name);
    glUniform3fv(location, size, value);
}

void OpenGL::setFloatVec4(const char* name, int size, const GLfloat* value) {
    auto location = glGetUniformLocation(program, name);
    glUniform4fv(location, size, value);
}

void OpenGL::setUnifromMatrix3f(const char* name, int size, const GLfloat* matrix) {
    auto location = glGetUniformLocation(program, name);
    glUniformMatrix3fv(location, size, GL_FALSE, matrix);
}

void OpenGL::setUnifromMatrix4f(const char* name, int size, const GLfloat* matrix) {
    auto location = glGetUniformLocation(program, name);
    glUniformMatrix4fv(location, size, GL_FALSE, matrix);
}
