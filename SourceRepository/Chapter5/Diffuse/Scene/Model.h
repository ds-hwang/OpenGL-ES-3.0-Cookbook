#ifndef MODEL_H
#define MODEL_H

#include "constant.h"
#include "ProgramManager.h"

//! Forward declaration
class Renderer;
class ProgramManager;
class Transform;

// Parminder: Interface for the rendering models.
class Model
{
public:
	Model(){}
	virtual ~Model(){}
    /*!
      Load textures, initialize shaders, etc.
      */
    virtual void InitModel() = 0;

    /*!
      This is called prior to every frame.  Use this
      to update your animation.
      */
	virtual void Update( float t ) {}

    /*!
      Draw your Model.
      */
    virtual void Render() = 0;

    /*!
      Called when screen is resized
      */
	virtual void Resize(int, int) {}


    /*!
      Called when screen is resized
      */
	virtual ModelType GetModelType() { return modelType; }

    /*!
      Select the specified program for rendering current Model.
      */
	virtual bool useProgram(char* program);

	/*!
      The method will be called before rendering, this help to set the different states of the model.
      */
	virtual void setStates(){}

	/*!
      The method will be called after rendering, this help to set the different states of the model.
      */
	virtual void releaseStates(){}

	virtual void TouchEventDown( float a, float b ){}

    virtual void TouchEventDoubleClick( float x, float y ){}

    virtual void TouchEventMove( float a, float b ){}

	virtual void TouchEventRelease( float a, float b ){}

protected:
    /*!
      Store current program ID
      */
	unsigned int	ProgramID;

	/*!
      Store current program metaData
      */
	PROGRAM*		program;

	/*!
      Store current program handler
      */
	ProgramManager* ProgramManagerObj;

	/*!
      Handle Projection-View current program metaData
      */
	Transform*	TransformObj;

	/*!
      Drawing engine
      */
	Renderer*	MapRenderHandler;

	/*!
      Type of the model rendered
      */
	ModelType		modelType;
};

#endif // MODEL_H
