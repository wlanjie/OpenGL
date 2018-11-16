//
//  render_screen.h
//  OpenGL
//
//  Created by wlanjie on 2018/6/8.
//  Copyright © 2018年 com.wlanjie.opengl. All rights reserved.
//

#ifndef render_screen_h
#define render_screen_h

#include "opengl.h"

class RenderScreen : public OpenGL {
public:
    RenderScreen();
    ~RenderScreen();
    void draw(int textureId);
};

#endif /* render_screen_h */
