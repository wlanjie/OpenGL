//
//  image_filter_group.cc
//  OpenGL
//
//  Created by wlanjie on 2018/11/10.
//  Copyright © 2018 com.wlanjie.opengl. All rights reserved.
//

#include "image_filter_group.h"

namespace OpenGLImage {
    
ImageFilterGroup::ImageFilterGroup() {
        
}
    
ImageFilterGroup::ImageFilterGroup(std::vector<ImageFilter*> filters) {
        
}
    
    ImageFilterGroup::onInit() {
        ImageFilter::onInit();
        
    }
    
}
