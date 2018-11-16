//
// Created by wlanjie on 2018/3/18.
//
#include <OpenGL/OpenGL.h>
#include <OpenGL/gl.h>
#include <cstdlib>
#include "shader_program.h"

void checkGlError(const char* op) {
    for (GLint error = glGetError(); error; error = glGetError()) {
        printf("after %s() glError (0x%x)\n", op, error);
    }
}

ShaderProgram::ShaderProgram(const char *vertexShader, const char *fragmentShader) :
        program(0),
        vertexShader(0),
        fragmentShader(0),
        colorUniformUseFourComponents(false) {
    program = glCreateProgram();
    this->vertexShader = compileShader(vertexShader, VERTEX);
    this->fragmentShader = compileShader(fragmentShader, FRAGMENT);
    glAttachShader(program, this->vertexShader);
    checkGlError("glAttachVertexShader");
    glAttachShader(program, this->fragmentShader);
    checkGlError("glAttachVertexShader");
    link();
}

ShaderProgram::~ShaderProgram() {
    if (program) {
        glDeleteProgram(program);
    }
}

GLuint ShaderProgram::compileShader(const char *shaderString, ShaderProgram::ShaderType type) {
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
    return shader;
}

void ShaderProgram::link() {
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
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

GLuint ShaderProgram::getProgram() {
    return program;
}

void ShaderProgram::setInt(const char* name, int value) {
    auto location = glGetUniformLocation(program, name);
    glUniform1i(location, value);
}

void ShaderProgram::setFloat(const char* name, float value) {
    auto location = glGetUniformLocation(program, name);
    glUniform1f(location, value);
}

void ShaderProgram::setFloatVec2(const char* name, int size, const GLfloat* value) {
    auto location = glGetUniformLocation(program, name);
    glUniform2fv(location, size, value);
}

void ShaderProgram::setFloatVec3(const char* name, int size, const GLfloat* value) {
    auto location = glGetUniformLocation(program, name);
    glUniform3fv(location, size, value);
}

void ShaderProgram::setFloatVec4(const char* name, int size, const GLfloat* value) {
    auto location = glGetUniformLocation(program, name);
    glUniform4fv(location, size, value);
}

void ShaderProgram::setUnifromMatrix3f(const char* name, int size, const GLfloat* matrix) {
    auto location = glGetUniformLocation(program, name);
    glUniformMatrix3fv(location, size, GL_FALSE, matrix);
}

void ShaderProgram::setUnifromMatrix4f(const char* name, int size, const GLfloat* matrix) {
    auto location = glGetUniformLocation(program, name);
    glUniformMatrix4fv(location, size, GL_FALSE, matrix);
}
