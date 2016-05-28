// OpenGL ES 2.0 Cookcook code

#ifdef __IPHONE_4_0
#include "RendererEx.h"
#include "ObjLoader.h"
#include "SimpleTexture.h"
#include "CustomModel.h"
#include "CustomScene.h"
#else
#include "../Scene/RendererEx.h"
#include "../Scene/ObjLoader.h"
#include "../Scene/SimpleTexture.h"
#include "../Scene/CustomModel.h"
#include "../Scene/CustomScene.h"
#endif

Renderer*       graphicsEngine;         // Graphics Engine
Scene*          customScene;            // CustomScene object
Scene*          hudScene;               // HUD Scene
Light*          globalLight;            // Global Light
SimpleTexture*  buttonUp;               // Button
Camera*         hudCamera;              // HUD Camera
Model*          customModel;            // CustomModel object

bool GraphicsInit()
{
    graphicsEngine  = new Renderer();
    customScene     = new CustomScene("RenderToTexScene", graphicsEngine);
    
    // Set the lights
    globalLight     = new Light(Material(MaterialWhite), glm::vec4(0.00001, 2.0, 25.0, 1.0));
    customScene->addLight(globalLight);
    
    // Camera's are added in the CustomScene's constructor
    customModel     = new CustomModel( customScene, NULL, None );
    customScene->addModel(customModel);
    graphicsEngine->initializeScenes();

    hudScene        = new Scene("ButtonScene");
    hudCamera       = new CameraHUD("hudCamera", hudScene);
    // Don't forget to specify the ViewPort size of the camera in the Resize function.
    
    buttonUp        = new SimpleTexture(hudScene, NULL, None, NULL, NULL);
    buttonUp->SetName(std::string("Depth Texture Viewer"));
    
    hudScene->addModel( buttonUp );
    
    graphicsEngine->addScene(hudScene);
    graphicsEngine->initializeScenes();
    
    return true;
}

bool GraphicsResize( int width, int height )
{
    graphicsEngine->resize(width, height);
    hudCamera->Viewport(0, 0, width, height);
    return true;
}

bool GraphicsRender()
{
    customModel->Rotate(1.0, 0.0, 1.0, 0.0);
    graphicsEngine->render();
    return true;
}

void TouchEventDown( float x, float y )
{
    graphicsEngine->TouchEventDown( x, y );
}

void TouchEventMove( float x, float y )
{
    graphicsEngine->TouchEventMove( x, y );
}

void TouchEventRelease( float x, float y )
{
    graphicsEngine->TouchEventRelease( x, y );
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
