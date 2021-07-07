//
//  CGDrawFramebufferCache.h
//  CGDraw
//
//  Created by Jason on 2021/6/11.
//

#ifndef CGDrawFramebufferCache_h
#define CGDrawFramebufferCache_h

#include <stdio.h>
#include "CGDrawFramebuffer.h"
#include "CGVec.h"
#include <pthread/pthread.h>
#include <map>
#include <iostream>
#include <list>

namespace CGDraw {

class CGDrawFramebufferCache {
    
public:
//    static pthread_mutex_t m_mutex;

    static CGDrawFramebufferCache* shareCache() {
//        pthread_mutex_lock(&m_mutex);
        static CGDrawFramebufferCache instance;
//        pthread_mutex_unlock(&m_mutex);
        return &instance;
    }
    
    void glInit();
    
    void glUnInit();
    
    void destroy();
    
    CGDrawFramebufferCache() {
//        pthread_mutex_init(&m_mutex, NULL);
    };
    ~CGDrawFramebufferCache() {
        
    };

private:

   
    
public:
    CGDrawFramebuffer *getFramebufferForSize(CGVec2f size, bool onlyTexture);
    
    CGDrawFramebuffer *getFramebufferForSize(CGVec2f size, CGTextureOptions textureOptions, bool onlyTexture);

    void recycleFramebufferToCache(CGDrawFramebuffer *framebuffer);
    
    void deleteAllUnassignedFramebuffers();
    
private:
  
    std::list<CGDrawFramebuffer *>mTargetList;

    std::map<std::string, std::list<CGDrawFramebuffer *>> mFramebufferCache;


};
}


#endif /* CGDrawFramebufferCache_h */
