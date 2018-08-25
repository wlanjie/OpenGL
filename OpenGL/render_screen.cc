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

GLfloat renderVertex[] = {
    -1.0f, -1.0f,
    1.0f, -1.0f,
    -1.0f, 1.0f,
    1.0f, 1.0f,
};

GLfloat renderTextureCoordinates[] = {
    0.0f, 0.0f,
    1.0f, 0.0f,
    0.0f, 1.0f,
    1.0f, 1.0f
};

auto renderVertexShader =
    "attribute vec4 position;\n "
    "attribute vec4 inputTextureCoordinate;\n \n "
    "varying vec2 textureCoordinate;\n \n "
    "void main()\n {\n     "
    "   gl_Position = position;\n     "
    "   textureCoordinate = inputTextureCoordinate.xy;\n "
    "}\n ";

auto renderFragmentShader =
    "varying vec2 textureCoordinate;\n"
    "uniform sampler2D inputImageTexture;\n"
    "void main() {\n"
    "   vec4 textureColor = texture2D(inputImageTexture, textureCoordinate);\n"
    "   gl_FragColor = textureColor;\n"
    "}\n";

RenderScreen::RenderScreen() {
    shader = new ShaderProgram(renderVertexShader, renderFragmentShader);
}

RenderScreen::~RenderScreen() {
    if (shader) {
        delete shader;
        shader = nullptr;
    }
}

void RenderScreen::draw(int textureId) {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    auto positionAttribute = shader->attributeIndex("position");
    glVertexAttribPointer(positionAttribute, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), renderVertex);
    auto textureCoordinateAttribute = shader->attributeIndex("inputTextureCoordinate");
    glVertexAttribPointer(textureCoordinateAttribute, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), renderTextureCoordinates);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureId);
    shader->use();
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindTexture(GL_TEXTURE_2D, 0);
}
