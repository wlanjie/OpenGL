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
#include "shader_program.h"

class OpenGL {
public:
    OpenGL(int width, int height, const char* vertex, const char* fragment);
    ~OpenGL();
    
protected:
    GLuint processImage(GLuint textureId, const GLfloat* vertexCoordinate, const GLfloat* textureCoordinate);
    virtual void runOnDrawTasks(GLuint programId) = 0;
    virtual void onDrawArrays() = 0;
    
private:
    ShaderProgram* shader;
    GLuint textureId;
    GLuint frameBufferId;
    int width;
    int height;
};

#endif /* opengl_h */
