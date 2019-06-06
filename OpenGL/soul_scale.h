//
//  soul_scale.h
//  OpenGL
//
//  Created by wlanjie on 2019/6/6.
//  Copyright © 2019年 com.wlanjie.opengl. All rights reserved.
//

#ifndef soul_scale_h
#define soul_scale_h

#include "frame_buffer.h"

class SoulScale : public FrameBuffer {
    
public:
    SoulScale(int width, int height);
    ~SoulScale();
    GLuint onDrawFrame(int textureId);
    
protected:
    virtual void runOnDrawTasks();
    virtual void onDrawArrays();
    
private:
    int width;
    int height;
    GLuint textureId;
    
    float mixturePercent[16];
    float scalePercent[16];
    
    int mixturePercentIndex;
    int scalePercentIndex;
};


#endif /* soul_scale_h */
