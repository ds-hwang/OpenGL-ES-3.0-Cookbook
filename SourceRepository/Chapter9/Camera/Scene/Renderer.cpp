#include "Renderer.h"
#include "ObjLoader.h"
#include "Grid.h"

const glm::vec3 CAMERA_ACCELERATION(8.0f, 8.0f, 8.0f);
const float   CAMERA_FOVX = 90.0f;
const glm::vec3 CAMERA_POS(0.0f, 1.0f, 4.0f);
const float   CAMERA_SPEED_ROTATION = 0.2f;
const float   CAMERA_SPEED_FLIGHT_YAW = 100.0f;
const glm::vec3 CAMERA_VELOCITY(2.0f, 2.0f, 2.0f);
const float   CAMERA_ZFAR = 100.0f;
const float   CAMERA_ZNEAR = 0.1f;
float     g_cameraRotationSpeed = CAMERA_SPEED_ROTATION;

int centerX = 0, centerY = 0;
int distanceCenterX = 0, distanceCenterY = 0;
float globalClock;
void Renderer::PerformCameraCollisionDetection()
{
}

float Renderer::yDistanceFromWindowCenter()
{
    return (float)distanceCenterY;
}

float Renderer::moveToWindowCenter()
{
    distanceCenterX = centerX;
    distanceCenterY = centerY;
}

float Renderer::xDistanceFromWindowCenter()
{
    return (float)distanceCenterX;
}

void Renderer::GetMovementDirection(glm::vec3& direction)
{
}

void Renderer::UpdateCamera(float elapsedTimeSec)
{
    float heading = 0.0f;
    float pitch = 0.0f;
    float roll = 0.0f;
    glm::vec3 direction(0.0f, 0.0f, 0.0f);
    //direction.z += 0.001;
    
    //GetMovementDirection(direction);
    
    switch (RenderMemData.camera->getBehavior())
    {
        case Camera::CAMERA_BEHAVIOR_FIRST_PERSON:
            pitch = yDistanceFromWindowCenter() * g_cameraRotationSpeed;
            heading = -xDistanceFromWindowCenter() * g_cameraRotationSpeed;
            
            RenderMemData.camera->rotate(heading, pitch, 0.0f);
            break;
            
        case Camera::CAMERA_BEHAVIOR_FLIGHT:
            heading = -direction.x * CAMERA_SPEED_FLIGHT_YAW * elapsedTimeSec;
            pitch = -yDistanceFromWindowCenter() * g_cameraRotationSpeed;
            roll = -xDistanceFromWindowCenter() * g_cameraRotationSpeed;
            
            RenderMemData.camera->rotate(heading, pitch, roll);
            direction.x = 0.0f; // ignore yaw motion when updating camera velocity
            break;
    }
    
    RenderMemData.camera->updatePosition(direction, elapsedTimeSec);
    PerformCameraCollisionDetection();
    
    moveToWindowCenter();
}

Renderer::Renderer(void)
{
	RenderMemData.isPerspective = true;
    RendererTransform()->TransformInit();
	RendererTransform()->TransformSetMatrixMode( VIEW_MATRIX );
    RendererTransform()->TransformLoadIdentity();
    RenderMemData.camera = new Camera(this);
    RenderMemData.camera->setBehavior(Camera::CAMERA_BEHAVIOR_FIRST_PERSON);
    RenderMemData.camera->setPosition(CAMERA_POS);
    RenderMemData.camera->setAcceleration(CAMERA_ACCELERATION);
    RenderMemData.camera->setVelocity(CAMERA_VELOCITY);
    globalClock = clock();
}

Renderer::~Renderer(void)
{
    delete RenderMemData.camera;
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
	addModel( new Grid	( this ) );
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

    //Transform Init() function is moved to the constructor no need to call every frame.
	//TransformObj->TransformInit();

	TransformObj->TransformSetMatrixMode( PROJECTION_MATRIX );

	TransformObj->TransformLoadIdentity();

	if ( considerAspectRatio ){
		GLfloat aspectRatio = (GLfloat)RenderMemData.screenWidth / (GLfloat)RenderMemData.screenHeight;
		if ( RenderMemData.isPerspective ){
			TransformObj->TransformSetPerspective(60.0f, aspectRatio, 0.01, 1000, 0);
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
//	TransformObj->TransformLoadIdentity();
    //TransformObj->TransformTranslate(0,0,-3);
    static float frame = 0.0;
    UpdateCamera((clock()-globalClock)/100000000);
    glm::mat4* m_viewMatrix = TransformObj->TransformGetViewMatrix();
    
    LOGI("00: %f",(*m_viewMatrix)[0][0]);
    LOGI("10: %f",(*m_viewMatrix)[1][0]);
    LOGI("20: %f",(*m_viewMatrix)[2][0]);
    LOGI("30: %f",(*m_viewMatrix)[3][0]);
    
    LOGI("01: %f",(*m_viewMatrix)[0][1]);
    LOGI("11: %f",(*m_viewMatrix)[1][1]);
    LOGI("21: %f",(*m_viewMatrix)[2][1]);
    LOGI("31: %f",(*m_viewMatrix)[3][1]);
    
    LOGI("02: %f",(*m_viewMatrix)[0][2]);
    LOGI("12: %f",(*m_viewMatrix)[1][2]);
    LOGI("22: %f",(*m_viewMatrix)[2][2]);
    LOGI("32: %f",(*m_viewMatrix)[3][2]);
    
    LOGI("03: %f",(*m_viewMatrix)[0][3]);
    LOGI("13: %f",(*m_viewMatrix)[1][3]);
    LOGI("23: %f",(*m_viewMatrix)[2][3]);
    LOGI("33: %f",(*m_viewMatrix)[3][3]);

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
    centerX = x;
    centerY = y;
    distanceCenterX = centerX;
    distanceCenterY = centerY;
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

 //   LOGI("Center: %d, %d", centerX, centerY);
    //LOGI("Current: %d, %d", (int)x, (int)y);

    distanceCenterX = x - centerX;
    distanceCenterY = (centerY - y);
    LOGI("dist: %d, %d", (int)distanceCenterX, (int)distanceCenterY);

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