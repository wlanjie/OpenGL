//
//  image_process.hpp
//  OpenGL
//
//  Created by wlanjie on 2019/6/6.
//  Copyright © 2019 com.wlanjie.opengl. All rights reserved.
//

#ifndef image_process_h
#define image_process_h

#include <vector>
#include "action.h"

#define NO_ACTION -1

namespace trinity {

class ImageProcess {
public:
    ImageProcess();
    ~ImageProcess();
    
    void RemoveAction(int action_id);
    void ClearAction();
    
    int AddFlashAction(int frame, uint64_t start_time, uint64_t end_time, int action_id = NO_ACTION);
private:
    int AddAction(ActionType type, void* args);
private:
    std::vector<Action> actions_;
    uint32_t current_aciton_id_;
};
    
}

#endif /* image_process_h */
