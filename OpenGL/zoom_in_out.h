//
//  zoom_in_out.h
//  OpenGL
//
//  Created by wlanjie on 2019/6/6.
//  Copyright © 2019年 com.wlanjie.opengl. All rights reserved.
//

#ifndef zoom_in_out_h
#define zoom_in_out_h

#include "frame_buffer.h"

class ZoomInOut : public FrameBuffer {
    
public:
    ZoomInOut(int width, int height);
    
protected:
    virtual void runOnDrawTasks();
    
private:
    int width;
    int height;
    GLuint textureId;
    float scalePercent[16];
    int scalePercentIndex;
};

#endif /* zoom_in_out_h */
