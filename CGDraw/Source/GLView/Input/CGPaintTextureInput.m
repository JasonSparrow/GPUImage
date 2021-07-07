//
//  CGPaintTextureInput.m
//  CGPaint
//
//  Created by CGPaint on 2021/5/13.
//  Copyright © 2021 CGPaint. All rights reserved.
//

#import "CGPaintTextureInput.h"
#import "CGPaintFramebufferCache.h"

@implementation CGPaintTextureInput

- (instancetype)initWithTexture:(GLuint)newInputTexture size:(CGSize)newTextureSize {
    self = [super init];
    if (self) {
        _outputFramebuffer = [[CGPaintFramebuffer alloc] initWithSize:newTextureSize texture:newInputTexture];
    }

    return self;
}

- (void)requestRender {
    runSyncOnSerialQueue(^{
        [[CGPaintContext sharedRenderContext] useAsCurrentContext];
        for (id<CGPaintInput> currentTarget in self->_targets){
            [currentTarget newTextureAvailable:self->_outputFramebuffer];
        }
    });
}
@end
