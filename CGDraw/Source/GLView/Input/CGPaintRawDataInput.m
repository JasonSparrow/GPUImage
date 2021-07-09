//
//  CGPaintRawDataInput.m
//  CGPaint
//
//  Created by CGPaint on 2021/5/13.
//  Copyright © 2021 CGPaint. All rights reserved.
//

#import "CGPaintRawDataInput.h"

NSString *const gl_vert = CG_SHADER_STRING (
    attribute vec4 position;
    attribute vec2 aTexCoord;

    varying lowp vec2 varyTextCoord;
    void main() {
      varyTextCoord = aTexCoord;
      gl_Position = position;
    }
);

NSString *const gl_frag_nv21 = CG_SHADER_STRING (
    precision highp float;
    varying vec2 varyTextCoord;
    uniform sampler2D y_texture;
    uniform sampler2D vu_texture;
    void main()
    {
      vec3 yuv;
      yuv.x = texture2D(y_texture, varyTextCoord).r;
      yuv.yz = texture2D(vu_texture, varyTextCoord).ra;

      float y = yuv.x;
      float v = yuv.y - 0.5;
      float u = yuv.z - 0.5;

      float r = y + 1.402 * v;
      float g = y - 0.344 * u - 0.714 * v;
      float b = y + 1.772 * u;
      gl_FragColor = vec4(r, g, b, 1.0);
    }
);

NSString *const gl_frag_nv12 = CG_SHADER_STRING (
    precision highp float;
    varying vec2 varyTextCoord;
    uniform sampler2D y_texture;
    uniform sampler2D vu_texture;
    void main()
    {
      vec3 yuv;
      yuv.x = texture2D(y_texture, varyTextCoord).r;
      yuv.yz = texture2D(vu_texture, varyTextCoord).ra;

      float y = yuv.x;
      float u = yuv.y - 0.5;
      float v = yuv.z - 0.5;

      float r = y + 1.402 * v;
      float g = y - 0.344 * u - 0.714 * v;
      float b = y + 1.772 * u;
      gl_FragColor = vec4(r, g, b, 1.0);
    }
);

NSString *const gl_frag_i420 = CG_SHADER_STRING (
    precision highp float;
    varying vec2 varyTextCoord;
    uniform sampler2D y_texture;
    uniform sampler2D u_texture;
    uniform sampler2D v_texture;

    void main()
    {
      vec3 yuv;
      yuv.x = texture2D(y_texture, varyTextCoord).r;
      yuv.y = texture2D(u_texture, varyTextCoord).r;
      yuv.z = texture2D(v_texture, varyTextCoord).r;

      float y = yuv.x;
      float u = yuv.y - 0.5;
      float v = yuv.z - 0.5;

      float r = y + 1.402 * v;
      float g = y - 0.344 * u - 0.714 * v;
      float b = y + 1.772 * u;
      gl_FragColor = vec4(r, g, b, 1.0);
    }
);

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

