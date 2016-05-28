#ifndef TRANSFORM_H
#define TRANSFORM_H

#pragma once
#include "glutils.h"
#include "constant.h"

//! The definition of the global  structure. This structure maintain the matrix stacks and current indexes. 
typedef struct
{
	//! The current matrix mode (either MODEL_MATRIX, VIEW_MATRIX, PROJECTION_MATRIX, TEXTURE_MATRIX).
	unsigned char	matrix_mode;
	
    //! The current modelview matrix index in the stack.
	unsigned char	modelMatrixIndex;

	//! The current modelview matrix index in the stack.
	unsigned char	viewMatrixIndex;

	//! The current projection matrix index in the stack.
	unsigned char	projectionMatrixIndex;
	
	//! The current texture matrix index in the stack.	
	unsigned char	textureMatrixIndex;

    //! Array of 4x4 matrix that represent the model matrix stack.
	glm::mat4			model_matrix[ MAX_MODEL_MATRIX ];
	
    //! Array of 4x4 matrix that represent the view matrix stack.
	glm::mat4			view_matrix[ MAX_VIEW_MATRIX ];

	//! Array of 4x4 matrix that represent the projection matrix stack.
	glm::mat4			projection_matrix[ MAX_PROJECTION_MATRIX ];

	//! Array of 4x4 matrix that represent the texture matrix stack.
	glm::mat4			texture_matrix[ MAX_TEXTURE_MATRIX ];
	
	//! Used to store the result of the modelview matrix multiply by the projection matrix. \sa _get_modelview_projection_matrix
	glm::mat4			modelview_projection_matrix;

	//! Used to store the result of the modelview matrix multiply by the projection matrix. \sa _get_modelview_projection_matrix
	glm::mat4			modelview_matrix;
	
	//! Used to store the result of the inverse, tranposed modelview matrix. \sa _get_normal_matrix
	glm::mat3			normal_matrix;
} TransformData;


class Transform
{
public:
	Transform(void);
	~Transform(void);

	void TransformInit( void );

	void TransformError( void );

	void TransformSetMatrixMode( unsigned int mode );

	void TransformLoadIdentity( void );

	void TransformPushMatrix( void );

	void TransformPopMatrix( void );

	void TransformLoadMatrix( glm::mat4 *m );

	void TransformMultiplyMatrix( glm::mat4 *m );

	void TransformTranslate( float x, float y, float z );

	void TransformRotate( float angle, float x, float y, float z );

	void TransformScale( float x, float y, float z );

	glm::mat4 *TransformGetModelMatrix( void );
    
	glm::mat4 *TransformGetViewMatrix( void );
    
	glm::mat4 *TransformGetProjectionMatrix( void );

	glm::mat4 *TransformGetTextureMatrix( void );

	glm::mat4 *TransformGetModelViewProjectionMatrix( void );

	glm::mat4 *TransformGetModelViewMatrix( void );

	void TransformGetNormalMatrix( glm::mat3* );

	void TransformOrtho( float left, float right, float bottom, float top, float clip_start, float clip_end );

	void TransformOrthoGrahpic( float screen_ratio, float scale, float aspect_ratio, float clip_start, float clip_end, float screen_orientation );

	void TransformSetPerspective( float fovy, float aspect_ratio, float clip_start, float clip_end, float screen_orientation );

	void TransformLookAt( glm::vec3 *eye, glm::vec3 *center, glm::vec3 *up );

	void TransformSetView(glm::mat4 mat);

	int TransformProject( float objx, float objy, float objz, glm::mat4 *modelview_matrix, glm::mat4 *projection_matrix, int *viewport_matrix, float *winx, float *winy, float *winz );

	int TransformUnproject( float winx, float winy, float winz, glm::mat4 *modelview_matrix, glm::mat4 *projection_matrix, int *viewport_matrix, float *objx, float *objy, float *objz );

	void Vec4MultiplyMat4( glm::vec4* dst, glm::vec4 *v0, glm::mat4 *v1 );

	TransformData TransformMemData;
};

#endif
