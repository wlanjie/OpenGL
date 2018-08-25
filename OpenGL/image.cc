//
// Created by wlanjie on 2018/3/20.
//

#include "image.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <OpenGL/OpenGL.h>

auto vertexSource =
"attribute vec4 position;\n "
"attribute vec4 inputTextureCoordinate;\n \n "
"varying vec2 textureCoordinate;\n \n "
"void main()\n {\n     "
"gl_Position = position;\n     "
"textureCoordinate = inputTextureCoordinate.xy;\n "
"}\n ";

auto fragmentMaskSource =
"varying vec2 textureCoordinate;\n"
"uniform sampler2D inputImageTexture;\n"
"void main() {\n"
"   gl_FragColor = texture2D(inputImageTexture, textureCoordinate);\n"
"   float radius = distance(textureCoordinate, vec2(0.5, 0.5)); \n"
"   float alpha = (radius >= 0.495)? 0.5 : ((radius <= 0.490)? 0.0 : (radius-0.490)*200.0);  \n"
"   gl_FragColor = mix(gl_FragColor, vec4(1.0, 1.0, 1.0, 1.0), alpha);"
"}\n";

auto fragmentDianJi =
"varying vec2 textureCoordinate;\n"
"uniform sampler2D inputImageTexture;\n"
"void main() {\n"
"   vec4 textureColor = texture2D(inputImageTexture, textureCoordinate);\n"
"   gl_FragColor = vec4((1.0 - textureColor.rgb), textureColor.a);\n"
"}\n";

auto fragmentGreySource =
"varying vec2 textureCoordinate;\n"
"uniform sampler2D inputImageTexture;\n"
"const vec3 grey = vec3(0.3, 0.59, 0.11);\n"
"void main() {\n"
"   vec4 textureColor = texture2D(inputImageTexture, textureCoordinate);\n"
"   float luminance = dot(textureColor.rgb, grey);\n"
"   gl_FragColor = vec4(vec3(luminance), textureColor.a);\n"
"}\n";

auto fragmentSource =
"varying vec2 textureCoordinate;\n"
"uniform sampler2D inputImageTexture;\n"
"void main() {\n"
"   vec4 textureColor = texture2D(inputImageTexture, textureCoordinate);\n"
"   gl_FragColor = textureColor;\n"
"}\n";

auto blur =
"uniform sampler2D inputImageTexture;\n"
"varying vec2 textureCoordinate;\n"
"void main() {\n"
"    float block = 150.0;\n"
"    float delta = 1.0/block;\n"
"    vec4 color = vec4(0.0);\n"
"    float factor[9];\n"
"    factor[0] = 0.0947416; factor[1] = 0.118318; factor[2] = 0.0947416;\n"
"    factor[3] = 0.118318; factor[4] = 0.147761; factor[5] = 0.118318;\n"
"    factor[6] = 0.0947416; factor[7] = 0.118318; factor[8] = 0.0947416;\n"
"    for (int i = -1; i <= 1; i++) {\n"
"        for (int j = -1; j <= 1; j++) {\n"
"            float x = max(0.0, textureCoordinate.x + float(i) * delta);\n"
"            float y = max(0.0, textureCoordinate.y + float(i) * delta);\n"
"            color += texture2D(inputImageTexture, vec2(x, y)) * factor[(i+1)*3+(j+1)];\n"
"        }\n"
"    }\n"
"    gl_FragColor = vec4(vec3(color), 1.0);\n"
"}\n";

GLfloat vertex[] = {
    -1.0f, -1.0f,
    1.0f, -1.0f,
    -1.0f, 1.0f,
    1.0f, 1.0f,
};

GLfloat textureCoordinates[] = {
    0.0f, 0.0f,
    1.0f, 0.0f,
    0.0f, 1.0f,
    1.0f, 1.0f
};

GLfloat triangleVertex[] = {
    
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

GLfloat triangleTexture[] = {
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

Image::Image(const char* filename, bool smoothlyScaleOutput) : imageFramebuffer(nullptr) {
    rendering = new OpenGLRendering();
    backgroundColor = Color::black();
    shader = new ShaderProgram(vertexSource, blur);

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

    InputTextureProperties *properties = new InputTextureProperties(vertex, 0, imageFramebuffer->getTexture());
    inputTextures.push_back(properties);
    stbi_image_free(data);
}

Image::~Image() {
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

void Image::processImage(bool synchronously) {
//    rendering->clearFrameBufferWithColor(backgroundColor);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    auto positionAttribute = shader->attributeIndex("position");
    glVertexAttribPointer(positionAttribute, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), vertex);
    auto textureCoordinateAttribute = shader->attributeIndex("inputTextureCoordinate");
    glVertexAttribPointer(textureCoordinateAttribute, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), textureCoordinates);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, imageFramebuffer->getTexture());
    shader->use();
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindTexture(GL_TEXTURE_2D, 0);
//    rendering->renderQuadWithShader(shader, inputTextures, vertex, vbo, textureCoordinates);
}
