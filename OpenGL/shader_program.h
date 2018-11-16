//
// Created by wlanjie on 2018/3/18.
//

#ifndef OPENGL_SHADER_PROGRAM_H
#define OPENGL_SHADER_PROGRAM_H

#include <OpenGL/OpenGL.h>
#include <OpenGL/OpenGL.h>
#include <string>
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
    GLuint getProgram();

    void setInt(const char* name, int value);
    void setFloat(const char* name, float value);
    void setFloatVec2(const char* name, int size, const GLfloat* value);
    void setFloatVec3(const char* name, int size, const GLfloat* value);
    void setFloatVec4(const char* name, int size, const GLfloat* value);
    void setUnifromMatrix3f(const char* name, int size, const GLfloat* matrix);
    void setUnifromMatrix4f(const char* name, int size, const GLfloat* matrix);
private:
    GLuint program;
    GLuint vertexShader;
    GLuint fragmentShader;
    bool colorUniformUseFourComponents;

    GLuint compileShader(const char* shaderString, ShaderType type);
    void link();
};

#endif //OPENGL_SHADER_PROGRAM_H
