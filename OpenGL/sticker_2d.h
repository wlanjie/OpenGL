//
//  sticker_2d.h
//  OpenGL
//
//  Created by wlanjie on 2019/6/6.
//  Copyright © 2019年 com.wlanjie.opengl. All rights reserved.
//

#ifndef sticker_2d_h
#define sticker_2d_h

#include "frame_buffer.h"

class Sticker2D : public FrameBuffer {
    
public:
    Sticker2D(int width, int height);
    ~Sticker2D();
    GLuint onDrawFrame(int textureId);
    
protected:
    virtual void RunOnDrawTasks();
    virtual void OnDrawArrays();
    
private:
    int width;
    int height;
    GLuint textureId;
    
    float colorDisx[9];
    float timeStamp[90];
    
    int colorDisxIndex;
    int timeStampIndex;
};


#endif /* sticker_2d_h */
