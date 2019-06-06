//
//  red_blue.h
//  OpenGL
//
//  Created by wlanjie on 2019/6/6.
//  Copyright © 2019年 com.wlanjie.opengl. All rights reserved.
//

#ifndef red_blue_h
#define red_blue_h

#include "frame_buffer.h"

class RedBlue : public FrameBuffer {
    
public:
    RedBlue(int width, int height);
    
protected:
    virtual void RunOnDrawTasks();
    
private:
    int width;
    int height;
    GLuint textureId;
    GLuint lookup_texture_id_;
    float scalePercent[16];
    int scalePercentIndex;
};


#endif /* red_blue_h */
