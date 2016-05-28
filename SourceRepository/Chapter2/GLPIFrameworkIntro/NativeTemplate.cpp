// OpenGL ES 2.0 Cookcook code

#include "NativeTemplate.h"
#ifdef __APPLE__
	#include "renderer.h"
#else
	#include "../Scene/renderer.h"
#endif



bool GraphicsInit()
{
	Renderer::Instance().initializeRenderer();
}

bool GraphicsResize( int width, int height )
{
	Renderer::Instance().resize(width, height);
}

bool GraphicsRender()
{
	Renderer::Instance().setUpProjection();
	Renderer::Instance().render();
}


#ifdef __ANDROID__

JNIEXPORT void JNICALL Java_cookbook_gles_GLESNativeLib_init( JNIEnv *env, jobject obj, jstring FilePath )
{
	setenv( "FILESYSTEM", env->GetStringUTFChars( FilePath, NULL ), 1 );
	GraphicsInit();
}

JNIEXPORT void JNICALL Java_cookbook_gles_GLESNativeLib_resize( JNIEnv *env, jobject obj, jint width, jint height)
{
	GraphicsResize( width, height );
}

JNIEXPORT void JNICALL Java_cookbook_gles_GLESNativeLib_step(JNIEnv * env, jobject obj)
{
	GraphicsRender();
}
#endif