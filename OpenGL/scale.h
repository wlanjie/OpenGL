//
//  scale.h
//  OpenGL
//
//  Created by wlanjie on 2018/11/15.
//  Copyright © 2018年 com.wlanjie.opengl. All rights reserved.
//

#ifndef scale_h
#define scale_h

#include "opengl.h"

class Scale : OpenGL {
public:
    Scale(int width, int height);
    ~Scale();
    GLuint onDrawFrame(GLuint textureId);
protected:
    virtual void runOnDrawTasks() override;
    virtual void onDrawArrays() override;
private:
    float progress;
    int frames;
    float scale;
    bool maxScale;
};

#endif /* scale_h */
