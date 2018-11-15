//
//  glitch_filter.cc
//  OpenGL
//
//  Created by wlanjie on 2018/11/15.
//  Copyright © 2018年 com.wlanjie.opengl. All rights reserved.
//

#include "glitch_filter.h"
#define GLM_ENABLE_EXPERIMENTAL
#include <transform.hpp>
#include <type_ptr.hpp>
#include "shader.h"

#define MAX_FRAMES 8
#define SKIP_FRAMES 19

GlitchFilter::GlitchFilter(int width, int height) : OpenGL(width, height, defaultVertexShader, glitchFilterFragmentShader) {
    frames = 0;
    progress = 0;
    driftSequence = new float[MAX_FRAMES];
    driftSequence[0] = 0.0f;
    driftSequence[1] = 0.03f;
    driftSequence[2] = 0.032f;
    driftSequence[3] = 0.035f;
    driftSequence[4] = 0.03f;
    driftSequence[5] = 0.032f;
    driftSequence[6] = 0.031f;
    driftSequence[7] = 0.029f;
    driftSequence[8] = 0.025f;
    
    jitterSequence = new float[MAX_FRAMES];
    jitterSequence[0] = 0.0f;
    jitterSequence[1] = 0.03f;
    jitterSequence[2] = 0.01f;
    jitterSequence[3] = 0.02f;
    jitterSequence[4] = 0.05f;
    jitterSequence[5] = 0.055f;
    jitterSequence[6] = 0.03f;
    jitterSequence[7] = 0.02f;
    jitterSequence[7] = 0.025f;
    
    threshHoldSequence = new float[MAX_FRAMES];
    threshHoldSequence[0] = 1.0f;
    threshHoldSequence[1] = 0.965f;
    threshHoldSequence[2] = 0.9f;
    threshHoldSequence[3] = 0.9f;
    threshHoldSequence[4] = 0.9f;
    threshHoldSequence[5] = 0.6f;
    threshHoldSequence[6] = 0.8f;
    threshHoldSequence[7] = 0.5f;
    threshHoldSequence[8] = 0.5f;
}

GlitchFilter::~GlitchFilter() {
    delete[] driftSequence;
    delete[] jitterSequence;
    delete[] threshHoldSequence;
}

void GlitchFilter::runOnDrawTasks(GLuint programId) {
    frames++;
    if (frames > MAX_FRAMES) {
        frames = 0;
    }
    auto driftUniform = glGetUniformLocation(programId, "colorDrift");
    glUniform1f(driftUniform, driftSequence[frames]);
    auto jitterUniform = glGetUniformLocation(programId, "scanLineJitter");
    glUniform2fv(jitterUniform, 1, new float[2]{jitterSequence[frames], threshHoldSequence[frames]});
}

void GlitchFilter::onDrawArrays() {
    
}

GLuint GlitchFilter::onDrawFrame(GLuint textureId) {
    return processImage(textureId, defaultVertexCoordinates, defaultTextureCoordinate);
}
