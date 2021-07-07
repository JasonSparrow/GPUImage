//
//  CGDrawInput.h
//  CGDraw
//
//  Created by Jason on 2021/6/11.
//

#ifndef CGDrawInput_h
#define CGDrawInput_h

#include <stdio.h>
#include "CGDrawFramebuffer.h"

namespace CGDraw {

class CGDrawInput {
        
public:
    //纯虚函数, 有定义无实现
    virtual void newFramebufferAvailable(CGDrawFramebuffer *inFramebuffer)=0;

};

}
#endif /* CGDrawInput_h */
