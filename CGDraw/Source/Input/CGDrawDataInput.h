//
//  CGDrawDataInput.h
//  CGDraw
//
//  Created by Jason on 2021/6/11.
//

#ifndef CGDrawDataInput_h
#define CGDrawDataInput_h

#include "CGDrawOutput.h"
namespace CGDraw {

class CGDrawDataInput:public CGDrawOutput {
    
public:
    void setInputData(const unsigned char *data, int width, int height) {
        CGVec2f _size = CGVec2f(width, height);
        framebuffer.genWithSize(_size, true);
        framebuffer.bindTexture();
        framebuffer.upload(data, _size, GL_RGBA, GL_RGBA, false);
        framebuffer.unbindTexture();
        mOutputFramebuffer = &framebuffer;
    }
    void requestRender() override {
        std::list<CGDrawInput *>::iterator iter;
        for(iter = mTargetList.begin(); iter != mTargetList.end(); iter++) {
            CGDrawInput *target = *iter;
            target->newFramebufferAvailable(mOutputFramebuffer);
        }
    }
private:
    CGDrawFramebuffer framebuffer;
};

}

#endif /* CGDrawDataInput_h */
