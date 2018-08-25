//
//  mirror.cpp
//  OpenGL
//
//  Created by wlanjie on 2018/6/2.
//  Copyright © 2018年 com.wlanjie.opengl. All rights reserved.
//

#include "stb_image.h"
#include "mirror.h"
#include "opengl.h"


auto mirrorVertexSource =
"attribute vec4 position;\n "
"attribute vec4 inputTextureCoordinate;\n \n "
"varying vec2 textureCoordinate;\n \n "
"void main()\n {\n     "
"gl_Position = position;\n     "
"textureCoordinate = inputTextureCoordinate.xy;\n "
"}\n ";

auto mirrorFragmentSource =
"varying vec2 textureCoordinate;\n"
"uniform sampler2D inputImageTexture;\n"
"void main() {\n"
"   vec4 textureColor = texture2D(inputImageTexture, textureCoordinate);\n"
"   gl_FragColor = textureColor;\n"
"}\n";

GLfloat mirrorVertex[] = {
    -1.0f, -1.0f,
    1.0f, -1.0f,
    -1.0f, 1.0f,
    1.0f, 1.0f,
};

GLfloat mirrorTextureCoordinates[] = {
    0.0f, 0.0f,
    1.0f, 0.0f,
    0.0f, 1.0f,
    1.0f, 1.0f
};

GLfloat mirrorTriangleVertex[] = {
    
    // 左上角
    -1.0f, 0.0f,
    0.0f, 1.0f,
    -1.0f, 1.0f,
    
    -1.0f, 0.0f,
    0.0f, 0.0f,
    0.0f, 1.0f,
    
    // 右上角
    0.0f, 0.0f,
    1.0f, 1.0f,
    0.0f, 1.0f,
    
    0.0f, 0.0f,
    1.0f, 0.0f,
    1.0f, 1.0f,
    
    // 左下角
    -1.0f, -1.0f,
    0.0f, 0.0f,
    -1.0f, 0.0f,
    
    -1.0f, -1.0f,
    0.0f, -1.0f,
    0.0f, 0.0f,
    
    // 右下角
    0.0f, -1.0f,
    1.0f, 0.0f,
    0.0f, 0.0f,
    
    0.0f, -1.0f,
    1.0f, -1.0f,
    1.0f, 0.0f,
    
};

GLfloat mirrorTriangleTexture[] = {
    // 左上角
    0.0f, 0.0f,
    1.0f, 1.0f,
    0.0f, 1.0f,
    
    0.0f, 0.0f,
    1.0f, 0.0f,
    1.0f, 1.0f,
    
    // 右上角
    1.0f, 0.0f,
    0.0f, 1.0f,
    1.0f, 1.0f,
    
    1.0f, 0.0f,
    0.0f, 0.0f,
    0.0f, 1.0f,
    
    // 左下角
    //    1.0f, 1.0f,
    0.0f, 1.0f,
    1.0f, 0.0f,
    0.0f, 0.0f,
    
    0.0f, 1.0f,
    1.0f, 1.0f,
    1.0f, 0.0f,
    
    // 右下角
    1.0f, 1.0f,
    0.0f, 0.0f,
    1.0f, 0.0f,
    
    1.0f, 1.0f,
    0.0f, 1.0f,
    0.0f, 0.0f
};


GLuint generateVBO(GLfloat *vertices) {
    GLuint newBuffer = 0;
    glGenBuffers(1, &newBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, newBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    return newBuffer;
}

Mirror::Mirror(const char* filename, bool smoothlyScaleOutput) : imageFramebuffer(nullptr) {
    rendering = new OpenGLRendering();
    backgroundColor = Color::black();
    shader = new ShaderProgram(mirrorVertexSource, mirrorFragmentSource);
    
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char *data = stbi_load(filename, &width, &height, &nrChannels, 0);
    
    size = new GLSize(width, height);
    imageFramebuffer = new Framebuffer(size, true, GL_LINEAR, GL_LINEAR, GL_REPEAT, GL_REPEAT);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, imageFramebuffer->getTexture());
    if (smoothlyScaleOutput) {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    }
    if (smoothlyScaleOutput) {
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glBindTexture(GL_TEXTURE_2D, 0);
    //    glGenBuffers(1, &vbo);
    //    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    //    glBufferData(GL_ARRAY_BUFFER, sizeof(vertex), vertex, GL_STATIC_DRAW);
    //    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    InputTextureProperties *properties = new InputTextureProperties(mirrorVertex, 0, imageFramebuffer->getTexture());
    inputTextures.push_back(properties);
    stbi_image_free(data);
}

Mirror::~Mirror() {
    if (imageFramebuffer) {
        delete imageFramebuffer;
        imageFramebuffer = nullptr;
    }
    if (rendering) {
        delete rendering;
        rendering = nullptr;
    }
    if (shader) {
        delete shader;
        shader = nullptr;
    }
    if (size) {
        delete size;
        size = nullptr;
    }
    
}

int Mirror::processImage(bool synchronously) {
    //    rendering->clearFrameBufferWithColor(backgroundColor);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    auto positionAttribute = shader->attributeIndex("position");
    glVertexAttribPointer(positionAttribute, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), mirrorTriangleVertex);
    auto textureCoordinateAttribute = shader->attributeIndex("inputTextureCoordinate");
    glVertexAttribPointer(textureCoordinateAttribute, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), mirrorTriangleTexture);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, imageFramebuffer->getTexture());
    shader->use();
    glDrawArrays(GL_TRIANGLES, 0, 24);
    glBindTexture(GL_TEXTURE_2D, 0);
    //    rendering->renderQuadWithShader(shader, inputTextures, vertex, vbo, textureCoordinates);
    return imageFramebuffer->getTexture();
}
