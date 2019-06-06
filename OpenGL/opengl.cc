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
    this->width_ = 0;
    this->height_ = 0;
    CreateProgram(vertex, fragment);
}

OpenGL::OpenGL(int width, int height, const char* vertex, const char* fragment) {
    this->width_ = width;
    this->height_ = height;
    CreateProgram(vertex, fragment);
}

void OpenGL::setOutput(int width, int height) {
    this->width_ = width;
    this->height_ = height;
}

void OpenGL::CreateProgram(const char* vertex, const char* fragment) {
#ifdef __APPLE__
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
    
    program_ = glCreateProgram();
    auto vertexShader = glCreateShader(GL_VERTEX_SHADER);
    auto fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    CompileShader(vertexCode.c_str(), vertexShader);
    CompileShader(fragmentCode.c_str(), fragmentShader);
    glAttachShader(program_, vertexShader);
    checkGlError("glAttachVertexShader");
    glAttachShader(program_, fragmentShader);
    checkGlError("glAttachVertexShader");
    Link();
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
#else
    program = glCreateProgram();
    auto vertexShader = glCreateShader(GL_VERTEX_SHADER);
    auto fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    compileShader(vertex, vertexShader);
    compileShader(fragment, fragmentShader);
    glAttachShader(program, vertexShader);
    checkGlError("glAttachVertexShader");
    glAttachShader(program, fragmentShader);
    checkGlError("glAttachVertexShader");
    link();
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
#endif
}

OpenGL::~OpenGL() {
    if (program_ != 0) {
        glDeleteProgram(program_);
        program_ = 0;
    }
}

void OpenGL::RunOnDrawTasks() {
}

void OpenGL::OnDrawArrays() {
    
}

void OpenGL::ProcessImage(GLuint textureId) {
    ProcessImage(textureId, defaultVertexCoordinates, defaultTextureCoordinate);
}

void OpenGL::ProcessImage(GLuint textureId, const GLfloat* vertexCoordinate, const GLfloat* textureCoordinate) {
    glUseProgram(program_);
    if (width_ > 0 && height_ > 0) {
        glViewport(0, 0, width_, height_);
    }
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    RunOnDrawTasks();
    auto positionAttribute = glGetAttribLocation(program_, "position");
    glEnableVertexAttribArray(positionAttribute);
    glVertexAttribPointer(positionAttribute, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), vertexCoordinate);
    auto textureCoordinateAttribute = glGetAttribLocation(program_, "inputTextureCoordinate");
    glEnableVertexAttribArray(textureCoordinateAttribute);
    glVertexAttribPointer(textureCoordinateAttribute, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), textureCoordinate);
    
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureId);
    SetInt("inputImageTexture", 0);
    OnDrawArrays();
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glDisableVertexAttribArray(positionAttribute);
    glDisableVertexAttribArray(textureCoordinateAttribute);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void OpenGL::CompileShader(const char *shaderString, GLuint shader) {
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

void OpenGL::Link() {
    glLinkProgram(program_);
    GLint linkStatus = GL_FALSE;
    glGetProgramiv(program_, GL_LINK_STATUS, &linkStatus);
    if (linkStatus == GL_FALSE) {
        GLint infoLen;
        glGetProgramiv(program_, GL_INFO_LOG_LENGTH, &infoLen);
        if (infoLen) {
            auto *buf = (char *) malloc((size_t) infoLen);
            if (buf) {
                glGetProgramInfoLog(program_, infoLen, nullptr, buf);
                printf("%s", buf);
                free(buf);
            }
            glDeleteProgram(program_);
            program_ = 0;
        }
    }
}

void OpenGL::SetInt(const char* name, int value) {
    auto location = glGetUniformLocation(program_, name);
    glUniform1i(location, value);
}

void OpenGL::SetFloat(const char* name, float value) {
    auto location = glGetUniformLocation(program_, name);
    glUniform1f(location, value);
}

void OpenGL::SetFloatVec2(const char* name, int size, const GLfloat* value) {
    auto location = glGetUniformLocation(program_, name);
    glUniform2fv(location, size, value);
}

void OpenGL::SetFloatVec3(const char* name, int size, const GLfloat* value) {
    auto location = glGetUniformLocation(program_, name);
    glUniform3fv(location, size, value);
}

void OpenGL::SetFloatVec4(const char* name, int size, const GLfloat* value) {
    auto location = glGetUniformLocation(program_, name);
    glUniform4fv(location, size, value);
}

void OpenGL::SetUnifromMatrix3f(const char* name, int size, const GLfloat* matrix) {
    auto location = glGetUniformLocation(program_, name);
    glUniformMatrix3fv(location, size, GL_FALSE, matrix);
}

void OpenGL::SetUnifromMatrix4f(const char* name, int size, const GLfloat* matrix) {
    auto location = glGetUniformLocation(program_, name);
    glUniformMatrix4fv(location, size, GL_FALSE, matrix);
}
