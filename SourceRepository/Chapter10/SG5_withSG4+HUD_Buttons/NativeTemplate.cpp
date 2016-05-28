// OpenGL ES 2.0 Cookcook code

#include "NativeTemplate.h"
#ifdef __IPHONE_4_0
    #include "RendererEx.h"
    #include "ObjLoader.h"
    #include "Button.h"
#else
    #include "../Scene/RendererEx.h"
    #include "../Scene/ObjLoader.h"
    #include "../Scene/Button.h"
#endif

// Icons used from
// http://neurovit.deviantart.com/art/simplicio-92311415

Renderer* graphicsEngine;
//Scene* scene1;
Scene* scene2;

//ObjLoader* Base;
//ObjLoader* Stand;
//ObjLoader* MotorShaft;
//ObjLoader* CubePlane;
//ObjLoader* Sphere;
//ObjLoader* Torus;
//ObjLoader* Suzzane;

Button* buttonUp;
Button* buttonDown;
Button* buttonLeft;
Button* buttonRight;
Button* buttonForward;
Button* buttonBackward;

bool GraphicsInit()
{
    graphicsEngine = new Renderer();
//    scene1 = new Scene("MeshScene", graphicsEngine);
//    scene1->addLight(new Light(Material(MaterialWhite),glm::vec4(0.0, 0.0, 10.0, 1.0)));
//    
//    // Base
//    Base =  new ObjLoader	( scene1, Sphere, CUBE, None );
//    Base->SetMaterial(Material(MaterialSilver));
//    Base->SetName(std::string("Base"));
//    Base->ScaleLocal(1.5, 0.25, 1.5);
//    
//    // Stand
//    Stand =  new ObjLoader	( scene1, Base, SEMI_HOLLOW_CYLINDER, None );
//    Stand->SetMaterial(Material(MaterialSilver));
//    Stand->SetName(std::string("Stand"));
//    Stand->Translate(0.0, 4.0, 0.0);
//    Stand->SetCenter(glm::vec3(0.0, -4.0, 0.0));
//    Stand->ScaleLocal(0.5, 4.0, 0.5);
//    
//    // Motor Shaft
//    MotorShaft =  new ObjLoader	( scene1, Stand, CUBE, None );
//    MotorShaft->SetMaterial(Material(MaterialSilver));
//    MotorShaft->SetName(std::string("MotorShaft"));
//    MotorShaft->Translate(0.0, 4.0, 1.0);
//    MotorShaft->SetCenter(glm::vec3(0.0, -4.0, -1.0));
//    MotorShaft->ScaleLocal(0.5, 0.5, 2.0);
//    
//    // Motor Engine
//    Sphere =  new ObjLoader	( scene1, MotorShaft, SPHERE, None );
//    Sphere->SetMaterial(Material(MaterialGold));
//    Sphere->Translate(0.0, 0.0, 2.0);
//    Sphere->SetCenter(glm::vec3(0.0, 0.0, -2.0));
//    Sphere->SetName(std::string("Sphere"));
//    
//    // Fan Blades
//    for(int i=0; i<360; i+=360/18){
//        CubePlane =  new ObjLoader	( scene1, Sphere, CUBE, None );
//        CubePlane->SetMaterial(Material(MaterialCopper));
//        CubePlane->SetName(std::string("FanBlade"));
//        CubePlane->Translate(0.0, 2.0, 0.0);
//        CubePlane->SetCenter(glm::vec3(0.0, -2.0, 0.0));
//        CubePlane->ScaleLocal(0.20, 2.0, 0.20);
//        CubePlane->Rotate(i, 0.0, 0.0, 1.0);
//    }
//    scene1->addModel( Base);
//    graphicsEngine->initializeScenes();
    
    
    glm::vec2 texCoords[4] = {
        glm::vec2(0.0f, 0.0f),
        glm::vec2(1.0f, 0.0f),
        glm::vec2(0.0f, 1.0f),
        glm::vec2(1.0f, 1.0f)
    };
    
    glm::vec3 vertices[4] = {
        glm::vec3( 0.0f, 0.0f, 0.0f),
        glm::vec3( 400.0f, 0.0f, 0.0f),
        glm::vec3( 0.0f, 400.0f, 0.0f),
        glm::vec3( 400.0f,  400.0f, 0.0f)
    };
    
    scene2 = new Scene("ButtonScene");
    
    buttonUp = new Button(scene2, NULL, None, NULL, texCoords, (char*)"dir_up.png");
    buttonUp->SetName(std::string("Direction Up"));
    buttonUp->Translate(50.0, 100, 0.0);
    buttonUp->Scale(2.0, 2.0, 2.0);
    
    buttonDown = new Button(scene2, buttonUp, None, NULL, texCoords, (char*)"dir_down.png");
    buttonDown->SetName(std::string("Direction Down"));
    buttonDown->Translate(50.0, 0.0, 0.0);
    
    buttonLeft = new Button(scene2, buttonUp, None, NULL, texCoords, (char*)"dir_left.png");
    buttonLeft->SetName(std::string("Direction Left"));
    buttonLeft->Translate(100.0, 0.0, 0.0);
    
    buttonRight = new Button(scene2, buttonUp, None, NULL, texCoords, (char*)"dir_right.png");
    buttonRight->SetName(std::string("Direction Right"));
    buttonRight->Translate(150.0, 0.0, 0.0);
    
    buttonForward = new Button(scene2, buttonUp, None, NULL, texCoords, (char*)"dir_down.png");
    buttonForward->SetName(std::string("Direction Forward"));
    buttonForward->Translate(200.0, 0.0, 0.0);
    buttonForward->SetCenter(glm::vec3(16, 16, 0));
    buttonForward->Rotate(45.0, 0.0, 0.0, 1.0);
    
    buttonBackward = new Button(scene2, buttonUp, None, NULL, texCoords, (char*)"dir_down.png");
    buttonBackward->SetName(std::string("Direction Backward"));
    buttonBackward->Translate(250.0, 0.0, 0.0);
    buttonBackward->SetCenter(glm::vec3(16, 16, 0));
    buttonBackward->Rotate(-135.0, 0.0, 0.0, 1.0);
    scene2->addModel( buttonUp );
    
    graphicsEngine->addScene(scene2);
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
