//
//  blend.h
//  OpenGL
//
//  Created by wlanjie on 2018/11/15.
//  Copyright © 2018年 com.wlanjie.opengl. All rights reserved.
//

#ifndef blend_h
#define blend_h

#include "opengl.h"

class Blend : public OpenGL {
public:
    Blend(int width, int height);
    ~Blend();
    
    GLuint onDrawFrame(GLuint textureId);
protected:
    virtual void runOnDrawTasks(GLuint programId) override;
    virtual void onDrawArrays() override;
private:
    int frames;
    float progress;
    GLuint blendTextureId;
};

#endif /* blend_h */
