//
// Created by wlanjie on 2018/3/19.
//

#ifndef OPENGL_POSITION_H
#define OPENGL_POSITION_H

class Position {
public:
    Position(float x, float y, float z = 0.0f) {
        this->x = x;
        this->y = y;
        this->z = z;
    }

    float getX() { return x; }
    float getY() { return y; }
    float getZ() { return z; }

    static Position* center() {
        return new Position(0.5f, 0.5f);
    }

    static Position* zero() {
        return new Position(0.0f, 0.0f);
    }

private:
    float x;
    float y;
    float z;
};

#endif //OPENGL_POSITION_H
