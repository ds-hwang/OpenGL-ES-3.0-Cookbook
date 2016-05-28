#ifndef MODEL_H
#define MODEL_H

#include "constant.h"
#include "ProgramManager.h"
#include "Transform.h"
#include "Ray.h"
#include "Material.h"

#include <vector>

//! Forward declaration
class Renderer;
class ProgramManager;

// Parminder: Interface for the rendering models.
class Model
{
public:
    Model(Renderer*	RendererHandler, Model* model, ModelType type);
    virtual ~Model();
    /*!
      Load textures, initialize shaders, etc.
      */
    virtual void InitModel();

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

    virtual void TouchEventDown( float a, float b );

	virtual void TouchEventMove( float a, float b ){}

	virtual void TouchEventRelease( float a, float b ){}

    virtual bool IntersectWithRay(Ray ray0, glm::vec3& intersectionPoint);
    
    void Rotate( float angle, float x, float y, float z );
    
    void Translate(float x, float y, float z );
    
    void Scale(float x, float y, float z );

    void RotateLocal( float angle, float x, float y, float z );
    
    void TranslateLocal(float x, float y, float z );
    
    void ScaleLocal(float x, float y, float z );
    
    void SetParent(Model* parent = 0);

    void RemoveParent();
    
    void SetChild(Model* child = 0);

    void RemoveChild();

    Model* GetParent();
    
    std::vector<Model*>* GetChildren();
    
    glm::mat4 GetEyeCoordinatesFromRoot();

    glm::mat4 GetParentTransformation(Model* parent);

    void SetMaterial(Material mat){ materialObj = mat; }
    
    const Material& GetMaterial(){ return materialObj; }
    
    void SetName(std::string mdlName){ name = mdlName;}
    
    std::string GetName(){return name;}

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
	Renderer*	RendererHandler;

	/*!
      Type of the model rendered
      */
	ModelType		modelType;
    
    /*!
    Transformation of the model
     */
    glm::mat4 transformation;

    /*!
     Local transformation of the model
     */
    glm::mat4 transformationLocal;
    
    /*!
     Model's parent
     */
    Model* parent;
    
    /*!
     Model's child list
     */
    std::vector<Model*> childList;

    /*!
     Model's material
     */
    Material materialObj;

    /*!
     Model's name
     */
    std::string name;
};

#endif // MODEL_H
