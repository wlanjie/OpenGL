//
//  fake_3d.h
//  OpenGL
//
//  Created by wlanjie on 2019/6/6.
//  Copyright © 2019年 com.wlanjie.opengl. All rights reserved.
//

#ifndef fake_3d_h
#define fake_3d_h

#include "frame_buffer.h"

class Fake3D : public FrameBuffer {
    
public:
    Fake3D(int width, int height);
    
protected:
    virtual void RunOnDrawTasks();
    
private:
    int width;
    int height;
    GLuint textureId;
    float scalePercent[16];
    int scalePercentIndex;
};

#endif /* fake_3d_h */
