//
//  CGDrawProgram.h
//  CGDraw
//
//  Created by Jason on 2021/6/11.
//

#ifndef CGDrawProgram_h
#define CGDrawProgram_h

#include <stdio.h>
#include <OpenGLES/ES2/gl.h>
#include <OpenGLES/ES2/glext.h>

namespace CGDraw {

class CGDrawProgram {
    
public:
    CGDrawProgram();
    ~CGDrawProgram();
    
    bool setWithVertexShader(unsigned char *vShaderString, unsigned char *fShaderString);

    void use();

    void unuse();

    bool link();

    bool validate();

    #pragma mark -
    GLint getUniformLocation(unsigned char *name);
    GLint getAttribLocation(unsigned char *name);
    void setUniform1f(GLint location, GLfloat value);
    
protected:
    GLuint          program;
    GLuint          vertShader;
    GLuint          fragShader;
private:
    bool compileShader(GLuint *shader, GLenum type, unsigned char *shaderString);

};

};
#endif /* CGDrawProgram_h */
