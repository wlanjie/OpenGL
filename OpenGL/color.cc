//
//  color.cc
//  OpenGL
//
//  Created by wlanjie on 2018/9/11.
//  Copyright © 2018年 com.wlanjie.opengl. All rights reserved.
//

#include "color.h"
#include <math.h>

static const char* colorFragmentShader =
"uniform float u_time;\n"
"uniform vec2 u_resolution;\n"
"varying vec2 textureCoordinate;\n"
"uniform sampler2D inputImageTexture;\n"
"void main() {\n"
"   vec4 textureColor = texture2D(inputImageTexture, textureCoordinate);\n"
"   // gl_FragColor = vec4(abs(sin(u_time)), 0.0, 0.0, 1.0);\n"
"   vec2 st = gl_FragCoord.xy/u_resolution;\n"
"   gl_FragColor = vec4(st.x, st.y, 0.0, 1.0);"
"}\n";

static const char* colorSmoothstepFragmentShader =
"uniform float u_time;\n"
"uniform vec2 u_resolution;\n"
"varying vec2 textureCoordinate;\n"
"uniform sampler2D inputImageTexture;\n"
"float plot(vec2 st, float pct) {\n"
"   return smoothstep(pct-0.02, pct, st.y) - smoothstep(pct, pct + 0.02, st.y);\n"
"}\n"
"void main() {\n"
"   vec2 st = gl_FragCoord.xy/u_resolution;\n"
"   vec3 color = vec3(st.x);\n"
"   float pct = plot(st, st.x);\n"
"   color = (1.0 - pct) * color + pct * vec3(0.0, 1.0, 0.0);\n"
"   gl_FragColor = vec4(color, 1.0);"
"}\n";

static const char* circleBaseFragmentShader =
    "uniform vec2 u_resolution;\n"
    "uniform float u_time;\n"
    "void main() {\n"
    "   vec2 st = gl_FragCoord.xy/u_resolution;\n"
    "   float pct = 0.0;\n"
    "   pct = distance(st, vec2(0.5));\n"
    "   vec3 color = vec3(pct);\n"
    "   gl_FragColor = mix(vec4(1.0, 0.5, 0.5, 1.0), vec4(color, 1.0), 0.5);\n"
    "}\n";


// 圆
//uniform vec2 u_resolution;
//uniform vec2 u_mouse;
//uniform float u_time;
//
//float circle(in vec2 _st, in float _radius){
//    vec2 dist = _st-vec2(0.5);
//    return 1.-smoothstep(_radius-(_radius*0.01),
//                         _radius+(_radius*0.01),
//                         dot(dist,dist)*4.0);
//}
//
//void main(){
//    vec2 st = gl_FragCoord.xy/u_resolution.xy;
//
//    vec3 color = vec3(circle(st,0.9));
//
//    gl_FragColor = vec4( color, 1.0 );
//}


Color::Color(int width, int height) {
    this->width = width;
    this->height = height;
    shader = new ShaderProgram(defaultVertexShader, circleBaseFragmentShader);
    
    glGenTextures(1, &textureId);
    glGenFramebuffers(1, &frameBufferId);
    glBindTexture(GL_TEXTURE_2D, textureId);
    glBindFramebuffer(GL_FRAMEBUFFER, frameBufferId);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureId, 0);
    
    int status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (status != GL_FRAMEBUFFER_COMPLETE) {
        printf("frame buffer error\n");
    }
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    startTime = time(nullptr);
    resolution[0] = width;
    resolution[1] = height;
}

Color::Color() {
    if (shader) {
        delete shader;
        shader = nullptr;
    }
    
    glDeleteTextures(1, &textureId);
    glDeleteFramebuffers(1, &frameBufferId);
}

GLuint Color::processImage(int textureId) {
    time_t now = time(nullptr);
    auto time = now - startTime;
//    auto value = (GLfloat) abs(cos(time));
//    printf("red = %f \n", abs(cos(time)));
//    printf("value = %f\n", value);
    shader->use();
//    glBindFramebuffer(GL_FRAMEBUFFER, frameBufferId);
    glViewport(0, 0, width, height);
    glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//    shader->setValue(value, "u_time");
    auto timeIndex = shader->uniformIndex("u_time");
    glUniform1f(timeIndex, (GLfloat) time);
    auto resolutionIndex = shader->uniformIndex("u_resolution");
    glUniform2fv(resolutionIndex, 1, resolution);
    auto positionAttribute = shader->attributeIndex("position");
    glVertexAttribPointer(positionAttribute, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), defaultVertexCoordinates);
    auto textureCoordinateAttribute = shader->attributeIndex("inputTextureCoordinate");
    glVertexAttribPointer(textureCoordinateAttribute, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), defaultTextureCoordinate);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureId);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindTexture(GL_TEXTURE_2D, 0);
//    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    return this->textureId;
}
