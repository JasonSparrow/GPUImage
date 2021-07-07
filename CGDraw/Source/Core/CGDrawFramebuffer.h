//
//  CGDrawFramebuffer.h
//  CGDraw
//
//  Created by Jason on 2021/6/11.
//

#ifndef CGDrawFramebuffer_h
#define CGDrawFramebuffer_h

#include <stdio.h>
#include "CGOpenGL.h"
#include "CGVec.h"
#include <iostream>
#include "CGShaderUtil.h"

typedef struct _CGTextureOptions {
    GLenum minFilter;
    GLenum magFilter;
    GLenum wrapS;
    GLenum wrapT;
    GLenum internalFormat;
    GLenum format;
    GLenum type;
} CGTextureOptions;

namespace CGDraw {
class CGDrawFramebuffer {
    
public:
    
    CGDrawFramebuffer() {
        printf("CGDrawFramebuffer init\n");
    }
    ~CGDrawFramebuffer() {
        printf("CGDrawFramebuffer unInit\n");
    }

public:
    CGDraw::CGVec2f getFboSize() {
        return mFboSize;
    };
    GLuint getTexture() {
        return mTexture;
    };
    bool getOnlyGenTexture() {
        return mOnlyGenTexture;
    };
    std::string getHashKey() {
        return "";
    };
    
    void setActivity(bool isActivity) {
        mIsActivity = isActivity;
    }
    CGTextureOptions getTextureOptions() {
        return mTextureOptions;
    };
        
public:
    
    void glInit();
    void glUnInit();
    void destroy();
    
    void genWithSize(CGVec2f fboSize, bool onlyTexture);
    
    void genWithSize(CGVec2f fboSize, CGTextureOptions textureOptions, bool onlyTexture);
    
    void genWithSize(CGVec2f fboSize, GLuint texture);

    inline void bindFramebuffer() {
        glBindFramebuffer(GL_FRAMEBUFFER, mFramebuffer);
        CGShaderUtil::glCheckError("bindFramebuffer");
    };

    inline void unbindFramebuffer() {
        glBindFramebuffer(GL_FRAMEBUFFER, GL_NONE);
        CGShaderUtil::glCheckError("unbindFramebuffer");
    };

    inline void bindTexture() {
        glBindTexture(GL_TEXTURE_2D, mTexture);
        CGShaderUtil::glCheckError("bindTexture");
    };

    inline void unbindTexture() {
        glBindTexture(GL_TEXTURE_2D, GL_NONE);
        CGShaderUtil::glCheckError("unbindTexture");
    };
    
    void upload(const unsigned char *data, CGVec2f size, GLenum internalformat, GLenum format, bool isOverride);
    
    void recycle();
        
    static CGTextureOptions defaultTextureOption() {
        CGTextureOptions defaultTextureOptions;
        defaultTextureOptions.minFilter = GL_LINEAR;
        defaultTextureOptions.magFilter = GL_LINEAR;
        defaultTextureOptions.wrapS = GL_CLAMP_TO_EDGE;
        defaultTextureOptions.wrapT = GL_CLAMP_TO_EDGE;
        defaultTextureOptions.internalFormat = GL_RGBA;
        defaultTextureOptions.format = GL_BGRA;
        defaultTextureOptions.type = GL_UNSIGNED_BYTE;
        return defaultTextureOptions;
    };
    
private:
    CGVec2f mFboSize;
    GLTex mTexture;
    bool mOnlyGenTexture;
    
    GLuint           mFramebuffer;
    CGTextureOptions mTextureOptions;
    bool mIsActivity;
    
private:
    
    void generateTexture();
    void generateFramebuffer();
};
}

#endif /* CGDrawFramebuffer_h */
