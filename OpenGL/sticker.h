//
//  sticker.h
//  OpenGL
//
//  Created by wlanjie on 2018/11/15.
//  Copyright © 2018年 com.wlanjie.opengl. All rights reserved.
//

#ifndef sticker_h
#define sticker_h

#include "opengl.h"

class Sticker : public OpenGL {
public:
    Sticker(int width, int height);
    ~Sticker();
    GLuint onDrawFrame(GLuint textureId);
protected:
    virtual void RunOnDrawTasks() override;
    virtual void OnDrawArrays() override;
private:
    GLuint textureId;
    GLuint frameBufferId;
    int width;
    int height;
    GLfloat vertex[8];
    GLfloat fragment[8];
    GLuint stickerId;
};

#endif /* sticker_h */
