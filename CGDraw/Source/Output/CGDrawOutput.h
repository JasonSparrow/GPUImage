//
//  CGDrawOutput.h
//  CGDraw
//
//  Created by Jason on 2021/6/11.
//

#ifndef CGDrawOutput_h
#define CGDrawOutput_h

#include <stdio.h>
#include <list>
#include "CGDrawInput.h"
#include "CGDrawFramebuffer.h"

namespace CGDraw {
class CGDrawOutput {
            
public:
    virtual void addTarget(CGDrawInput *newtarget);
    virtual void removeTarget(CGDrawInput *targetToRemove);
    virtual void removeAllTargets();
    virtual std::list<CGDrawInput *>targets();
    virtual void requestRender();
    
protected:
    CGDrawFramebuffer *mOutputFramebuffer;
    std::list<CGDrawInput *>mTargetList;
};

}

#endif /* CGDrawOutput_h */

