// OpenGL ES 2.0 Cookcook code

#ifdef __IPHONE_4_0
#include "RendererEx.h"
#include "Circle.h"
#else
#include "../Scene/RendererEx.h"
#include "../Scene/Circle.h"
#endif

Renderer*       graphicsEngine;   // Graphics Engine
Scene*          scene;            // Scene object
Circle*         circle;
Camera* camera;
bool GraphicsInit()
{
    // Create rendering engine
    graphicsEngine  = new Renderer();

    // Create the scene
    scene     = new Scene("MeshScene", graphicsEngine);
    
    // Create camera and added to the scene
    camera = new Camera("Camera1", scene);
    camera->SetClearBitFieldMask(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    camera->SetPosition(glm::vec3 (0.00000, 0.0, 2.00000));
    camera->SetTarget(glm::vec3 (0.0, 0.0,0.0));
    
    circle = new Circle(scene, NULL, None);
    circle->SetName(std::string("My Circle"));

    scene->addModel(circle);
    graphicsEngine->initializeScenes();
    return true;
}

bool GraphicsResize( int width, int height )
{
    camera->Viewport(0, 0, width, height);
    graphicsEngine->resize(width, height);
    return true;
}

bool GraphicsRender()
{
    //circle->Rotate(1.0, 1.0, 1.0, 1.0);
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
