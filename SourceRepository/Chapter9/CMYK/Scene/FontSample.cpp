#include "FontSample.h"
#include "Font.h"
#include <unistd.h> // used for sleep
extern int CYMKCaseType;
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
    TransformObj->TransformTranslate(-0.8, 0.80, 0.0);
    TransformObj->TransformScale(scale, scale, scale);
    
    if(CYMKCaseType == 0){
        English->printText((char*)"CYAN Coversions");
    }
    else if(CYMKCaseType == 1){
        English->printText((char*)"MAGENTA Coversions");
    }
    else if(CYMKCaseType == 2){
        English->printText((char*)"YELLOW Coversions");
    }
    else if(CYMKCaseType == 3){
        English->printText((char*)"BLACK Coversions");
    }
    else{
        English->printText((char*)"RGB - Original Print");
    }
    TransformObj->TransformPopMatrix();
}


void FontSample::InitModel()
{
    
}

