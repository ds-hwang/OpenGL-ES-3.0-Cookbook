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
    float scale = 1.5/200.0;

    TransformObj->TransformPushMatrix();
    TransformObj->TransformTranslate(-0.5, 0.50, 0.0);
    TransformObj->TransformScale(scale, scale, scale);
    English->printText((char*)"Binocular view");
    TransformObj->TransformPopMatrix();
}


void FontSample::InitModel()
{
    
}

