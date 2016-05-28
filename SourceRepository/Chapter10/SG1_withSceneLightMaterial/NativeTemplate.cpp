// OpenGL ES 2.0 Cookcook code

#include "NativeTemplate.h"
#ifdef __IPHONE_4_0
    #include "RendererEx.h"
    #include "ObjLoader.h"
//    #include "Button.h"
#else
    #include "../Scene/RendererEx.h"
    #include "../Scene/ObjLoader.h"
    //#include "../Scene/Button.h"
#endif

// Icons used from
// http://neurovit.deviantart.com/art/simplicio-92311415

Renderer* graphicsEngine;
ObjLoader* Suzzane;
Scene* scene1;

bool GraphicsInit()
{
    // Create new renderer object
    graphicsEngine = new Renderer();

    // Create new Scene with name "Mesh Scene".
    // Add the scene into renderer.
    scene1 = new Scene("MeshScene", graphicsEngine);
    
    //Create a new light and set into the scene
    scene1->addLight(new Light(Material(MaterialWhite),glm::vec4(0.0, 0.0, 10.0, 1.0)));
    
    // Create Suzzane and added into the create scene.
    Suzzane =  new ObjLoader    ( scene1, NULL, SUZZANE, None );
    
    // Set the copper type material on Suzzane.
    Suzzane->SetMaterial(Material(MaterialCopper));
    
    // Add Suzzane into the Scene
    scene1->addModel( Suzzane);
    
    // Intialize the engine
    graphicsEngine->initializeScenes();
    return true;
}


bool GraphicsResize( int width, int height )
{
	graphicsEngine->resize(width, height);
    return true;
}

bool GraphicsRender()
{
    static int i = 0;
    static clock_t start = clock();
    if(clock()-start > CLOCKS_PER_SEC){
        start = clock();
        (i %=6)++; //Plus one to avoid none type
        Suzzane->SetMaterial(Material(MaterialType(i)));
    }
	graphicsEngine->render();
    return true;
}

void TouchEventDown( float x, float y )
{
	//graphicsEngine->TouchEventDown( x, y );
}

void TouchEventMove( float x, float y )
{
	//graphicsEngine->TouchEventMove( x, y );
}

void TouchEventRelease( float x, float y )
{
	//graphicsEngine->TouchEventRelease( x, y );
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
    //renderFrame();
}

JNIEXPORT void JNICALL Java_cookbook_gles_GLESNativeLib_TouchEventStart(JNIEnv * env, jobject obj, float x, float y )
{
	TouchEventDown(x ,y);
}

JNIEXPORT void JNICALL Java_cookbook_gles_GLESNativeLib_TouchEventMove(JNIEnv * env, jobject obj, float x, float y )
{
	TouchEventMove(x ,y);
}

JNIEXPORT void JNICALL Java_cookbook_gles_GLESNativeLib_TouchEventRelease(JNIEnv * env, jobject obj, float x, float y )
{
	TouchEventRelease(x ,y);
}

#endif
