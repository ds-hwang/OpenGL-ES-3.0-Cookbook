#include "Renderer.h"
#include "ObjLoader.h"

Renderer::Renderer(void)
{
	RenderMemData.isPerspective = true;
}

Renderer::~Renderer(void)
{
}

void Renderer::initializeRenderer()
{
	LOGI("initializeRenderer");
	setUpModels();
}

void Renderer::createModels()
{
	clearModels();
	addModel( new ObjLoader	( this ) );
}

/*!
	Remove all the Model classes from the Renderer.

	\param[in] None.

	\return None.
*/
void Renderer::clearModels()
{
	for( int i=0; i<RenderMemData.models.size();  i++ ){
		delete RenderMemData.models.at(i);
	}
	RenderMemData.models.clear();
}

/*!
	Setup of the view port and projection initialization. In projection initialization the Ortho or Perspective is set
	as per requirement.

	\param[in] width of the screen.
	\param[in] height of the screen.

	\return void.
*/
void Renderer::setUpProjection()
{
	ProgramManager* ProgramManagerObj	= &RenderMemData.ProgramManagerObj;
	Transform*	TransformObj			= &RenderMemData.TransformObj;
	bool considerAspectRatio			= true;
	float span							= 10.0;

	TransformObj->TransformInit();

	TransformObj->TransformSetMatrixMode( PROJECTION_MATRIX );

	TransformObj->TransformLoadIdentity();

	if ( considerAspectRatio ){
		GLfloat aspectRatio = (GLfloat)RenderMemData.screenWidth / (GLfloat)RenderMemData.screenHeight;
		if ( RenderMemData.isPerspective ){
			TransformObj->TransformSetPerspective(60.0f, aspectRatio, 0.01, 100, 0);
		}else{
			if ( RenderMemData.screenWidth <= RenderMemData.screenHeight ){
				TransformObj->TransformOrtho( -span, span, -span / aspectRatio, span / aspectRatio, -span, span);
			}
			else{
				TransformObj->TransformOrtho( -span * aspectRatio, span * aspectRatio, -span, span, -span, span);
			}
		}
	}
	else{
		if ( RenderMemData.isPerspective ){
			TransformObj->TransformSetPerspective(60.0f, 1, 1.0, 100, 0);
		}
		else{
			TransformObj->TransformOrtho( -span, span, -span, span, -span, span );
		}
	}
	TransformObj->TransformSetMatrixMode( VIEW_MATRIX );
	TransformObj->TransformLoadIdentity();

	TransformObj->TransformSetMatrixMode( MODEL_MATRIX );
	TransformObj->TransformLoadIdentity();
}

/*!
	Setup of the view port and projection initialization. In projection initialization the Ortho or Perspective is set
	as per requirement.

	\param[in] width of the screen.
	\param[in] height of the screen.

	\return void.
*/
void Renderer::resize(int w, int h)
{
	RenderMemData.screenWidth		= w;
	RenderMemData.screenHeight		= h;
	glViewport( 0, 0, w, h );
}

/*!
	Create and Initializes all models.

	\param[in] void.

	\return void.
*/
void Renderer::setUpModels()
{
	// ! Generate the models and cache all
	createModels();

	//! Do the init stuff as per model requirements
	initializeModels();
}

/*!
	This returns the model renderer of the model. A Model renderer is basically a class responsible for managing the same type of models.

	\param[in] type Enumeration of the model type want to query.

	\return Model pointer.
*/
Model* Renderer::getModel( ModelType type)
{
	for( int i=0; i<RenderMemData.models.size();  i++ )
	{
		Model* model = RenderMemData.models.at(i);

		if ( model->GetModelType() == type )
			return model;
	}

	return NULL;
}

/*!
	This function is responsible for adding a model in the Renderer model vector.
	The Renderer handle all the models from a unique vector based list.

	\param[in] Model pointer of the Model user want to add.

	\return None.
*/
void Renderer::addModel(Model* model)
{
	RenderMemData.models.push_back( model );
}

/*!
	This function handle Touch event down action.

	\param[in] x and y screen pixel position.

	\return None.
*/
void Renderer::TouchEventDown( float x, float y )
{
	for( int i=0; i<RenderMemData.models.size(); i++ ){
		RenderMemData.models.at(i)->TouchEventDown(x, y);
	}
    
}

void Renderer::TouchEventDoubleClick( float x, float y )
{
    for( int i=0; i<RenderMemData.models.size(); i++ ){
        RenderMemData.models.at(i)->TouchEventDoubleClick(x, y);
    }
    
}

/*!
	This function handle Touch event move action.

	\param[in] x and y screen pixel position.

	\return None.
*/
void Renderer::TouchEventMove( float x, float y )
{
	for( int i=0; i<RenderMemData.models.size(); i++ ){
		RenderMemData.models.at(i)->TouchEventMove(x, y);
	}
}

/*!
	This function handle Touch event release action.

	\param[in] x and y screen pixel position.

	\return None.
*/
void Renderer::TouchEventRelease( float x, float y )
{
	for( int i=0; i<RenderMemData.models.size(); i++ ){
		RenderMemData.models.at(i)->TouchEventRelease(x, y);
	}
}


/*!
	Performs initializing of all the models this may include setup for shaders their caching.
	rendering for each model, creation and caching of IBO, VBO, FBO etc.

	\param[in] void.

	\return void.
*/
void Renderer::initializeModels()
{
	for( int i=0; i<RenderMemData.models.size();  i++ )
		RenderMemData.models.at(i)->InitModel();
}

/*!
	Performs rendering for each model

	\param[in] void.

	\return void.
*/
void Renderer::render()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

	for( int i=0; i<RenderMemData.models.size();  i++ ){
		RenderMemData.models.at(i)->Render();
	}
}