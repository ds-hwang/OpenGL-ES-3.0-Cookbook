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
Scene* scene1;
ObjLoader* Sphere;
ObjLoader* BaseSphere;
ObjLoader* Cube[2];

bool GraphicsInit()
{
    graphicsEngine = new Renderer();
    scene1 = new Scene("MeshScene", graphicsEngine);
    scene1->addLight(new Light(Material(MaterialWhite),glm::vec4(0.0, 0.0, 10.0, 1.0)));

    BaseSphere =  new ObjLoader	( scene1, NULL, SPHERE, None );
    BaseSphere->SetMaterial(Material(MaterialGold));
    BaseSphere->ScaleLocal(1.5,1.5,1.5);
    int j = 0;
    for(int i=-1; i<2; i+=2){
        Cube[j] =  new ObjLoader	( scene1, BaseSphere, CUBE, None );
        Cube[j]->SetMaterial(Material(MaterialCopper));
        Cube[j]->Translate(10.0*i, 0.0, 0.0);
        for(int i=-1; i<2; i+=2){
            Sphere =  new ObjLoader	( scene1, Cube[j], SPHERE, None );
            Sphere->SetMaterial(Material(MaterialSilver));
            Sphere->Translate(0.0, -5.0*i, 0.0);
        }
        j++;
    }
    scene1->addModel( BaseSphere);
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
    BaseSphere->Rotate(1.0, 0.0, 1.0, 0.0);
    Cube[0]->Rotate(-1.0, 1.0, 0.0, 0.0);
    Cube[1]->Rotate( 1.0, 1.0, 0.0, 0.0);
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
