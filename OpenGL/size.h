//
// Created by wlanjie on 2018/3/19.
//

#ifndef OPENGL_SIZE_H
#define OPENGL_SIZE_H

#include <cmath>

class Size {
public:
    Size(float width, float height) {
        this->width = width;
        this->height = height;
    }

    float getWidth() { return width; }
    float getHeight() { return height; }
private:
    float width;
    float height;
};

class GLSize {
public:
    GLSize(int width, int height) {
        this->width = width;
        this->height = height;
    }

    GLSize(Size size) {
        this->width = int(round(double(size.getWidth())));
        this->height = int(round(double(size.getHeight())));
    }

    int getWidth() { return width; }
    int getHeight() { return height; }
private:
    int width;
    int height;
};

#endif //OPENGL_SIZE_H
