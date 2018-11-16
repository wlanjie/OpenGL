//
//  flash_white.h
//  OpenGL
//
//  Created by wlanjie on 2018/11/15.
//  Copyright © 2018年 com.wlanjie.opengl. All rights reserved.
//

#ifndef flash_white_h
#define flash_white_h

#include "frame_buffer.h"

class FlashWhite : public FrameBuffer {
public:
    FlashWhite(int width, int height);
    ~FlashWhite();
  
    GLuint onDrawFrame(GLuint textureId);
    
protected:
    virtual void runOnDrawTasks() override;
    
private:
    int frames;
    float progress;
};

#endif /* flash_white_h */
