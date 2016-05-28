//
//  NativeTemplate.h
//  iOSBlueTriangle
//
//  Created by Parminder Singh on 6/04/14.
//  Email: parminder_ce@yahoo.co.in
//  Copyright (c) 2013 macbook. All rights reserved.
//

#ifndef iOSBlueTriangle_NativeTemplate_h
#define iOSBlueTriangle_NativeTemplate_h


//! OpenGL ES 3.0 Cookcook code
#include "glutils.h"

#ifdef __ANDROID__
//! JNI function declarations
extern "C" {
    JNIEXPORT void JNICALL Java_cookbook_gles_GLESNativeLib_init(JNIEnv * env, jobject obj, jstring FilePath);
    JNIEXPORT void JNICALL Java_cookbook_gles_GLESNativeLib_resize(JNIEnv * env, jobject obj,  jint width, jint height);
    JNIEXPORT void JNICALL Java_cookbook_gles_GLESNativeLib_step(JNIEnv * env, jobject obj);
};
#endif // End of __ANDROID__ scope

bool GraphicsInit();
bool GraphicsResize( int width, int height );
bool GraphicsRender();

#endif
