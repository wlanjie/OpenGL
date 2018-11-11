//
//  image_filter.hpp
//  OpenGL
//
//  Created by wlanjie on 2018/11/10.
//  Copyright © 2018 com.wlanjie.opengl. All rights reserved.
//

#ifndef image_filter_h
#define image_filter_h

#include <stdio.h>
#include "opengl.h"

namespace OpenGLImage {

class ImageFilter {
public:
    ImageFilter();
    ImageFilter(const char* vertexShader, const char* fragmentShader);
    ~ImageFilter();
    void onInit();
    void onInitialized();
    void destroy();
    void onDestroy();
    void onOutputSizeChanged(const int width, const int height);
    void onDraw(const int textureId, const float* cubeBuffer, const float* textureBuffer);
    bool isInitialized();
    int getOutputWidth();
    int getOutputHeight();
    int getProgram();
    int getAttributePosition();
    int getAttributeTextureCoordinate();
    int getUniformTexture();

protected:
    void onDrawArrayPre();
    void runPedingOnDrawTasks();
    void setInteger(const int location, const int value);
    void setFloat(const int location, const float value);
    void setFloatVec2(const int location, const float* value);
    void setFloatVec3(const int location, const float* value);
    void setFloatVec4(const int location, const float* value);
    void setFloatArray(const int location, const int size, const float* value);
    void setPoint(const int location, const float x, const float y);
    void setUniformMatrix3f(const int location, const float* matrix);
    void setUniformMatrix4f(const int location, const float* matrix);
    
private:
    enum ShaderType {
        VERTEX,
        FRAGMENT
    };
    GLuint compileShader(const char* shaderString, ShaderType type);
    void link();
    void checkGlError(const char* op);
private:
    bool initialized;
    GLuint program;
    char* vertexShader;
    char* fragmentShader;
    GLint position;
    GLint inputTextureCoordinate;
    GLint inputImageTexture;
    int outputWidth;
    int outputHeight;
};

}


#endif /* image_filter_hpp */
