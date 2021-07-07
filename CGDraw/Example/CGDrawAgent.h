//
//  CGDrawDemo.h
//  CGDraw
//
//  Created by Jason on 2021/6/11.
//

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>

NS_ASSUME_NONNULL_BEGIN

@protocol CGDrawDelegate <NSObject>

- (void)glRequestRender:(int)textureId size:(CGSize)size;

@end

@interface CGDrawAgent : NSObject

@property(nonatomic, weak)id<CGDrawDelegate>delegate;

- (void)setInputData:(UInt8 *)data size:(CGSize)size;

@end

NS_ASSUME_NONNULL_END
