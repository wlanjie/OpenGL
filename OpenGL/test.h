//
//  test.h
//  OpenGL
//
//  Created by wlanjie on 2018/11/16.
//  Copyright © 2018年 com.wlanjie.opengl. All rights reserved.
//

#ifndef test_h
#define test_h

#include "frame_buffer.h"

class Test : public FrameBuffer {
    
public:
    Test(int width, int height);
    GLuint onDrawFrame(int textureId);
    
protected:
    virtual void RunOnDrawTasks() override;
    virtual void OnDrawArrays() override;
    
private:
    GLuint textureId;
    int frames;
    float progress;
};

#endif /* test_h */
