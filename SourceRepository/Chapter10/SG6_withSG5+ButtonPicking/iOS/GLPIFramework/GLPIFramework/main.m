//
//  main.m
//  GLPIFramework
//
//  Created by macbook on 9/8/13.
//  Copyright (c) 2013 macbook. All rights reserved.
//

#import <UIKit/UIKit.h>

#import "AppDelegate.h"

int main(int argc, char *argv[])
{
    setenv( "FILESYSTEM", argv[ 0 ], 1 );
    @autoreleasepool {
        return UIApplicationMain(argc, argv, nil, NSStringFromClass([AppDelegate class]));
    }
}
