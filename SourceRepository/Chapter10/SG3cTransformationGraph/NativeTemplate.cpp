// OpenGL ES 2.0 Cookcook code

#include "NativeTemplate.h"
#ifdef __IPHONE_4_0
    #include "RendererEx.h"
    #include "ObjLoader.h"
#else
    #include "../Scene/RendererEx.h"
    #include "../Scene/ObjLoader.h"
#endif

Renderer* graphicsEngine;
Scene* scene1           = NULL;
ObjLoader* Base         = NULL;
ObjLoader* Stand        = NULL;
ObjLoader* MotorShaft   = NULL;
ObjLoader* CubePlane    = NULL;
ObjLoader* Sphere       = NULL;
ObjLoader* Torus        = NULL;
ObjLoader* Suzzane      = NULL;

bool GraphicsInit()
{
    graphicsEngine = new Renderer();
    scene1 = new Scene("MeshScene", graphicsEngine);
    scene1->addLight(new Light(Material(MaterialWhite),glm::vec4(0.0, 0.0, 10.0, 1.0)));
    
    // Base
    Base =  new ObjLoader	( scene1, NULL, CUBE, None );
    Base->SetMaterial(Material(MaterialSilver));
    Base->SetName(std::string("Base"));
    Base->ScaleLocal(1.5, 0.25, 1.5);
    
    // Stand
    Stand =  new ObjLoader	( scene1, Base, SEMI_HOLLOW_CYLINDER, None );
    Stand->SetMaterial(Material(MaterialSilver));
    Stand->SetName(std::string("Stand"));
    Stand->Translate(0.0, 4.0, 0.0);
    Stand->ScaleLocal(0.5, 4.0, 0.5);
    
    // Motor Shaft
    MotorShaft =  new ObjLoader	( scene1, Stand, CUBE, None );
    MotorShaft->SetMaterial(Material(MaterialSilver));
    MotorShaft->SetName(std::string("MotorShaft"));
    MotorShaft->Translate(0.0, 4.0, 1.0);
    MotorShaft->ScaleLocal(0.5, 0.5, 2.0);
    
    // Motor Engine
    Sphere =  new ObjLoader	( scene1, MotorShaft, SPHERE, None );
    Sphere->SetMaterial(Material(MaterialGold));
    Sphere->Translate(0.0, 0.0, 2.0);
    Sphere->SetName(std::string("Sphere"));
    
    // Fan Blades
    for(int i=0; i<360; i+=360/18){
        CubePlane =  new ObjLoader	( scene1, Sphere, CUBE, None );
        CubePlane->SetMaterial(Material(MaterialCopper));
        CubePlane->SetName(std::string("FanBlade"));
        CubePlane->Translate(0.0, 2.0, 0.0);
        CubePlane->SetCenter(glm::vec3(0.0, -2.0, 0.0));
        CubePlane->ScaleLocal(0.20, 2.0, 0.20);
        CubePlane->Rotate(i, 0.0, 0.0, 1.0);
    }
    scene1->addModel( Base);
    graphicsEngine->initializeScenes();
//    Sphere->SetVisible(!true, true);
//    Sphere->SetVisible(true, !true);
    Base->Rotate(45.0, 0.0, 1.0, 0.0);
//    Sphere->SetVisible(true, true);
    return true;
}

bool GraphicsResize( int width, int height )
{
	graphicsEngine->resize(width, height);
    return true;
}

bool GraphicsRender()
{
    Sphere->Rotate(1.0, 0.0, 0.0, 1.0);
    Base->Rotate(0.4, 0.0, 1.0, 0.0);
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
