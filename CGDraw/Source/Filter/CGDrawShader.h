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
    vec4 mask = texture2D(uTexture, varyTextCoord);
    float v = (mask.r + mask.g + mask.b) / 3.0;
    if (varyTextCoord.x > 0.5) {
        gl_FragColor = vec4(vec3(v), mask.a);
    } else {
        gl_FragColor = mask;
    }
 }
);


#endif /* CGDrawShader_h */
