//
//  ViewController.m
//  GLPIFramework
//
//  Created by macbook on 9/8/13.
//  Copyright (c) 2013 macbook. All rights reserved.
//

#import "ViewController.h"
#include "NativeTemplate.h"


@interface ViewController () {
}
@property (strong, nonatomic) EAGLContext *context;

- (void)initializeGL;
- (void)tearDownGL;

@end

@implementation ViewController

- (void)dealloc
{
    [self tearDownGL];
    
    if ([EAGLContext currentContext] == self.context) {
        [EAGLContext setCurrentContext:nil];
    }
    
    [_context release];
    [super dealloc];
}

- (void)viewDidLoad
{
    [super viewDidLoad];
    
    self.context = [[[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES3] autorelease];
    
    if (!self.context) {
        NSLog(@"Failed to create ES context");
    }
    
    GLKView *view = (GLKView *)self.view;
    view.context = self.context;
    view.drawableDepthFormat = GLKViewDrawableDepthFormat24;
    int x = 10;
    
    // Add Six button on Fly
    for (int i=0; i<6; i++)
    {
        UIButton * button = [UIButton buttonWithType:UIButtonTypeRoundedRect];
        x = i*40;
        button.frame = CGRectMake(x,10,30, 30);
        switch (i) {
            case 0:
                [button setTitle:@"L" forState:UIControlStateNormal];
                break;

            case 1:
                [button setTitle:@"R" forState:UIControlStateNormal];
                break;
            
            case 2:
                [button setTitle:@"U" forState:UIControlStateNormal];
                break;
            
            case 3:
                [button setTitle:@"D" forState:UIControlStateNormal];
                break;
            
            case 4:
                [button setTitle:@">>" forState:UIControlStateNormal];
                break;
            
            case 5:
                [button setTitle:@"<<" forState:UIControlStateNormal];
                break;
                
            default:
                break;
        }
        [button setTag:i];
        [ button addTarget:self action:@selector(buttonPressed) forControlEvents:UIControlEventTouchDown];
        // add gesture recognizer to button for longpress
        UILongPressGestureRecognizer *longpressGesture1 = [[UILongPressGestureRecognizer alloc] initWithTarget:self action:@selector(longPressHandler:)];
        longpressGesture1.minimumPressDuration = 1.0;
        [button addGestureRecognizer:longpressGesture1];
        [longpressGesture1 release];
        
        [view addSubview:button];
    }

    [self setupGL];
}

- (void)longPressHandler:(UILongPressGestureRecognizer *)gestureRecognizer {
    NSLog(@"Long press");
}

-(void)buttonPressed{
    LOGI("Button Pressed");
}

- (void) shouldAutorotate
{
    [EAGLContext setCurrentContext:self.context];

    GraphicsResize(self.view.bounds.size.width, self.view.bounds.size.height);
}

- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event
{
	UITouch *touch;
	CGPoint pos;
	
	for( touch in touches )
	{
		pos = [ touch locationInView:self.view ];

		TouchEventDown( pos.x, pos.y );
	}
}

- (void)touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event
{
	UITouch *touch;
	CGPoint pos;
	
	for( touch in touches )
	{
		pos = [ touch locationInView:self.view ];
		TouchEventMove( pos.x, pos.y );
		
	}
}

- (void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event
{
	UITouch *touch;
	CGPoint pos;
	
	for( touch in touches )
	{
		pos = [ touch locationInView:self.view ];
		
		TouchEventRelease( pos.x, pos.y );
	}
}

- (void)viewDidUnload
{
    [super viewDidUnload];
    
    if ([self isViewLoaded] && ([[self view] window] == nil)) {
        self.view = nil;
        
        if ([EAGLContext currentContext] == self.context) {
            [EAGLContext setCurrentContext:nil];
        }
        self.context = nil;
    }
    
    // Dispose of any resources that can be recreated.
}

- (void)setupGL
{
    [EAGLContext setCurrentContext:self.context];
    [((GLKView *) self.view) bindDrawable];
    //Optional code to demonstrate how can you bind frame buffer and render buffer.
    GLint defaultFBO;
    GLint defaultRBO;
    
    glGetIntegerv(GL_FRAMEBUFFER_BINDING, &defaultFBO);
    glGetIntegerv(GL_RENDERBUFFER_BINDING, &defaultRBO);
    
    glBindFramebuffer( GL_FRAMEBUFFER, defaultFBO );
    glBindRenderbuffer( GL_RENDERBUFFER, defaultRBO );

    GraphicsInit();
    
}

- (void)tearDownGL
{
    [EAGLContext setCurrentContext:self.context];
}

- (void)glkView:(GLKView *)view drawInRect:(CGRect)rect
{
    GraphicsRender();
}


@end
