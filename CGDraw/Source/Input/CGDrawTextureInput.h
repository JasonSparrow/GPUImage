//
//  CGDrawTextureInput.h
//  CGDraw
//
//  Created by Jason on 2021/6/11.
//

#ifndef CGDrawTextureInput_h
#define CGDrawTextureInput_h

#include <stdio.h>
#include "CGDrawOutput.h"
#include "CGDrawInput.h"

namespace CGDraw {

class CGDrawTextureInput:public CGDrawOutput {
    
public:

    void requestRender() override {
        std::list<CGDrawInput *>::iterator iter;
        for(iter = mTargetList.begin(); iter != mTargetList.end() ;iter++) {
            CGDrawInput *target = *iter;
            target->newFramebufferAvailable(mOutputFramebuffer);
        }
    };
};

}
#endif /* CGDrawTextureInput_h */
