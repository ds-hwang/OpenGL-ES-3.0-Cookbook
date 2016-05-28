#include "FontSample.h"
#include "Font.h"
#include <unistd.h> // used for sleep
#include "Cube.h"

FontSample::FontSample(Renderer* parent)
{
    RendererHandler     = parent;
    ProgramManagerObj	= parent->RendererProgramManager();
    TransformObj		= parent->RendererTransform();
}

FontSample::~FontSample()
{
}

void FontSample::HeadUpDisplay(int w, int h)
{
    TransformObj->TransformSetMatrixMode( PROJECTION_MATRIX );
    
    TransformObj->TransformLoadIdentity();
    // Left ,Right ,Bottom , Top, Near, Far
    TransformObj->TransformOrtho( 0, w, 0, h, -1, 1);
    
    TransformObj->TransformSetMatrixMode( VIEW_MATRIX );
    TransformObj->TransformLoadIdentity();
    
    TransformObj->TransformSetMatrixMode( MODEL_MATRIX );
    TransformObj->TransformLoadIdentity();
}

void FontSample::Render()
{
    int	  viewport_matrix[4];
    glGetIntegerv( GL_VIEWPORT, viewport_matrix );
    HeadUpDisplay(viewport_matrix[2], viewport_matrix[3]);

    Font* English    = dynamic_cast<Font*> (RendererHandler->getModel(FontEnglish));
    Cube* cubeObject = dynamic_cast<Cube*> (RendererHandler->getModel(CubeType));
    std::vector<glm::vec2>* vertexVector = cubeObject->getScreenCoordinateVertices();
    
    char buffer[500];
    for(int i = 0; i<vertexVector->size(); i++) {
        TransformObj->TransformPushMatrix();
        TransformObj->TransformTranslate(vertexVector->at(i).x, vertexVector->at(i).y, 0.0);
        TransformObj->TransformScale(2.0, 2.0, 2.0);
        memset(buffer, 0, 500);
        sprintf(buffer, "Vertex pos: %d,%d", (int)vertexVector->at(i).x, (int)vertexVector->at(i).y);
        English->printText(buffer, 1.0, 1.0, 1.0, 1.0f );
        TransformObj->TransformPopMatrix();
    }
}


void FontSample::InitModel()
{
    
}