//
//  CGDrawFilter.cpp
//  CGDraw
//
//  Created by Jason on 2021/6/11.
//

#include "CGDrawFilter.h"
#include "CGDrawShader.h"
#include "CGDrawProgram.h"
#include "CGShaderUtil.h"
#include "CGOpenGL.h"
#include "CGDrawFramebufferCache.h"

using namespace CGDraw;

static const GLfloat imageVertices[] = {
    -1.0f, -1.0f,
    1.0f, -1.0f,
    -1.0f,  1.0f,
    1.0f,  1.0f,
};

static const GLfloat textureCoordinates[] = {
    0.0f, 0.0f,
    1.0f, 0.0f,
    0.0f, 1.0f,
    1.0f, 1.0f,
};

CGDrawFilter::CGDrawFilter() {

}

CGDrawFilter::~CGDrawFilter() {
    
}

void CGDrawFilter::setInputVertexShader(unsigned char *vShader, unsigned char *fShader) {
    CGDrawProgram *program = new CGDrawProgram();
    program->setWithVertexShader(vShader, fShader);
    CGShaderUtil::glCheckError("CGDrawFilter1");
    
    if (program && program->link()) {
        mPosition = program->getAttribLocation((unsigned char *)ATTR_POSITION);
        mTexCoord = program->getAttribLocation((unsigned char *)ATTR_TEXCOORD);
        uTexture = program->getUniformLocation((unsigned char *)UNIF_TEXTURE);
        glEnableVertexAttribArray(mPosition);
        glEnableVertexAttribArray(mTexCoord);
    }
    
    CGShaderUtil::glCheckError("CGDrawFilter2");
    mShaderProgram = program;
}

void CGDrawFilter::newFramebufferAvailable(CGDrawFramebuffer *inFramebuffer) {
    mInputFramebuffer = inFramebuffer;
    //1.处理自己的滤镜
    renderToTextureWithVertices(imageVertices, textureCoordinates);
    //2.通知自己的下一个节点处理滤镜
    notifyNextTargetsAboutNewFrame();
}

void CGDrawFilter::renderToTextureWithVertices(const GLfloat *vertices, const GLfloat *textureCoordinates) {
    mOutputFramebuffer = CGDrawFramebufferCache::shareCache()->getFramebufferForSize(mInputFramebuffer->getFboSize(), false);
    glViewport(0, 0, mInputFramebuffer->getFboSize().x, mInputFramebuffer->getFboSize().y);
    mOutputFramebuffer->bindFramebuffer();
    mOutputFramebuffer->bindTexture();
    bool valid = mShaderProgram->validate();
    if (valid == false) {
        printf("验证失败\n");
    }
    
    mShaderProgram->use();
    glClearColor(1.f, 1.f, 1.f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT);
    
    glActiveTexture(CG_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, mInputFramebuffer->getTexture());
    glUniform1i(uTexture, CG_TEXTURE0);
    
    glEnableVertexAttribArray(mPosition);
    glVertexAttribPointer(mPosition, 2, GL_FLOAT, 0, 0, vertices);
    glEnableVertexAttribArray(mTexCoord);
    glVertexAttribPointer(mTexCoord, 2, GL_FLOAT, 0, 0, textureCoordinates);
    
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glDisableVertexAttribArray(mPosition);
    glDisableVertexAttribArray(mTexCoord);
    
    mOutputFramebuffer->unbindTexture();
    mOutputFramebuffer->unbindTexture();
    mShaderProgram->unuse();
    
    if (mInputFramebuffer->getOnlyGenTexture() == false) {
        mInputFramebuffer->recycle();
    }
}

void CGDrawFilter::notifyNextTargetsAboutNewFrame() {
    std::list<CGDrawInput *>::iterator iter;
    for(iter = mTargetList.begin(); iter != mTargetList.end() ;iter++) {
        CGDrawInput *target = *iter;
        target->newFramebufferAvailable(mOutputFramebuffer);
    }
}
