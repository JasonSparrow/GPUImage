//
//  CGShaderUtil.h
//  CGDraw
//
//  Created by Jason on 2021/6/23.
//
#include <cstring>
#include "CGShaderUtil.h"
#include "CGDrawLog.h"
#include "CGOpenGL.h"
#include <iostream>

int CG_LOG_LEVEL = 4;

using namespace CGDraw;

void CGShaderUtil::setLogLevel(int level) {
    CG_LOG_LEVEL = level;
}
GLuint CGShaderUtil::loadShader(GLenum type, const GLchar *source) {
    GLuint shader = glCreateShader(type);

    if (shader != 0 && source != NULL) {
        GLint length = (GLint)strlen(source);
        glShaderSource(shader, 1, &source, &length);
        glCompileShader(shader);
        GLint compiled;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
        if (compiled == 0) {
            CG_DRAW_LOGE("glError: Could not compile shader %d:\n %s", type, source);
            char temp[512] = {0};
            GLsizei length;
            glGetShaderInfoLog(shader, 512, &length, temp);
            CG_DRAW_LOGE("glError %s", temp);
            glDeleteShader(shader);
            shader = 0;

        }
    }
    return shader;
}

GLuint CGShaderUtil::createProgram(const GLchar *vertexSource, const GLchar *fragmentSource) {
    GLuint vertexShader = loadShader(GL_VERTEX_SHADER, vertexSource);
    glCheckError("loadShader GL_VERTEX_SHADER");

    GLuint pixelShader = loadShader(GL_FRAGMENT_SHADER, fragmentSource);
    glCheckError("loadShader GL_FRAGMENT_SHADER");

    GLuint program = glCreateProgram();
    if (program != 0) {
        glAttachShader(program, vertexShader);
        glCheckError("glAttachShader");
        glAttachShader(program, pixelShader);
        glCheckError("glAttachShader");
        glLinkProgram(program);
        GLint linkStatus;
        glGetProgramiv(program, GL_LINK_STATUS, &linkStatus);
        if (linkStatus != GL_TRUE) {
            CG_DRAW_LOGE("glError: Could not link program: ");
            char temp[512] = {0};
            GLsizei length;
            glGetProgramInfoLog(program, 512, &length, temp);
            CG_DRAW_LOGE("glError %s", temp);
            glDeleteProgram(program);
            program = 0;
        }
        glDeleteShader(vertexShader);
        glDeleteShader(pixelShader);
    }
    return program;
}

GLuint CGShaderUtil::genTexture() {
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

GLuint CGShaderUtil::genTexture(int w, int h, GLenum f) {
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


GLuint CGShaderUtil::genTexture(void *data, int w, int h) {
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

void CGShaderUtil::glCheckError(const char *flag) {
    GLenum error;
    while ((error = glGetError()) != GL_NO_ERROR) {
        switch (error) {
            case GL_NO_ERROR:
//                NSLog(@"GL_NO_ERROR: gl success");
                break;
            case GL_INVALID_ENUM:
//                NSLog(@"GL_INVALID_ENUM: 为枚举参数指定了不可接受的值");
                break;
            case GL_INVALID_VALUE:
//                NSLog(@"GL_INVALID_VALUE: 数字参数超出范围");
                break;
            case GL_INVALID_OPERATION:
//                NSLog(@"GL_INVALID_OPERATION: 当前状态下不允许指定的操作");
                break;
            case GL_INVALID_FRAMEBUFFER_OPERATION:
//                NSLog(@"GL_INVALID_FRAMEBUFFER_OPERATION: 该命令试图渲染到帧缓冲区或从帧缓冲区读取，而当前绑定的帧缓冲区不是帧缓冲区完成状态, 即glCheckFramebufferStatus的返回值不是GL_FRAMEBUFFER_COMPLETE");
                break;
            case GL_OUT_OF_MEMORY:
//                NSLog(@"GL_OUT_OF_MEMORY: 没有足够的内存来执行命令。 在记录此错误之后，GL的状态是未定义的，除了错误标志的状态。");
                break;
            default:
//                NSLog(@"其他gl错误");
                break;
        }
        printf("%s: glError: 0x%X\n", flag, error);
    }
}
