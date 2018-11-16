//
//  glitch_filter.h
//  OpenGL
//
//  Created by wlanjie on 2018/11/15.
//  Copyright © 2018年 com.wlanjie.opengl. All rights reserved.
//

#ifndef glitch_filter_h
#define glitch_filter_h

#include "opengl.h"

class GlitchFilter : public OpenGL {
public:
    GlitchFilter(int width, int height);
    ~GlitchFilter();
    
    GLuint onDrawFrame(GLuint textureId);
protected:
    virtual void runOnDrawTasks() override;
    virtual void onDrawArrays() override;
private:
    int frames;
    float progress;
    float* driftSequence;
    float* jitterSequence;
    float* threshHoldSequence;
};

#endif /* glitch_filter_h */
