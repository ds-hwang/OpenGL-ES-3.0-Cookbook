#ifndef SHADERMANAGER_H
#define SHADERMANAGER_H

#pragma once

//! Header File inclusion
#include "constant.h"

//! Structure to handle shading language shader object.
typedef struct
{
	//! Name assigned to shader.
	char			name[ MAX_CHAR ];

	//! Shader type. [GL_VERTEX_SHADER or GL_FRAGMENT_SHADER]
	unsigned int	type;

	//! The Shader's unique ID.
	unsigned int	sid;

} SHADER;


class ShaderManager
{

public:
    //! Constructor Shader class.
	ShaderManager(void);

    //! Destructor Shader class.
	~ShaderManager(void);

    //! Create a new shader of type mentioned.
	static SHADER*			ShaderInit( char *name, unsigned int type );

    //! Compilation process for Shader.
	static unsigned char	ShaderCompile( SHADER *shader, const char *code, unsigned char debug );

    //! Free the Shader.
	static void			    ShaderDelete( SHADER *shader );
};

#endif
