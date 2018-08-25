//
// Created by wlanjie on 2018/3/18.
//

#ifndef OPENGL_COLOR_H
#define OPENGL_COLOR_H

class Color {
public:
    Color(float red, float green, float blue, float alpha = 1.0f) :
            redColor(0),
            greenColor(0),
            blueColor(0),
            alphaColor(0) {
        this->redColor = red;
        this->greenColor = green;
        this->blueColor = blue;
        this->alphaColor = alpha;
    }

    float getRed() { return redColor; }
    float getGreen() { return greenColor; }
    float getBlue() { return blueColor; }
    float getAlpha() { return alphaColor; }

    static Color* black() {
        return new Color(0.0f, 0.0f, 0.0f, 1.0f);
    }

    static Color* white() {
        return new Color(1.0f, 1.0f, 1.0f, 1.0f);
    }

    static Color* red() {
        return new Color(1.0f, 0.0f, 0.0f, 1.0f);
    }

    static Color* green() {
        return new Color(0.0f, 1.0f, 0.0f, 1.0f);
    }

    static Color* blue() {
        return new Color(0.0f, 0.0f, 1.0f, 1.0f);
    }

    static Color* transparent() {
        return new Color(0.0f, 0.0f, 0.0f, 0.0f);
    }
private:
    float redColor;
    float greenColor;
    float blueColor;
    float alphaColor;
};

#endif //OPENGL_COLOR_H
