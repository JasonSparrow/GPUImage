//
//  CGShaderUtil.h
//  CGDraw
//
//  Created by Jason on 2021/6/23.
//
#ifndef CAMERASDK_SHADER_UTIL_H
#define CAMERASDK_SHADER_UTIL_H

#include <stdio.h>
#include "CGOpenGL.h"

namespace CGDraw {

class CGShaderUtil {

public:
    static void setLogLevel(int level);
    
    static GLuint createProgram(const GLchar *vertexSource, const GLchar *fragmentSource);

    static GLuint genTexture();

    static GLuint genTexture(int w, int h, GLenum f = GL_RGBA);

    static GLuint genTexture(void *data, int w, int h);
    
    static void glCheckError(const char *flag);
private:
    static GLuint loadShader(GLenum type, const GLchar *);
};

}

#endif //CAMERASDK_SHADER_UTIL_H
