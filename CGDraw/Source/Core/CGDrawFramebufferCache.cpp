//
//  CGDrawFramebufferCache.cpp
//  CGDraw
//
//  Created by Jason on 2021/6/11.
//

#include "CGDrawFramebufferCache.h"

using namespace CGDraw;
using namespace std;
void CGDrawFramebufferCache::glInit() {
    
}

void CGDrawFramebufferCache::glUnInit() {
    
}

void CGDrawFramebufferCache::destroy() {
    
}

CGDrawFramebuffer * CGDrawFramebufferCache::getFramebufferForSize(CGDraw::CGVec2f size, bool onlyTexture) {
    CGDrawFramebuffer *buffer = CGDrawFramebufferCache::getFramebufferForSize(size, CGDrawFramebuffer::defaultTextureOption(), onlyTexture);
    return buffer;
}

CGDrawFramebuffer * CGDrawFramebufferCache::getFramebufferForSize(CGDraw::CGVec2f size, CGTextureOptions textureOptions, bool onlyTexture) {
    CGDrawFramebuffer *buffer = new CGDrawFramebuffer();
    buffer->genWithSize(size, textureOptions, onlyTexture);
    return buffer;
}

void CGDrawFramebufferCache::recycleFramebufferToCache(CGDraw::CGDrawFramebuffer *framebuffer) {
    map<string, list<CGDrawFramebuffer *>>::iterator itor = mFramebufferCache.find(framebuffer->getHashKey());
    if (itor != mFramebufferCache.end()) {
        pair<string, list<CGDrawFramebuffer *>> fbCache = *itor;
        list<CGDrawFramebuffer *> fbList = fbCache.second;
        fbList.push_back(framebuffer);
        framebuffer->setActivity(true);
    }
    
}

void CGDrawFramebufferCache::deleteAllUnassignedFramebuffers() {
    mFramebufferCache.clear();
}


