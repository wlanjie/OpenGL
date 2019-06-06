//
//  opengl.h
//  OpenGL
//
//  Created by wlanjie on 2018/11/14.
//  Copyright © 2018年 com.wlanjie.opengl. All rights reserved.
//

#ifndef opengl_h
#define opengl_h

#include <OpenGL/OpenGL.h>
#include <OpenGL/gl.h>
#include <string>

class OpenGL {
public:
    OpenGL(const char* vertex, const char* fragment);
    OpenGL(int width, int height, const char* vertex, const char* fragment);
    ~OpenGL();
    void setOutput(int width, int height);
    void SetInt(const char* name, int value);
    void SetFloat(const char* name, float value);
    void SetFloatVec2(const char* name, int size, const GLfloat* value);
    void SetFloatVec3(const char* name, int size, const GLfloat* value);
    void SetFloatVec4(const char* name, int size, const GLfloat* value);
    void SetUnifromMatrix3f(const char* name, int size, const GLfloat* matrix);
    void SetUnifromMatrix4f(const char* name, int size, const GLfloat* matrix);
protected:
    void ProcessImage(GLuint textureId);
    void ProcessImage(GLuint textureId, const GLfloat* vertexCoordinate, const GLfloat* textureCoordinate);
    virtual void RunOnDrawTasks();
    virtual void OnDrawArrays();
    
private:
    void CreateProgram(const char* vertex, const char* fragment);
    void CompileShader(const char* shaderString, GLuint shader);
    void Link();
private:
    GLuint program_;
    int width_;
    int height_;
};

#endif /* opengl_h */
