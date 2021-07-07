//
//  CGDrawProgram.cpp
//  CGDraw
//
//  Created by Jason on 2021/6/11.
//

#include "CGDrawProgram.h"
#include <stdlib.h>
#include "CGShaderUtil.h"

using namespace CGDraw;

bool CGDrawProgram::setWithVertexShader(unsigned char *vShaderString, unsigned char *fShaderString) {
    program = glCreateProgram();
    
    // Create and compile vertex shader
    bool complieOK = compileShader(&vertShader, GL_VERTEX_SHADER, vShaderString);
    if (complieOK == false) {
        printf("Failed to compile vertex shader\n");
    }
    
    // Create and compile vertex shader
    complieOK = compileShader(&fragShader, GL_FRAGMENT_SHADER, fShaderString);
    if (complieOK == false) {
        printf("Failed to compile fragment shader\n");
    }
    
    // Attach Shader to program
    glAttachShader(program, vertShader);
    glAttachShader(program, fragShader);
    
    return complieOK;
}

bool CGDrawProgram::compileShader(GLuint *shader, GLenum type, unsigned char *shaderString) {

    GLint status;
    const GLchar *source;
    if (shaderString == nullptr) {
        printf("Failed to load shader\n");
        return false;
    }
    source = (GLchar *)shaderString;

    *shader = glCreateShader(type);
    glShaderSource(*shader, 1, &source, NULL);
    glCompileShader(*shader);

    glGetShaderiv(*shader, GL_COMPILE_STATUS, &status);

    if (status == GL_TRUE) {
//        NSLog(@"%d: shader compile success", type);
    } else {
        GLint logLength;
        glGetShaderiv(*shader, GL_INFO_LOG_LENGTH, &logLength);
        if (logLength > 0) {
            GLchar *log = (GLchar *)malloc(logLength);
            glGetShaderInfoLog(*shader, logLength, &logLength, log);
            if (shader == &vertShader) {
                printf(" vertex Shader log is : %s\n", log);
            } else {
                printf(" fragment Shader log is : %s\n", log);
            }
            free(log);
        }
    }
    return status == GL_TRUE;
}

void CGDrawProgram::use() {
    glUseProgram(program);
    CGShaderUtil::glCheckError((char *)"use");
}

void CGDrawProgram::unuse() {
    glUseProgram(GL_NONE);
    CGShaderUtil::glCheckError((char *)"unuse");
}

bool CGDrawProgram::link() {
    GLint status;
    glLinkProgram(program);
    glGetProgramiv(program, GL_LINK_STATUS, &status);
    if (status == GL_FALSE) {
        printf("linked failed, 检查着色器书写内容是否有误\n");
        return false;
    }
    if (vertShader) {
        glDeleteShader(vertShader);
        vertShader = 0;
    }
    if (fragShader) {
        glDeleteShader(fragShader);
        fragShader = 0;
    }
    
    CGShaderUtil::glCheckError((char *)"use");
    return true;
}

#pragma mark -
#pragma mark validate
bool CGDrawProgram::validate()
{
    //日志长度
    GLint logLength;
    
    //验证program
    glValidateProgram(program);
    
    //获取日志长度
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLength);
    
    //日志长度>0
    if (logLength > 0) {
        //获取日志信息
        GLchar *log = (GLchar *)malloc(logLength);
        glGetProgramInfoLog(program, logLength, &logLength, log);
//        NSString *programLog = [NSString stringWithFormat:@"%s", log];
//        printf("program validate failed : %@", programLog);
        free(log);
        return false;
    } else {
//        NSLog(@"program validate success");
        return true;
    }
}

CGDrawProgram::CGDrawProgram() {
    
}
CGDrawProgram::~CGDrawProgram() {
    if (vertShader) {
        glDeleteShader(vertShader);
        CGShaderUtil::glCheckError((char *)"CGProgram: glDeleteShader");
    }
    
    if (fragShader) {
        glDeleteShader(fragShader);
        CGShaderUtil::glCheckError((char *)"CGProgram: glDeleteShader");
    }
    
    
    if (program) {
        glDeleteProgram(program);
        CGShaderUtil::glCheckError((char *)"CGProgram: glDeleteProgram");
    }
}

GLint CGDrawProgram::getUniformLocation(unsigned char *name) {
    GLint loc = glGetUniformLocation(program, (const GLchar *)name);
    CGShaderUtil::glCheckError((char *)"getUniformLocation");
    return loc;
}

GLint CGDrawProgram::getAttribLocation(unsigned char *name) {
    GLint loc = glGetAttribLocation(program, (const GLchar *)name);
    CGShaderUtil::glCheckError((char *)"getAttribLocation");
    return loc;
}

#pragma mark -
#pragma mark setter
void CGDrawProgram::setUniform1f(GLint location, GLfloat value) {
    glUniform1f(location, value);
}

