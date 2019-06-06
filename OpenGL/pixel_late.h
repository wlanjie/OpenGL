//
//  pixel_late.h
//  OpenGL
//
//  Created by wlanjie on 2018/11/16.
//  Copyright © 2018年 com.wlanjie.opengl. All rights reserved.
//

#ifndef pixel_late_h
#define pixel_late_h

#include "frame_buffer.h"

class PixelLate : public FrameBuffer {
    
public:
    PixelLate(int width, int height);
    ~PixelLate();
    GLuint onDrawFrame(int textureId);
    
protected:
    virtual void RunOnDrawTasks();
    virtual void OnDrawArrays();
    
private:
    int width;
    int height;
};

#endif /* pixel_late_h */