@implementation CGPaintRawDataInput
{
    CGDataFormat _format;
    CGSize _byteSize;
    CGPaintProgram *_shaderProgram;
    //顶点属性,纹理属性
    GLint _position, _aTexCoord;
    
    CGPaintFramebuffer *_yFramebuffer;
    CGPaintFramebuffer * _uFramebuffer;
    CGPaintFramebuffer * _vFramebuffer;
    CGPaintFramebuffer * _uvFramebuffer;

    GLuint _yTexUniform;
    GLuint _uTexUniform;
    GLuint _vTexUniform;
    GLuint _uvTexUniform;
    
    BOOL _isOverride;
}
- (instancetype)initWithByte:(UInt8 *)byte byteSize:(CGSize)byteSize format:(CGDataFormat)format {
    self = [super init];
    if (self) {
        _isOverride = NO;
        _format = format;
        _byteSize = byteSize;
        runSyncOnSerialQueue(^{
            [[CGPaintContext sharedRenderContext] useAsCurrentContext];
            if (format == CGDataFormatRGBA || format == CGDataFormatBGRA) {
                self->_outputFramebuffer = [[CGPaintFramebufferCache sharedFramebufferCache] fetchFramebufferForSize:byteSize onlyTexture:YES];
                [self->_outputFramebuffer bindTexture];
                [self uploadByte:byte byteSize:byteSize format:format];
                [self->_outputFramebuffer unbindTexture];
            } else if (format == CGDataFormatNV21 || format == CGDataFormatNV12 || format == CGDataFormatI420) {
                if (format ==CGDataFormatNV21) {
                    self->_shaderProgram = [[CGPaintProgram alloc] initWithVertexShaderString:gl_vert fragmentShaderString:gl_frag_nv21];
                } else if (format == CGDataFormatNV12) {
                    self->_shaderProgram = [[CGPaintProgram alloc] initWithVertexShaderString:gl_vert fragmentShaderString:gl_frag_nv12];
                } else if (format == CGDataFormatI420) {
                    self->_shaderProgram = [[CGPaintProgram alloc] initWithVertexShaderString:gl_vert fragmentShaderString:gl_frag_i420];
                }
                if (self->_shaderProgram && [self->_shaderProgram link]) {
                    self->_position = [self->_shaderProgram getAttribLocation:ATTR_POSITION];
                    self->_aTexCoord = [self->_shaderProgram getAttribLocation:ATTR_TEXCOORD];
                    self->_yTexUniform = [self->_shaderProgram getUniformLocation:@"y_texture"];
                }
                
                self->_outputFramebuffer = [[CGPaintFramebuffer alloc] initWithSize:byteSize onlyTexture:NO];
                self->_yFramebuffer = [[CGPaintFramebufferCache sharedFramebufferCache] fetchFramebufferForSize:byteSize onlyTexture:YES];
                if (format == CGDataFormatNV21 || format == CGDataFormatNV12) {
                    self->_uvTexUniform = [self->_shaderProgram getUniformLocation:@"vu_texture"];
                    self->_uvFramebuffer = [[CGPaintFramebufferCache sharedFramebufferCache] fetchFramebufferForSize:byteSize onlyTexture:YES];
                }else if (format == CGDataFormatI420) {
                    self->_uTexUniform = [self->_shaderProgram getUniformLocation:@"u_texture"];
                    self->_vTexUniform = [self->_shaderProgram getUniformLocation:@"v_texture"];

                    self->_uFramebuffer = [[CGPaintFramebufferCache sharedFramebufferCache] fetchFramebufferForSize:byteSize onlyTexture:YES];
                    self->_vFramebuffer = [[CGPaintFramebufferCache sharedFramebufferCache] fetchFramebufferForSize:byteSize onlyTexture:YES];
                }

                [self uploadByte:byte byteSize:byteSize format:format];
                
                [self->_shaderProgram use];
                [self->_outputFramebuffer bindFramebuffer];
                if (format == CGDataFormatNV21 ||format == CGDataFormatNV12) {
                    [self drawNV21NV12ToFBO];
                } else if (format == CGDataFormatI420) {
                    [self drawI420ToFBO];
                }
                /**
                 The FBO cannot be recycle because in the requestRender method output is treated as driving into the next level target, but the FBO is not the raw data, it is the rendered data, so the effect will be overlaid, and each time the effect is applied not to the raw data, but to the rendered data
                 为什么RGBA的就不会重复绘制, 因为RGBA的是CGPaintFramebuffer是一个纹理类型的, 不进缓存, 但是NV21/NV12是fbo+纹理类型,进缓存会导致这个问题, 所以数据源的所有fbo都不进缓存
                 */
                //[self->_outputFramebuffer recycle];
            }
        });
    }
    _isOverride = YES;
    return self;
}

