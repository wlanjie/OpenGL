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

GLuint ShaderProgram::attributeIndex(const char* attribute) {
    auto attributeAddress = attributeAddresses[attribute];
//    if (attributeAddress) {
//        return attributeAddress;
//    } else {
        attributeAddress = (GLuint) glGetAttribLocation(program, attribute);
        if (attributeAddress < 0) {
            return 0;
        }
        glEnableVertexAttribArray(attributeAddress);
        attributeAddresses[attribute] = attributeAddress;
        return attributeAddress;
//    }
}

GLint ShaderProgram::uniformIndex(const char* uniform) {
    auto uniformAddress = uniformAddresses[uniform];
    if (uniformAddress) {
        return uniformAddress;
    } else {
        uniformAddress = static_cast<GLuint>(glGetAttribLocation(program, uniform));
        if (uniformAddress < 0) {
            return 0;
        }
        uniformAddresses[uniform] = uniformAddress;
        return uniformAddress;
    }
}

void ShaderProgram::use() {
    glUseProgram(program);
}

void ShaderProgram::setValue(GLfloat value, const char* forUniform) {
    auto uniformAddress = uniformIndex(forUniform);
    if (!uniformAddress) {
        return;
    }
    if (currentUniformFloatValues[forUniform] != value) {
        glUniform1f(uniformAddress, value);
        currentUniformFloatValues[forUniform] = value;
    }
}

void ShaderProgram::setValue(GLint value, const char* forUniform) {
    auto uniformAddress = uniformIndex(forUniform);
    if (!uniformAddress) {
        return;
    }
    if (currentUniformIntValues[forUniform] != value) {
        glUniform1i(uniformAddress, value);
        currentUniformIntValues[forUniform] = value;
    }
}

void ShaderProgram::setValue(GLfloat* value, int size, const char* forUniform) {
    if (!value) {
        return;
    }
    auto uniformAddress = uniformIndex(forUniform);
    if (!uniformAddress) {
        return;
    }
    GLfloat* previousValue = currentUniformFloatArrayValues[forUniform];
    if (previousValue == value) {
        return;
    }
    if (size == 2) {
        glUniform2fv(uniformAddress, 1, value);
    } else if (size == 3) {
        glUniform3fv(uniformAddress, 1, value);
    } else if (size == 4) {
        glUniform4fv(uniformAddress, 1, value);
    }
    currentUniformFloatArrayValues[forUniform] = value;
}

void ShaderProgram::setMatrix(GLfloat* value, int size, const char* forUniform) {
    if (!value) {
        return;
    }
    if (size == 0) {
        return;
    }
    auto uniformAddress = uniformIndex(forUniform);
    if (!uniformAddress) {
        return;
    }
    GLfloat* previousValue = currentUniformFloatArrayValues[forUniform];
    if (previousValue == value) {
        return;
    }
    if (size == 9) {
        glUniformMatrix3fv(uniformAddress, 1, GL_FALSE, value);
    } else if (size == 16) {
        glUniformMatrix4fv(uniformAddress, 1, GL_FALSE, value);
    }
    currentUniformFloatArrayValues[forUniform] = value;
}
