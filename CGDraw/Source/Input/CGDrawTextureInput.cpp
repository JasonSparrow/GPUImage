//
//  CGDrawTextureInput.cpp
//  CGDraw
//
//  Created by Jason on 2021/6/11.
//

#include "CGDrawTextureInput.h"

void CGDraw::CGDrawTextureInput::requestRender() {

    std::list<CGDrawInput *>::iterator iter;
    for(iter = mTargetList.begin(); iter != mTargetList.end() ;iter++) {
        CGDrawInput *target = *iter;
        target->newFramebufferAvailable(mOutputFramebuffer);
    }
}
