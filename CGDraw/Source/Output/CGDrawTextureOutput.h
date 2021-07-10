//
//  CGDrawTextureOutput.h
//  CGDraw
//
//  Created by Jason on 2021/6/11.
//

#ifndef CGDrawTextureOutput_h
#define CGDrawTextureOutput_h

#include <stdio.h>
#include "CGDrawInput.h"

namespace CGDraw {
class CGDrawTextureOutput:public CGDrawInput {
    
public:
    CGDrawTextureOutput() {
        
    }
    
    ~CGDrawTextureOutput() {
        
    }
    
    int getTexId() {
        return mFramebuffer->getTexture();
    }
    
    int getTexWidth() {
        return mFramebuffer->getFboSize().x;
    }

    int getTexHeight() {
        return mFramebuffer->getFboSize().y;
    }
    
public:
    void newFramebufferAvailable(CGDrawFramebuffer *inFramebuffer) override {
        mFramebuffer = inFramebuffer;
    };
private:
    CGDrawFramebuffer *mFramebuffer{nullptr};
};

}
#endif /* CGDrawTextureOutput_h */
