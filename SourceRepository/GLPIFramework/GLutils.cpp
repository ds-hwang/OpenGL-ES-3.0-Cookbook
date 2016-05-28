#include "glutils.h"

#ifdef _WIN32
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>
#endif

GLUtils::GLUtils() {}

int GLUtils::checkForOpenGLError(const char * file, int line) {
    //
    // Returns 1 if an OpenGL error occurred, 0 otherwise.
    //
    int    retCode = 0;
	#ifdef _WIN32
    GLenum glErr;

    glErr = glGetError();
    while (glErr != GL_NO_ERROR)
    {
        LOGI("glError in file %s @ line %d: %s\n", file, line, gluErrorString(glErr));
        retCode = 1;
        glErr = glGetError();
    }
	#endif
    return retCode;

}

void GLUtils::printGLInfo(bool printExtensions) {
	LOGI("\nGL_VENDOR:      %s\n", ( char * )glGetString( GL_VENDOR     ) );
	LOGI("\nGL_RENDERER:    %s\n"  , ( char * )glGetString( GL_RENDERER   ) );
	LOGI("\nGL_VERSION:     %s\n"  , ( char * )glGetString( GL_VERSION    ) );
	LOGI("\nGL_SHADING_LANGUAGE_VERSION:  %s\n"  , ( char * )glGetString( GL_SHADING_LANGUAGE_VERSION ) );
	if (printExtensions){
		LOGI("\nGL_EXTENSIONS:  %s\n"  , ( char * )glGetString( GL_EXTENSIONS ) );
	}
}

void GLUtils::modifyPath( char *path )
{
    unsigned int i = 0;
    unsigned int l = (unsigned int)strlen( path );
    
    while( i != l )
    {
        if( path[ i ] == '\\' ) {
            path[ i ] = '/';
        }
        ++i;
    }
}


/*!
 Extract path from a file path.
 
 \param[in] filepath The file path to use to extract the path.
 \param[in,out] path The return path.
 */
void GLUtils::extractPath( char *path, char *modifiedPath )
{
    char *f			= NULL;
    unsigned int p	= 0;
    
    modifyPath( path );
    
    f = strrchr( path, '/' );
    
    if( f )
    {
        p = (unsigned int)(f - path) + 1;
        strncpy( modifiedPath, path, p );
        path[ p ] = 0;
    }
}


/*!
 Extract the file name from given file path.
 
 \param[in] path complete path contain file name.
 \param[in,out] filename extracted.
 
 */
void GLUtils::extractFileName( char *filepath, char *filename )
{
    char *t = NULL;
    
    modifyPath( filepath );
    
    t = strrchr( filepath, '/' );
    
    if( t ) strcpy( filename, t + 1 );
    else strcpy( filename, filepath );
}

/*!
 Get next best fit from the specified current 'number'.
 
 \param[in] number, input number.
 \param[in,out] next immediate power of two.
 
 */
int GLUtils::nextPowerOf2 (int number)
{
    int rval = 1;
    while (rval<number) rval*=2;
    return rval;
}

//! Byte swap unsigned short
uint16_t GLUtils::swap_uint16( uint16_t val )
{
    return (val << 8) | (val >> 8 );
}

//! Byte swap short
int16_t GLUtils::swap_int16( int16_t val )
{
    return (val << 8) | ((val >> 8) & 0xFF);
}

//! Byte swap unsigned int
uint32_t GLUtils::swap_uint32( uint32_t val )
{
    val = ((val << 8) & 0xFF00FF00 ) | ((val >> 8) & 0xFF00FF );
    return (val << 16) | (val >> 16);
}

//! Byte swap int
int32_t GLUtils::swap_int32( int32_t val )
{
    val = ((val << 8) & 0xFF00FF00) | ((val >> 8) & 0xFF00FF );
    return (val << 16) | ((val >> 16) & 0xFFFF);
}

int64_t GLUtils::swap_int64( int64_t val )
{
    val = ((val << 8) & 0xFF00FF00FF00FF00ULL ) | ((val >> 8) & 0x00FF00FF00FF00FFULL );
    val = ((val << 16) & 0xFFFF0000FFFF0000ULL ) | ((val >> 16) & 0x0000FFFF0000FFFFULL );
    return (val << 32) | ((val >> 32) & 0xFFFFFFFFULL);
}

uint64_t GLUtils::swap_uint64( uint64_t val )
{
    val = ((val << 8) & 0xFF00FF00FF00FF00ULL ) | ((val >> 8) & 0x00FF00FF00FF00FFULL );
    val = ((val << 16) & 0xFFFF0000FFFF0000ULL ) | ((val >> 16) & 0x0000FFFF0000FFFFULL );
    return (val << 32) | (val >> 32);
}
