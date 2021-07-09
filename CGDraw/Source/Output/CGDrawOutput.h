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
    virtual void addTarget(CGDrawInput *newtarget) {
        mTargetList.push_back(newtarget);
    };
    virtual void removeTarget(CGDrawInput *targetToRemove) {
        mTargetList.remove(targetToRemove);
    };
    virtual void removeAllTargets() {
        mTargetList.clear();
    };
    virtual std::list<CGDrawInput *>targets() {
        return mTargetList;
    };
    virtual void requestRender() {
        
    };
    
protected:
    CGDrawFramebuffer *mOutputFramebuffer{nullptr};
    std::list<CGDrawInput *>mTargetList;
};

}

#endif /* CGDrawOutput_h */

