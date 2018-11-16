//
//  render_screen.cc
//  OpenGL
//
//  Created by wlanjie on 2018/6/8.
//  Copyright © 2018年 com.wlanjie.opengl. All rights reserved.
//

#include "render_screen.h"
#include <OpenGL/OpenGL.h>
#include <OpenGL/gl.h>
#include "shader.h"

RenderScreen::RenderScreen() : OpenGL(defaultVertexShader, defaultFragmentShader) {
}

RenderScreen::~RenderScreen() {

}

void RenderScreen::draw(int textureId) {
    processImage(textureId);
}
