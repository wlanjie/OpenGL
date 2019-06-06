//
//  shake.h
//  OpenGL
//
//  Created by wlanjie on 2018/11/15.
//  Copyright © 2018年 com.wlanjie.opengl. All rights reserved.
//

#ifndef shake_h
#define shake_h

#include "opengl.h"

class Shake : public OpenGL {
public:
    Shake(int width, int height);
    ~Shake();
    
    GLuint onDrawFrame(int textureId);
    
protected:
    virtual void RunOnDrawTasks() override;
    virtual void OnDrawArrays() override;
    
private:
    int frames;
    float progress;
};

#endif /* shake_h */
