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
    Font* Arabic  = dynamic_cast<Font*> (RendererHandler->getModel(FontArabic));
    Font* English = dynamic_cast<Font*> (RendererHandler->getModel(FontEnglish));
    Font* Thai    = dynamic_cast<Font*> (RendererHandler->getModel(FontThai));
    Font* Tamil   = dynamic_cast<Font*> (RendererHandler->getModel(FontTamil));
    Font* Punjabi = dynamic_cast<Font*> (RendererHandler->getModel(FontPunjabi));
    
    static float angle = 0.0;
    angle += 4.0;
    if(angle > 360){
        angle = 0.0;
        sleep(4);
    }
    float scale = 1.0/200.0;

    TransformObj->TransformPushMatrix();
    TransformObj->TransformTranslate(-1.2, 0.80, 0.0);
    TransformObj->TransformScale(scale, scale*2, scale);
    TransformObj->TransformRotate(angle, 0.0, 1.0, 0.0);
    English->printText((char*)"Arabic Sample:  ", 1.0, 1.0, 1.0, 1.0f );
    Arabic->printText( (char*)"هذا هو اللغة العربية.                        ", 1.0, 1.0, 0.0, 1.0);
    TransformObj->TransformPopMatrix();

    TransformObj->TransformPushMatrix();
    TransformObj->TransformTranslate(-1.2, 0.40, 0.0);
    TransformObj->TransformRotate(angle, 0.0, 0.0, 1.0);
    TransformObj->TransformScale(scale, scale*2, scale);
    English->printText((char*)"English Sample: ", 1.0, 1.0, 1.0, 1.0f );
    English->printText((char*)"                                 This is 'English' language.", 1.0, 0.0, 0.0, 1.0f );
    TransformObj->TransformPopMatrix();

    TransformObj->TransformPushMatrix();
    TransformObj->TransformTranslate(-1.2, 0.0, 0.0);
    TransformObj->TransformRotate(angle, 0.0, 1.0, 1.0);
    TransformObj->TransformScale(scale, scale*2, scale);
    English->printText((char*)"Thai Sample:  ", 1.0, 1.0, 1.0, 1.0f );
    Thai->printText((char*)"                                นี้เป็น ภาษาไทย", 0.0, 1.0, 1.0, 1.0f );
    TransformObj->TransformPopMatrix();
    //http://archive.ubuntu.com/ubuntu/pool/main/t/ttf-indic-fonts/

    // Tamil: Font : Uni Ila.Sundaram-03
    // http://azhagi.com/fonts-sample5.html
    TransformObj->TransformPushMatrix();
    TransformObj->TransformTranslate(-1.2, -0.40, 0.0);
    TransformObj->TransformRotate(angle, 1.0, 1.0, 1.0);
    TransformObj->TransformScale(scale, scale*2, scale);
    English->printText((char*)"Tamil Sample:  ", 1.0, 1.0, 1.0, 1.0f );
    Tamil->printText((char*)"                 இந்த தமிழ் மொழி ஆகும்.", 0.0, 1.0, 0.0, 1.0f );
    TransformObj->TransformPopMatrix();

    // Tamil: Font : Uni Ila.Sundaram-03
    // http://www.gurbanifiles.org/unicode/
    TransformObj->TransformPushMatrix();
    TransformObj->TransformTranslate(-1.2, -0.80, 0.0);
    TransformObj->TransformRotate(angle, 1.0, 0.0, 0.0);
    TransformObj->TransformScale(scale, scale*2, scale);
    English->printText((char*)"Punjabi Sample:  ", 1.0, 1.0, 1.0, 1.0f );
    Punjabi->printText((char*)"                            ਇਹ ਪੰਜਾਬੀ ਭਾਸ਼ਾ ਹੈ |", 1.0, 0.0, 1.0, 1.0f );
    TransformObj->TransformPopMatrix();

}


void FontSample::InitModel()
{
    
}

