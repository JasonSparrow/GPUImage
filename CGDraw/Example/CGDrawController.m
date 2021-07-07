//
//  CGDrawController.m
//  CGPaintC
//
//  Created by Jason on 2021/6/11.
//

#import "CGDrawController.h"
#import "CGPaintViewOutput.h"
#import "CGDrawAgent.h"

@interface CGDrawController ()<CGDrawDelegate>

@property(nonatomic, strong)CGPaintViewOutput *glView;
@property(nonatomic, strong)CGDrawAgent *agent;
@end

@implementation CGDrawController

- (void)viewDidLoad {
    [super viewDidLoad];
    _glView = [[CGPaintViewOutput alloc] initWithFrame:CGRectMake(0, 0, UIScreen.mainScreen.bounds.size.width, UIScreen.mainScreen.bounds.size.height)];
    [self.view addSubview:_glView];
    
    NSString *path = [[NSBundle mainBundle] pathForResource:@"rgba8_1125x1125" ofType:@"rgba"];
    NSData *rgba = [NSData dataWithContentsOfFile:path];
    
    _agent = [[CGDrawAgent alloc] init];
    _agent.delegate = self;
    [_agent setInputData:(UInt8 *)rgba.bytes size:CGSizeMake(1125, 1125)];
    
}

- (void)glRequestRender:(int)textureId size:(CGSize)size {
    CGPaintFramebuffer *framebuffer = [[CGPaintFramebuffer alloc] initWithSize:size texture:textureId];
    [_glView newTextureAvailable:framebuffer];
}

@end
