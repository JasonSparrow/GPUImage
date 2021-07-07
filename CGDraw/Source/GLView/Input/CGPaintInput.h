//
//  CGPaintInput.h
//  CGPaint
//
//  Created by CGPaint on 2021/5/13.
//  Copyright © 2021 CGPaint. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <CoreMedia/CoreMedia.h>
#import "CGPaintFramebuffer.h"
#import "CGPaintUtils.h"

NS_ASSUME_NONNULL_BEGIN
//处理事件的发起者都是输入源
@protocol CGPaintInput <NSObject>
@required

- (void)newTextureAvailable:(CGPaintFramebuffer *)framebuffer;

@end

NS_ASSUME_NONNULL_END
