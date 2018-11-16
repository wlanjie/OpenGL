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
    void setInt(const char* name, int value);
    void setFloat(const char* name, float value);
    void setFloatVec2(const char* name, int size, const GLfloat* value);
    void setFloatVec3(const char* name, int size, const GLfloat* value);
    void setFloatVec4(const char* name, int size, const GLfloat* value);
    void setUnifromMatrix3f(const char* name, int size, const GLfloat* matrix);
    void setUnifromMatrix4f(const char* name, int size, const GLfloat* matrix);
protected:
    void processImage(GLuint textureId);
    void processImage(GLuint textureId, const GLfloat* vertexCoordinate, const GLfloat* textureCoordinate);
    virtual void runOnDrawTasks();
    virtual void onDrawArrays();
    
private:
    void createProgram(const char* vertex, const char* fragment);
    void compileShader(const char* shaderString, GLuint shader);
    void link();
private:
    GLuint program;
    int width;
    int height;
};

#endif /* opengl_h */
