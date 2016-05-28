//
//  NativeTemplate.h
//  iOSBlueTriangle
//
//  Created by Parminder Singh on 8/29/13.
//  Email: parminder_ce@yahoo.co.in
//  Copyright (c) 2013 macbook. All rights reserved.
//

#ifndef iOSBlueTriangle_NativeTemplate_h
#define iOSBlueTriangle_NativeTemplate_h


//! OpenGL ES 3.0 Cookcook code

#include <stdio.h>
#include <stdlib.h>

#ifdef __ANDROID__
//! Android Header files for NDK and OpenGLES 3.0
#include <jni.h>
#include <android/log.h>
#include <GLES3/gl3.h>
#include <GLES3/gl3ext.h>

//! JNI Information and Error Log
#define  LOG_TAG    "glOpenGLES3Native"
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO, LOG_TAG,__VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)


//! JNI function declarations
extern "C" {
    JNIEXPORT void JNICALL Java_cookbook_gles_GLESNativeLib_init(JNIEnv * env, jobject obj, jstring FilePath);
    JNIEXPORT void JNICALL Java_cookbook_gles_GLESNativeLib_resize(JNIEnv * env, jobject obj, jint width, jint height);
    JNIEXPORT void JNICALL Java_cookbook_gles_GLESNativeLib_step(JNIEnv * env, jobject obj);
};
#endif // End of __ANDROID__ scope

//! Load the Shader for compilation and linking
GLuint loadAndCompileShader(GLenum shaderType, const char* sourceCode);

//! Create program object and link program.
GLuint linkShader(GLuint vertShaderID, GLuint fragShaderID);

//! Create vertex and fragment shader executable program.
GLuint createProgramExec(const char* vertexSource, const char* fragmentSource);
    
//! Print information or error message.
void printOpenGLESInfo();

//! Initialize the shader.
bool GraphicsInit();

//! Set the Viewport with provide width and height.
bool GraphicsResize( int width, int height );

//! Render the triangle geometry.
bool GraphicsRender();

#endif
