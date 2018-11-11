//
//  image_filter_group.h
//  OpenGL
//
//  Created by wlanjie on 2018/11/10.
//  Copyright © 2018 com.wlanjie.opengl. All rights reserved.
//

#ifndef image_filter_group_h
#define image_filter_group_h

#include <vector>
#include "image_filter.h"

namespace OpenGLImage {
    
class ImageFilterGroup : public ImageFilter {
public:
    ImageFilterGroup();
    ImageFilterGroup(std::vector<ImageFilter*> filters);
    ~ImageFilterGroup();
    void addFilter(ImageFilter* filter);
    void onInit();
    void onDestroy();
    void onOutputSizeChanged(const int width, const int height);
    void onDraw(const int textureId, const float* cubeBuffer, const float* textureBuffer);
    std::vector<ImageFilter*> getFilters();
    std::vector<ImageFilter*> getMergedFilters();
    void updateMergedFilters();
    
protected:
    
private:
    void destroyFrameBuffers();
    
};

}

#endif /* image_filter_group_h */
