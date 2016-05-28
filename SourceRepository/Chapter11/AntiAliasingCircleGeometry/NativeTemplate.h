//
//  NativeTemplate.h
//  iOSBlueTriangle
//
//  Created by macbook on 8/29/13.
//  Copyright (c) 2013 macbook. All rights reserved.
//

#ifndef iOSBlueTriangle_NativeTemplate_h
#define iOSBlueTriangle_NativeTemplate_h


// OpenGL ES 3.0 Cookcook code

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#ifdef __IPHONE_4_0
    #include <OpenGLES/ES2/gl.h>
    #include <OpenGLES/ES2/glext.h>
    #define LOGI printf
    #define LOGE printf
#else
    #include <jni.h>
    #include <android/log.h>

    #include <GLES2/gl2.h>
    #include <GLES2/gl2ext.h>

	#define  LOG_TAG    "libgl2jni"
    #define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
    #define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)
#endif

GLuint loadShader(GLenum shaderType, const char* pSource);
GLuint createProgram(const char* pVertexSource, const char* pFragmentSource);
bool setupGraphics(int w, int h);
void renderFrame() ;
///////////////////////////////////
bool GraphicsInit();
bool GraphicsResize( int width, int height );
bool GraphicsRender();
void TouchEventDown( float x, float y );
void TouchEventMove( float x, float y );
void TouchEventRelease( float x, float y );
///////////////////////////////////
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
