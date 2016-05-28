#ifndef PROGRAM_MANA_H
#define PROGRAM_MANA_H

#pragma once
#include "ShaderManager.h"

//! Structure to deal with GLSL uniform variables.
typedef struct
{
	//! The name of the uniform.
	char			name[ MAX_CHAR ];

	//! The variable type for this uniform.
	unsigned int	type;

	//! The location id maintained by GLSL for this uniform.
	int				location;

	//! Determine if the uniform is constant or shoud be updated every frame.
	unsigned char	constant;

} UNIFORM;


//! Structure to deal with vertex attribute variables.
typedef struct
{
	//! The name of the vertex attribute.
	char			name[ MAX_CHAR ];

	//! The variable type for this vertex attribute.
	unsigned int	type;

	//! The location of the id maintained GLSL for this vertex attribute.
	int				location;

} VERTEX_ATTRIB;


//! Draw callback function prototype.
typedef void( PROGRAMDRAWCALLBACK( void * ) );

//! Callback function prototypes for binding vertex attribute before compilation.
typedef void( PROGRAMBINDATTRIBCALLBACK( void * ) );


//! Structure to easily handle GLSL programs.
typedef struct
{
	//! The internal name to use for the shader program.
	char						 name[ MAX_CHAR ];

	//! The vertex shader.
	SHADER						 *VertexShader;

	//! The fragment shader.
	SHADER						 *FragmentShader;

	//! The internal GLES program id for the shader program.
	unsigned int				 ProgramID;

	//! The number of uniforms.
	unsigned char				 UniformCount;

	//! Array of UNIFORM variables.
	UNIFORM						 *UniformArray;

	//! The number of vertex attributes.
	unsigned char				 VertexAttribCount;

	//! Array of vertex attributes.
	VERTEX_ATTRIB				 *VertexAttribArray;

} PROGRAM;


class ProgramManager
{
public:
	ProgramManager(void);
	~ProgramManager(void);

public:
	PROGRAM *ProgramInit( char *name );

	PROGRAM *ProgramFree( PROGRAM *program );

	unsigned char ProgramLink( PROGRAM *program, unsigned char debug );

	char ProgramGetVertexAttribLocation( PROGRAM *program, char *name );

	char ProgramGetUniformLocation( PROGRAM *program, char *name );

	void ProgramDeleteId( PROGRAM *program );

	void ProgramReset( PROGRAM *program );

    PROGRAM* ProgramLoad(char* name, char* vertexShaderPath, char* fragmentShaderPath);
	unsigned char ProgramLoad( PROGRAM *program, PROGRAMBINDATTRIBCALLBACK	*ProgramBindAttribCallback, PROGRAMDRAWCALLBACK	*ProgramDrawCallback, char *filename, unsigned char	debug_shader, unsigned char relative_path );

	unsigned char ProgramAddUniform( PROGRAM *program, char *name, unsigned int type );

	unsigned char ProgramAddVertexAttrib( PROGRAM *program, char *name, unsigned int type );

    PROGRAM* Program( char* program );

    PROGRAM* Program( int shaderID );

	unsigned char AddProgram( PROGRAM* program );

	unsigned char RemoveProgram( PROGRAM* program );

	inline unsigned char ProgramCount() { return programCounter; }

private:
	//! We need some mechanism to recognize the program for each model.
	//! Parminder temporarily using a array, must use a list instead.
	PROGRAM* programList[20]; 

	//! Keeps track of the number of program in the renderer.
	unsigned char programCounter;
};

#endif
