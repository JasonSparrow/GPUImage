//
//  CGDrawTexture.h
//  CGDraw
//
//  Created by Jason on 2021/6/24.
//

#ifndef CGDrawTexture_h
#define CGDrawTexture_h

#include <stdio.h>
#include "CGOpenGL.h"

namespace CGDraw {
class CGDrawTexture {
  
public:
    static GLuint genTexture() {
        GLuint tid;
        glGenTextures(1, &tid);
        glBindTexture(GL_TEXTURE_2D, tid);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glBindTexture(GL_TEXTURE_2D, 0);
        return tid;
    }

    static GLuint genTexture(int w, int h, GLenum f) {
        if(w * h <= 0) {
            return TEXTURE_NULL;
        }
        GLuint tid;
        glGenTextures(1, &tid);
        if (tid > TEXTURE_NULL) {
            glBindTexture(GL_TEXTURE_2D, tid);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexImage2D(GL_TEXTURE_2D, 0, f, w, h, 0, f, GL_UNSIGNED_BYTE, 0);
            glBindTexture(GL_TEXTURE_2D, 0);
        }
        return tid;
    }


    static GLuint genTexture(void *data, int w, int h) {
        GLuint tid;
        glGenTextures(1, (GLuint *) &tid);
        glBindTexture(GL_TEXTURE_2D, (GLuint) tid);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        return tid;
    }
};
}
#endif /* CGDrawTexture_h */
