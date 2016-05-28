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
ObjLoader* Cube;
#define NUM_OF_PARENT 9
#define ANGLE 45.0
#define BASE_ELEMENT 8.0

Model* parents[NUM_OF_PARENT];

bool GraphicsInit()
{
    graphicsEngine = new Renderer();
    scene1 = new Scene("MeshScene", graphicsEngine);
    scene1->addLight(new Light(Material(MaterialWhite),glm::vec4(0.0, 0.0, 10.0, 1.0)));
    for(int j = 0.0; j<NUM_OF_PARENT; j++){
        parents[j] =  new DummyModel( scene1, NULL, None, "" );
        scene1->addModel( parents[j]);
        for(int i = 0.0; i<BASE_ELEMENT*j; i++){
            Cube =  new ObjLoader	( scene1, parents[j], CUBE, None );
            Cube->SetMaterial(Material(MaterialType((j%7)+1)));
            Cube->SetName(std::string("Cube"));
            if(i == 0.0){
                Cube->Rotate(ANGLE*i, 0.0, 1.0, 0.0);
            }
            else{
                Cube->Rotate(45.0*i/(2.0*j), 0.0, 1.0, 0.0);
            }
            Cube->Translate(6.0*j, 0.0, 0.0);
        }
    }
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
    for(int j = 0.0; j<NUM_OF_PARENT; j++){
        if(j%2)
            parents[j]->Rotate(-1.0*j, 0.0, 1.0, 0.0);
        else
            parents[j]->Rotate(1.0*j, 0.0, 1.0, 0.0);
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
