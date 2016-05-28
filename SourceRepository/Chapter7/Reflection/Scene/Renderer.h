#pragma once
#include "Model.h"
#include "ProgramManager.h"
#include "Transform.h"
#include <vector>

//! Render Member Data structure
typedef struct
{
	//! Program handler for render
	ProgramManager	ProgramManagerObj;

	//! Transform operation handler
	Transform	TransformObj;

	//! Screen Height
	int screenHeight;

	//! Screen Width
	int screenWidth;

	//! Model List
	std::vector <Model*> models;

	//! Boolean value for to tell system that taping occurs.
	int HorizonHeight;

	//! Render Scene as Orthographics Or Perspective.
	bool isPerspective;

	//! Render Scene as Orthographics Or Perspective.
    glm::vec3 cameraPosition;
} RendererData;

class Renderer
{

public:
	//! Default Constructor
	Renderer(void);

	//! Destructor
	~Renderer(void);

	//! Initialize Renderer
	void initializeRenderer();

	//! Return the ProgramManager handler
	inline ProgramManager* RendererProgramManager(){ return &RenderMemData.ProgramManagerObj; }

	//! Return the Transform handler
	inline Transform*  RendererTransform() { return &RenderMemData.TransformObj;  }

	//! Screen Width returned
	inline int screenWidthPixel() { return RenderMemData.screenWidth; }

	//! Screen Height returned
	inline int screenHeightPixel() { return RenderMemData.screenHeight; }

	//! Generate the models
	void createModels();

	//! Render the Models
	void render();

	//! Initializes the Models
	void initializeModels();

	//! Remove all the models
	void clearModels();

	//! Set Project System
	void setUpProjection();

	//! Initializes the Models
	void setUpModels();

	//! Get the model
	Model* getModel( ModelType );

	//! Cache the model
	void addModel( Model* );
    
    glm::vec3 getCameraPosition(){ return RenderMemData.cameraPosition;}

    void setCameraPosition( glm::vec3 cp ){ RenderMemData.cameraPosition = -cp;}

	void TouchEventDown( float a, float b );

	void TouchEventMove( float a, float b );

	void TouchEventRelease( float a, float b );

	//! resize screen
	void resize( int w, int h );
    
	static Renderer& Instance()
    {
        static Renderer rendererObj;
        return rendererObj;
    }

	inline bool isPerspectiveView(){ return RenderMemData.isPerspective; }

private:
	//! Map render member variable container
	RendererData RenderMemData;
};
