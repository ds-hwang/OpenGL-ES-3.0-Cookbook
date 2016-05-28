#include "FontSample.h"
#include "Font.h"
#include <unistd.h> // used for sleep
FontSample::FontSample(Renderer* parent)
{
    RendererHandler     = parent;
    ProgramManagerObj	= parent->RendererProgramManager();
    TransformObj		= parent->RendererTransform();
}

FontSample::~FontSample()
{
}

void FontSample::Render()
{
    Font* English = dynamic_cast<Font*> (RendererHandler->getModel(FontEnglish));
    static float angle = 0.0;
    static float angle2 = 0.0;
    angle += 4.0;
    if(angle > 360){
        angle = 0.0;
        //sleep(4);
    }
    float scale = 1.5/200.0;

    TransformObj->TransformPushMatrix();
    TransformObj->TransformTranslate(-1.0, 1.20, 0.0);
    TransformObj->TransformRotate(angle, 1.0, 0.0, 0.0);
    TransformObj->TransformScale(scale, scale*2, scale);
    English->printText((char*)"This is Free Type Project", 1.0, 0.0, 0.0, 1.0f );
    TransformObj->TransformPopMatrix();

    TransformObj->TransformPushMatrix();
    TransformObj->TransformTranslate(-0.70, 0.0, 0.0);
    //TransformObj->TransformRotate(angle, 1.0, 0.0, 0.0);
    TransformObj->TransformScale(scale, scale*2, scale);
    English->printText((char*)"Be Calm, Carry On.", 1.0, 1.0, 0.0, 1.0f );
    TransformObj->TransformPopMatrix();
    
    TransformObj->TransformPushMatrix();
    TransformObj->TransformTranslate(-0.0, -1.0, 0.0);
    TransformObj->TransformRotate(15.0, 1.0, 0.0, 0.0);
    TransformObj->TransformScale(scale, scale*2, scale);
    English->animateText((char*)"Hello World !!!          ", 1.0, 0.0, 1.0, 1.0f, 100.0, angle2+=1.0 );
    TransformObj->TransformPopMatrix();
}


void FontSample::InitModel()
{
    
}

