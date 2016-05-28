//
//  NativeTemplate.h
//  iOSBlueTriangle
//
//  Created by macbook on 8/29/13.
//  Copyright (c) 2013 macbook. All rights reserved.
//

#ifndef iOSBlueTriangle_NativeTemplate_h
#define iOSBlueTriangle_NativeTemplate_h

//! OpenGL ES 3.0 Cookcook code
#include "glutils.h"

bool GraphicsInit();
bool GraphicsResize( int width, int height );
bool GraphicsRender();
void TouchEventDown( float x, float y );
void TouchEventMove( float x, float y );
void TouchEventRelease( float x, float y );

#ifdef __ANDROID__
extern "C" {
    JNIEXPORT void JNICALL Java_cookbook_gles_GLESNativeLib_init(JNIEnv * env, jobject obj, jstring FilePath);
    JNIEXPORT void JNICALL Java_cookbook_gles_GLESNativeLib_resize(JNIEnv * env, jobject obj, jint width, jint height);
    JNIEXPORT void JNICALL Java_cookbook_gles_GLESNativeLib_step(JNIEnv * env, jobject obj);
	JNIEXPORT void JNICALL Java_cookbook_gles_GLESNativeLib_TouchEventStart(JNIEnv * env, jobject obj, float x, float y );
	JNIEXPORT void JNICALL Java_cookbook_gles_GLESNativeLib_TouchEventMove(JNIEnv * env, jobject obj, float x, float y );
	JNIEXPORT void JNICALL Java_cookbook_gles_GLESNativeLib_TouchEventRelease(JNIEnv * env, jobject obj, float x, float y );

};
#endif

#endif
