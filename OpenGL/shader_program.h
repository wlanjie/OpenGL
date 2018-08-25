//
// Created by wlanjie on 2018/3/18.
//

#ifndef OPENGL_SHADER_PROGRAM_H
#define OPENGL_SHADER_PROGRAM_H

#include <map>
#include <string>
#include "color.h"
#include <OpenGL/OpenGL.h>

class ShaderProgram {
public:
    enum ShaderType {
        VERTEX,
        FRAGMENT
    };

    ShaderProgram(const char* vertexShader, const char* fragmentShader);
    ~ShaderProgram();

    GLuint attributeIndex(const char* attribute);
    GLint uniformIndex(const char* uniform);
    void use();
    void setValue(GLfloat value, const char* forUniform);
    void setValue(GLint value, const char* forUniform);
    void setValue(Color* value, const char* forUniform);
    void setValue(GLfloat* value, int size, const char* forUniform);
    void setMatrix(GLfloat* value, int size, const char* forUniform);
    void setColorUniformUseFourComponents(bool colorUniformUseFourComponents) {
        this->colorUniformUseFourComponents = colorUniformUseFourComponents;
    }

public:
    GLuint program;
    GLuint vertexShader;
    GLuint fragmentShader;
    bool colorUniformUseFourComponents;

    std::map<const char*, GLuint> attributeAddresses;
    std::map<const char*, GLint> uniformAddresses;
    std::map<const char*, GLint> currentUniformIntValues;
    std::map<const char*, GLfloat> currentUniformFloatValues;
    std::map<const char*, GLfloat*> currentUniformFloatArrayValues;

    GLuint compileShader(const char* shaderString, ShaderType type);
    void link();
};

#endif //OPENGL_SHADER_PROGRAM_H
