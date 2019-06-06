//
//  huan_ying.h
//  OpenGL
//
//  Created by wlanjie on 2019/6/6.
//  Copyright © 2019年 com.wlanjie.opengl. All rights reserved.
//

#ifndef huan_ying_h
#define huan_ying_h

#include "frame_buffer.h"

class HuanYing : public FrameBuffer {
    
public:
    HuanYing(int width, int height);
    ~HuanYing();
    GLuint onDrawFrame(int textureId);
    
protected:
    virtual void runOnDrawTasks();
    virtual void onDrawArrays();
    
private:
    int width;
    int height;
    GLuint textureId;
};

#endif /* huan_ying_h */
