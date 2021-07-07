//
//  CGDrawShader.h
//  CGDraw
//
//  Created by Jason on 2021/6/23.
//

#ifndef CGDrawShader_h
#define CGDrawShader_h

#include "CGOpenGL.h"

const char * kCGDrawVertexShaderString = CG_SHADER_SRC
(
 attribute vec4 position;
 attribute vec4 aTexCoord;

 varying vec2 varyTextCoord;

 void main()
 {
    gl_Position = position;
    varyTextCoord = aTexCoord.xy;
 }
 );

const char * kCGDrawFragmentShaderString = CG_SHADER_SRC
(
 precision highp float;
 varying vec2 varyTextCoord;

 uniform sampler2D uTexture;
 uniform float a;
 void main()
 {
     gl_FragColor = texture2D(uTexture, varyTextCoord);
 }
);


#endif /* CGDrawShader_h */
