#ifndef _FONT_SAMPLE_H
#define _FONT_SAMPLE_H

#include "glutils.h"
#include "Renderer.h"
#include "Model.h"

class FontSample : public Model
{
public:
    FontSample(Renderer* parent);
    ~FontSample();
    void Render();
    void InitModel();
};

#endif
