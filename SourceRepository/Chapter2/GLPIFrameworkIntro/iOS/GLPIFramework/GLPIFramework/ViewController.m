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
    
    [self setupGL];
}

- (void) shouldAutorotate
{
    [EAGLContext setCurrentContext:self.context];

    GraphicsResize(self.view.bounds.size.width, self.view.bounds.size.height);
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

- (void)glkView:(GLKView *)view drawInRect:(CGRect)rect
{
    GraphicsRender();
}


@end
