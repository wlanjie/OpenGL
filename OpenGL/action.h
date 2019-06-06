//
//  action.h
//  OpenGL
//
//  Created by wlanjie on 2019/6/6.
//  Copyright © 2019 com.wlanjie.opengl. All rights reserved.
//

#ifndef action_h
#define action_h

enum ActionType {
    NONE,
    ROTATE,
    FLASH_WHITE
};

enum RotateType {
    ROTATE_90,
    ROTATE_180,
    ROTATE_270,
    ROTATE_FREE
};

typedef struct {
    uint64_t start_time;
    uint64_t end_time;
    uint32_t action_id;
    ActionType type;
    void* args;
} Action;

typedef struct {
    float rotate;
} RotateAction;

typedef struct {
    uint64_t start_time;
    uint64_t end_time;
    int flash_time;
} FlashWhiteAction;

#endif /* action_h */