- (void)uploadByte:(UInt8 *)byte byteSize:(CGSize)byteSize format:(CGDataFormat)format {
    if (format == CGDataFormatRGBA) {
        [self->_outputFramebuffer upload:byte size:byteSize internalformat:GL_RGBA format:GL_RGBA isOverride:_isOverride];
    } else if (format == CGDataFormatBGRA) {
        [self->_outputFramebuffer upload:byte size:byteSize internalformat:GL_RGBA format:GL_BGRA isOverride:_isOverride];
    } else if (format == CGDataFormatNV21 || format == CGDataFormatNV12) {
        int width = byteSize.width;
        int height = byteSize.height;
        int uvOffset = width * height;
        [self->_yFramebuffer bindTexture];
        [self->_yFramebuffer upload:byte size:byteSize internalformat:GL_LUMINANCE format:GL_LUMINANCE isOverride:_isOverride];
        [self->_yFramebuffer unbindTexture];
        
        CGSize uvSize = CGSizeMake(width * 0.5, height * 0.5);
        [self->_uvFramebuffer bindTexture];
        [self->_uvFramebuffer upload:byte + uvOffset size:uvSize internalformat:GL_LUMINANCE_ALPHA format:GL_LUMINANCE_ALPHA isOverride:_isOverride];
        [self->_uvFramebuffer unbindTexture];
        
    } else if (format == CGDataFormatI420) {
        int width = byteSize.width;
        int height = byteSize.height;
        int uOffset = width * height;
        int vOffset = width * height * 5 / 4;
        [self->_yFramebuffer bindTexture];
        [self->_yFramebuffer upload:byte size:byteSize internalformat:GL_LUMINANCE format:GL_LUMINANCE isOverride:_isOverride];
        [self->_yFramebuffer unbindTexture];
        
        CGSize uvSize = CGSizeMake(width * 0.5, height * 0.5);
        [self->_uFramebuffer bindTexture];
        [self->_uFramebuffer upload:byte + uOffset size:uvSize internalformat:GL_LUMINANCE format:GL_LUMINANCE isOverride:_isOverride];
        [self->_uFramebuffer unbindTexture];
        
        [self->_vFramebuffer bindTexture];
        [self->_vFramebuffer upload:byte + vOffset size:uvSize internalformat:GL_LUMINANCE format:GL_LUMINANCE isOverride:_isOverride];
        [self->_vFramebuffer unbindTexture];
    }
}

- (void)drawNV21NV12ToFBO {
    CGSize size = [_outputFramebuffer fboSize];
    glViewport(0, 0, size.width, size.height);
    glClearColor(1, 1, 1, 1);
    glClear(GL_COLOR_BUFFER_BIT);
    
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, _yFramebuffer.texture);
    glUniform1i(_yTexUniform, CG_TEXTURE0);
    
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, _uvFramebuffer.texture);
    glUniform1i(_uvTexUniform, CG_TEXTURE1);
    
    glEnableVertexAttribArray(self->_position);
    glVertexAttribPointer(_position, 2, GL_FLOAT, 0, 0, imageVertices);
    glEnableVertexAttribArray(self->_aTexCoord);
    glVertexAttribPointer(_aTexCoord, 2, GL_FLOAT, 0, 0, textureCoordinates);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

- (void)drawI420ToFBO {
    CGSize size = [_outputFramebuffer fboSize];
    glViewport(0, 0, size.width, size.height);
    glClearColor(1, 1, 1, 1);
    glClear(GL_COLOR_BUFFER_BIT);
    
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, _yFramebuffer.texture);
    glUniform1i(_yTexUniform, CG_TEXTURE0);
    
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, _uFramebuffer.texture);
    glUniform1i(_uTexUniform, CG_TEXTURE1);
    
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, _vFramebuffer.texture);
    glUniform1i(_vTexUniform, CG_TEXTURE2);
    
    glEnableVertexAttribArray(self->_position);
    glVertexAttribPointer(_position, 2, GL_FLOAT, 0, 0, imageVertices);
    glEnableVertexAttribArray(self->_aTexCoord);
    glVertexAttribPointer(_aTexCoord, 2, GL_FLOAT, 0, 0, textureCoordinates);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glDisableVertexAttribArray(self->_position);
    glDisableVertexAttribArray(self->_aTexCoord);
}

- (void)requestRender {
    [super requestRender];
    runSyncOnSerialQueue(^{
        [[CGPaintContext sharedRenderContext] useAsCurrentContext];
        for (id<CGPaintInput> currentTarget in self->_targets){
            [currentTarget newTextureAvailable:self->_outputFramebuffer];
        }
    });
}

- (void)dealloc
{
    runSyncOnSerialQueue(^{
        [[CGPaintContext sharedRenderContext] useAsCurrentContext];
        self->_outputFramebuffer = nil;
    });
}

@end
