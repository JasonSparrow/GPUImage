//
//  CGDrawDemo.m
//  CGDraw
//
//  Created by Jason on 2021/6/11.
//

#import "CGDrawAgent.h"
#include "CGDrawFramebuffer.h"
#include "CGDrawFilter.h"
#include "CGDrawDataInput.h"
#include "CGDrawTextureOutput.h"
#include "CGDrawDataInput.h"

using namespace CGDraw;

@interface CGDrawAgent ()
{
    CGDrawDataInput *_dataInput;
    CGDrawFilter *_filter;
    CGDrawTextureOutput *_output;
    CGDrawFramebuffer *_framebuffer;
}
@end

@implementation CGDrawAgent

- (instancetype)init
{
    self = [super init];
    if (self) {
        _dataInput = new CGDrawDataInput();
        _filter = new CGDrawFilter();
        _output = new CGDrawTextureOutput();
        _framebuffer = new CGDrawFramebuffer();
    }
    return self;
}

- (void)setInputData:(UInt8 *)data size:(CGSize)size {
    
    _dataInput->setInputData(data, size.width, size.height);
    _dataInput->addTarget(_filter);
    _filter->addTarget(_output);
    _dataInput->requestRender();
    int texId = _output->getTexId();
    CGSize textSize = CGSizeMake(_output->getTexWidth(), _output->getTexHeight());
    
    if (self.delegate && [self.delegate respondsToSelector:@selector(glRequestRender:size:)]) {
        [self.delegate glRequestRender:texId size:textSize];
    }
}

@end
