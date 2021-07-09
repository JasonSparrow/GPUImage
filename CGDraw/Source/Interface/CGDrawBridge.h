//
//  CGDrawBridge.h
//  CGDraw
//
//  Created by UFOTO on 2021/7/9.
//

#ifndef CGDrawBridge_h
#define CGDrawBridge_h

#ifdef __APPLE__

typedef unsigned char *(*getTexture)(void *bridge, long tmp);
typedef struct Bridge_IOS {
    void *bridge;
    getTexture getTexture;
} CallbackCpp;

#endif

#endif /* CGDrawBridge_h */
