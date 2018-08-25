//
// Created by wlanjie on 2018/3/19.
//

#ifndef OPENGL_OPENGL_RENDERING_H
#define OPENGL_OPENGL_RENDERING_H

#include <OpenGL/gl.h>
#include <vector>
#include "shader_program.h"
#include "framebuffer.h"
#include "size.h"

enum InputTextureStorageFormat {
    NEON,
    TEXTURE_COORDINATES,
    TEXTURE_VBO
};

class InputTextureProperties {
public:
    InputTextureProperties(GLfloat* textureCoordinates = nullptr, GLuint textureVBO = 0, GLuint texture = 0) :
            textureCoordinates(nullptr),
            textureVBO(0),
            texture(0),
            format(NEON) {
        if (textureCoordinates) {
            format = TEXTURE_COORDINATES;
        }
        if (textureVBO) {
            format = TEXTURE_VBO;
        }
        this->textureCoordinates = textureCoordinates;
        this->textureVBO = textureVBO;
        this->texture = texture;
    }

    GLfloat* getTextureCoordinates() {
        return textureCoordinates;
    }

    GLuint getTextureVBO() {
        return textureVBO;
    }

    GLuint getTexture() {
        return texture;
    }

    InputTextureStorageFormat getFormat() {
        return format;
    }

private:
    GLfloat *textureCoordinates;
    GLuint textureVBO;
    GLuint texture;
    InputTextureStorageFormat format;
};

class OpenGLRendering{
public:
    OpenGLRendering();
    ~OpenGLRendering();

    void renderQuadWithShader(ShaderProgram *shader, std::vector<InputTextureProperties*>& inputTextures, GLfloat* vertices = nullptr, GLuint vertexBufferObject = 0, GLfloat* texture = nullptr);
    void clearFrameBufferWithColor(Color* color);
    void renderStencilMaskFrameBuffer(Framebuffer* frameBuffer);
    GLuint generateTexture(GLint minFilter, GLint magFilter, GLint wrapS, GLint wrapT);
    void uploadLocalArray(GLfloat* data, GLuint texture, GLSize& size);
    GLuint generateFramebufferForTexture(GLuint texture, GLint width, GLint height, int internalFormat, GLenum format, GLenum type);
    GLuint generateVBO(GLfloat* vertices);
    void deleteVBO(GLuint vbo);
private:
    GLenum textureUnitForIndex(int index);
    const char* withNonZeroSuffix(const char* input, int index);
};


#endif //OPENGL_OPENGL_RENDERING_H
