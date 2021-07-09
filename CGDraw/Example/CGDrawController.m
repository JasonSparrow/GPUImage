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
{
    NSData *rgba;
}
@property(nonatomic, strong)CGPaintViewOutput *glView;
@property(nonatomic, strong)CGDrawAgent *agent;
@end

@implementation CGDrawController

- (void)viewDidLoad {
    [super viewDidLoad];
    _glView = [[CGPaintViewOutput alloc] initWithFrame:CGRectMake(0, 0, UIScreen.mainScreen.bounds.size.width, UIScreen.mainScreen.bounds.size.width)];
    _glView.center = self.view.center;
    [self.view addSubview:_glView];
    
    NSString *path = [[NSBundle mainBundle] pathForResource:@"rgba8_1125x1125" ofType:@"rgba"];
    rgba = [NSData dataWithContentsOfFile:path];
    
}

- (void)glRequestRender:(int)textureId size:(CGSize)size {
    CGPaintFramebuffer *framebuffer = [[CGPaintFramebuffer alloc] initWithSize:size texture:textureId];
    [_glView newTextureAvailable:framebuffer];
}

- (void)touchesBegan:(NSSet<UITouch *> *)touches withEvent:(UIEvent *)event {
    runSyncOnSerialQueue(^{
        [[CGPaintContext sharedRenderContext] useAsCurrentContext];
        self->_agent = [[CGDrawAgent alloc] init];
        self->_agent.delegate = self;
        [self->_agent setInputData:(UInt8 *)self->rgba.bytes size:CGSizeMake(1125, 1125)];
    });
}
@end
