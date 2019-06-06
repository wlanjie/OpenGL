//
//  frame_buffer.h
//  OpenGL
//
//  Created by wlanjie on 2018/11/16.
//  Copyright © 2018年 com.wlanjie.opengl. All rights reserved.
//

#ifndef frame_buffer_h
#define frame_buffer_h

#include "opengl.h"

class FrameBuffer : public OpenGL {
public:
    FrameBuffer(int width, int height, const char* vertex, const char* fragment);
    ~FrameBuffer();
    
    GLuint onDrawFrame(GLuint textureId);
    GLuint onDrawFrame(GLuint textureId, const GLfloat* vertexCoordinate, const GLfloat* textureCoordinate);
protected:
    virtual void RunOnDrawTasks();
    virtual void OnDrawArrays();
private:
    GLuint textureId;
    GLuint frameBufferId;
};

#endif /* frame_buffer_h */
