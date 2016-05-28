// OpenGL ES 2.0 Cookcook code

#include "NativeTemplate.h"
#ifdef __IPHONE_4_0
    #include "RendererEx.h"
    #include "ObjLoader.h"
#else
    #include "../Scene/RendererEx.h"
    #include "../Scene/ObjLoader.h"
#endif

// Icons used from
// http://neurovit.deviantart.com/art/simplicio-92311415

Renderer* graphicsEngine;
ObjLoader* Suzzane;
ObjLoader* Torus;
Scene* scene1;

bool GraphicsInit()
{
    graphicsEngine = new Renderer();
    scene1 = new Scene("MeshScene", graphicsEngine);
    scene1->addLight(new Light(Material(MaterialWhite),glm::vec4(0.0, 0.0, 10.0, 1.0)));
    Suzzane =  new ObjLoader	( scene1, NULL, SUZZANE, None );
    Torus   =  new ObjLoader	( scene1, NULL, TORUS, None );
    Suzzane->SetMaterial(Material(MaterialCopper));
    Torus->SetMaterial(Material(MaterialGold));
    Torus->Scale(0.40, 0.40, 0.4);
    scene1->addModel( Suzzane );
    scene1->addModel( Torus );
    Suzzane->SetCenter(glm::vec3 (-3.0, 0.0, 0.0));
    Suzzane->Translate(3.0, 0.0, 0.0);

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

    Suzzane->Rotate(1.0, 0.0, 1.0, 0.0);     // Relative
    Suzzane->RotateLocal(3.0, 0.0, 1.0, 0.0);// Local
	graphicsEngine->render();
    return true;
}

void TouchEventDown( float x, float y )
{
}

void TouchEventMove( float x, float y )
{
}

void TouchEventRelease( float x, float y )
{
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
