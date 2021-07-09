//
//  CGDrawEngine.h
//  CGDraw
//
//  Created by UFOTO on 2021/7/9.
//

#ifndef CGDrawEngine_h
#define CGDrawEngine_h

#ifdef __APPLE__

//cpp
#ifdef __cplusplus
extern "C" {
#endif

//C symbol declarations
#include "CGDrawBridge.h" //这个是引入定义的结构体
long registerEngine();
void setInputData(long handle, char *data);
void register_callback(long handle, CallbackCpp *callback);
void getRenderTexture(long handle);
//C symbol declarations

#ifdef __cplusplus
}
#endif
//cpp

#endif

#endif /* CGDrawEngine_h */
