#include "ShaderManager.h"
#include <stdio.h>
#include "glutils.h"
#include <stdlib.h>
#include <string.h>

/*!
	Default Constructor ShaderManager
	
	\param[in] NONE.
	
	\return NONE.
*/
ShaderManager::ShaderManager(void)
{
}

/*!
	Default Destructor ShaderManager
	
	\param[in] NONE.
	
	\return NONE.
*/
ShaderManager::~ShaderManager(void)
{
}

/*!
	Create a new SHADER structure.
	
	\param[in] name The internal name for the new SHADER structure pointer.
	\param[in] type The type of shader the structure will deal with. (Either GL_VERTEX_SHADER or
	GL_FRAGMENT_SHADER)
	
	\return Return a new SHADER structure pointer.
*/
SHADER *ShaderManager::ShaderInit( char *name, unsigned int type )
{
	SHADER *shader = ( SHADER * ) calloc( 1, sizeof( SHADER ) );

	strcpy( shader->name, name );

	shader->type = type;
	
	return shader;
}

/*!
	Compile a VERTEX or FRAGMENT shader code.
	
	\param[in,out] shader A valid SHADER structure pointer.
	\param[in] code The code to compile for the current SHADER.
	\param[in] debug Enable (1) or disable (0) debugging functionalities while compiling the shader.
	
	\return Return 1 if the shader code compile successfully, else return 0.
*/
unsigned char ShaderManager::ShaderCompile( SHADER *shader, const char *code, unsigned char debug )
{
	char type[ MAX_CHAR ] = {""};
	
	int loglen,
		status;
	
	if( shader->sid ) return 0;
	
	shader->sid = glCreateShader( shader->type );

    glShaderSource( shader->sid, 1, &code, NULL );

    glCompileShader( shader->sid );

	if( debug )
	{
		if( shader->type == GL_VERTEX_SHADER ) strcpy( type, "GL_VERTEX_SHADER" );
		else strcpy( type, "GL_FRAGMENT_SHADER" );
		
		glGetShaderiv( shader->sid, GL_INFO_LOG_LENGTH, &loglen );
		
		if( loglen )
		{
			char *log = ( char * ) malloc( loglen );

			glGetShaderInfoLog( shader->sid, loglen, &loglen, log );
			
			#ifdef __IPHONE_4_0			
				printf("[ %s:%s ]\n%s", shader->name, type, log );
			#else
				#ifdef _WIN32
					printf("[ %s:%s ]\n%s", shader->name, type, log );
				#else
					__android_log_print( ANDROID_LOG_ERROR, "", "[ %s:%s ]\n%s", shader->name, type, log );
				#endif
			#endif
			
			free( log );
		}
	}
    
    glGetShaderiv( shader->sid, GL_COMPILE_STATUS, &status );

	if( !status )
	{
		ShaderDelete( shader );
		return 0;
	}

	return 1;
}

/*!
	Delete the shader internal id maintained by GLES.
	
	\param[in,out] shader A valid SHADER structure pointer.
*/ 
void ShaderManager::ShaderDelete( SHADER *shader )
{
	if( shader->sid )
	{
		glDeleteShader( shader->sid );
		shader->sid = 0;
	}
}

