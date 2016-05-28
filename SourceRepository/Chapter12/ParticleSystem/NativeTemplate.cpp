// OpenGL ES 3.0 Cookcook code

#include "NativeTemplate.h"
#ifdef __IPHONE_4_0
    #include "RendererEx.h"
    #include "ParticleSystem.h"
#else
    #include "../Scene/RendererEx.h"
    #include "../Scene/ParticleSystem.h"
#endif

Renderer* graphicsEngine;
Scene* scene1;

ParticleSystem* particles;
Camera* camera1;

bool GraphicsInit()
{
    graphicsEngine = new Renderer();
    glm::vec2 texCoords[4] = {
        glm::vec2(0.0f, 0.0f),
        glm::vec2(1.0f, 0.0f),
        glm::vec2(0.0f, 1.0f),
        glm::vec2(1.0f, 1.0f)
    };
    
    glm::vec3 vertices[4] = {
        glm::vec3( -.5f,  .5f, 0.0f),
        glm::vec3( 0.5f, .50f, 0.0f),
        glm::vec3( -0.50f, -0.5f, 0.0f),
        glm::vec3( 0.5f, -0.5f, 0.0f),
    };
    
    scene1 = new Scene("ButtonScene");
    scene1->addLight(new Light(Material(MaterialWhite),glm::vec4(0.0, 0.0, 10.0, 1.0)));
    camera1 = new Camera("camera1", scene1);
    camera1->MoveBackwards(120.0);
    //camera1->StrafeUpside(40.0);
    
    particles = new ParticleSystem(scene1, NULL, None, vertices, texCoords, (char*)"particle.png");
    particles->SetName(std::string("particle"));
    particles->Translate(0.0, -40.0, 0.0);
    scene1->addModel(particles);
    graphicsEngine->addScene(scene1);
    graphicsEngine->initializeScenes();

    return true;
}

bool GraphicsResize( int width, int height )
{
	graphicsEngine->resize(width, height);
    camera1->Viewport(0, 0, width, height);
    return true;
}

bool GraphicsRender()
{
    //particles->RotateLocal(10.0, 1.0, 1.0, 1.0);
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
