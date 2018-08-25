//
// Created by wlanjie on 2018/3/19.
//

#include "opengl_rendering.h"

OpenGLRendering::OpenGLRendering() {

}

OpenGLRendering::~OpenGLRendering() {

}

void OpenGLRendering::renderQuadWithShader(ShaderProgram *shader, std::vector<InputTextureProperties*>& inputTextures, GLfloat *vertices, GLuint vertexBufferObject, GLfloat* texture) {
    auto positionAttribute = shader->attributeIndex("position");
//    if (vertexBufferObject) {
//        glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
//        glVertexAttribPointer(positionAttribute, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), nullptr);
////        glBindBuffer(GL_ARRAY_BUFFER, 0);
//    } else {
//        glVertexAttribPointer(positionAttribute, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), vertices);
//    }
    
    glVertexAttribPointer(positionAttribute, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), vertices);
    auto textureCoordinateAttribute = shader->attributeIndex("inputTextureCoordinate");
    auto input = inputTextures[0];
    glVertexAttribPointer(textureCoordinateAttribute, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), texture);
    glActiveTexture(textureUnitForIndex(0));
    glBindTexture(GL_TEXTURE_2D, input->getTexture());
    shader->use();
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
//    if (vertexBufferObject) {
//        glBindBuffer(GL_ARRAY_BUFFER, 0);
//    }
//    for (int index = 0; index < inputTextures.size(); index++) {
//        glActiveTexture(textureUnitForIndex(index));
//        glBindTexture(GL_TEXTURE_2D, 0);
//    }

}

GLenum OpenGLRendering::textureUnitForIndex(int index) {
    switch (index) {
        case 0:
            return GL_TEXTURE0;
        case 1:
            return GL_TEXTURE1;
        case 2:
            return GL_TEXTURE2;
        case 3:
            return GL_TEXTURE3;
        case 4:
            return GL_TEXTURE4;
        case 5:
            return GL_TEXTURE5;
        case 6:
            return GL_TEXTURE6;
        case 7:
            return GL_TEXTURE7;
        case 8:
            return GL_TEXTURE8;
        default:
            break;
    }
    return 0;
}

const char *OpenGLRendering::withNonZeroSuffix(const char *input, int index) {
    if (index == 0) {
        return input;
    } else {
        std::string ret;
        ret += input;
        ret += (std::to_string(index + 1));
        return ret.c_str();
    }
}

void OpenGLRendering::clearFrameBufferWithColor(Color *color) {
    glClearColor(color->getRed(), color->getGreen(), color->getBlue(), color->getAlpha());
    glClear(GL_COLOR_BUFFER_BIT);
}

void OpenGLRendering::renderStencilMaskFrameBuffer(Framebuffer *frameBuffer) {

}

GLuint OpenGLRendering::generateTexture(GLint minFilter, GLint magFilter, GLint wrapS, GLint wrapT) {
    GLuint texture = 0;
    glActiveTexture(GL_TEXTURE1);
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapS);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapT);

    glBindTexture(GL_TEXTURE_2D, 0);
    return texture;
}

void OpenGLRendering::uploadLocalArray(GLfloat *data, GLuint texture, GLSize &size) {
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, size.getWidth(), size.getHeight(), 0, GL_RGBA, GL_FLOAT, data);
    glBindTexture(GL_TEXTURE_2D, 0);
}

GLuint OpenGLRendering::generateFramebufferForTexture(GLuint texture, GLint width, GLint height, int internalFormat,
                                                      GLenum format, GLenum type) {
    GLuint frameBuffer = 0;
    glActiveTexture(GL_TEXTURE1);
    glGenFramebuffers(1, &frameBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, type, nullptr);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);
    auto status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (status != GL_FRAMEBUFFER_COMPLETE) {
        return 0;
    }
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    return frameBuffer;
}

GLuint OpenGLRendering::generateVBO(GLfloat *vertices) {
    GLuint newBuffer = 0;
    glGenBuffers(1, &newBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, newBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat), vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    return newBuffer;
}

void OpenGLRendering::deleteVBO(GLuint vbo) {
    GLuint deleteVBO = vbo;
    glDeleteBuffers(1, &deleteVBO);
}
