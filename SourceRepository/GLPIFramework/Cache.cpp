#ifdef _WIN32
#endif

#include "Cache.h"

#include <string.h>
#include <fstream>
using std::ifstream;
#include <sstream>
using std::ostringstream;
#include "GLutils.h"

/*!
	Open/Extract a file from disk and load it in cachePtr.

	\param[in] filename The file to load in cachePtr.
	\param[in] relative_path Determine if the filename is an absolute or relative path.

	\return Return a CACHE structure pointer if the file is found and loaded, instead will return
	NULL.
*/
CACHE *reserveCache( char *filename, unsigned char relative_path )
{

	#ifdef __IPHONE_4_0

		FILE *f;

		char fname[ MAX_PATH ] = {""};

		if( relative_path )
		{
            GLUtils::extractPath( getenv( "FILESYSTEM" ), fname );

			strcat( fname, filename );
		}
		else strcpy( fname, filename );

		f = fopen( fname, "rb" );

		if( !f ) return NULL;


		CACHE *cachePtr = ( CACHE * ) calloc( 1, sizeof( CACHE ) );

		strcpy( cachePtr->filename, fname );


		fseek( f, 0, SEEK_END );
		cachePtr->size = (unsigned int)ftell( f );
		fseek( f, 0, SEEK_SET );


		cachePtr->buffer = ( unsigned char * ) calloc( 1, cachePtr->size + 1 );
		fread( cachePtr->buffer, cachePtr->size, 1, f );
		cachePtr->buffer[ cachePtr->size ] = 0;


		fclose( f );

		return cachePtr;


	#else
	#ifdef _WIN32

		//Parminder: added handling for Win32 platform
		#ifdef _WIN32
		GLchar * shaderCode;
		#else
		unsigned char * shaderCode;
		#endif
		// Load contents of file into shaderCode here…
		ifstream inFile( filename, ifstream::in );
		if( !inFile ) {
			fprintf(stderr, "Error opening file: %s\n", filename );
			exit(1);
		}
		CACHE *cachePtr = ( CACHE * ) calloc( 1, sizeof( CACHE ) );
		#ifdef _WIN32
			cachePtr->buffer = (GLchar *)malloc(10000);
			memset(cachePtr->buffer, 0, 10000);
		#else
			cachePtr->buffer = (unsigned char *)malloc(10000);
		#endif
		int i = 0;
		while( inFile.good() ) {
			int c = inFile.get();
			cachePtr->buffer[i++] = c;
		}
		inFile.close();
		cachePtr->buffer[--i] = '\0';

		strcpy( cachePtr->filename, filename );
		cachePtr->position = 0;
		cachePtr->size	 = 10000;
		cachePtr->buffer[ 10000-1 ] = 0;
		return cachePtr;
	#else
		char fpath[ MAX_PATH ] = {""},
			 fname[ MAX_PATH ] = {""};

		unzFile		    uf;
		unz_file_info   fi;
		unz_file_pos    fp;
		strcpy( fpath, getenv( "FILESYSTEM" ) );
		uf = unzOpen( fpath );

		if( !uf ) return NULL;

		if( relative_path ) sprintf( fname, "assets/%s", filename );
		else strcpy( fname, filename );

		unzGoToFirstFile( uf );

		CACHE *cachePtr = ( CACHE * ) calloc( 1, sizeof( CACHE ) );

		unzGetFilePos( uf, &fp );

		if( unzLocateFile( uf, fname, 1 ) == UNZ_OK )
		{
			unzGetCurrentFileInfo(  uf,
								   &fi,
									cachePtr->filename,
									MAX_PATH,
									NULL, 0,
									NULL, 0 );

			if( unzOpenCurrentFilePassword( uf, NULL ) == UNZ_OK )
			{
				cachePtr->position = 0;
				cachePtr->size	 = fi.uncompressed_size;
				cachePtr->buffer   = ( unsigned char * ) realloc( cachePtr->buffer, fi.uncompressed_size + 1 );
				cachePtr->buffer[ fi.uncompressed_size ] = 0;

				while( unzReadCurrentFile( uf, cachePtr->buffer, fi.uncompressed_size ) > 0 ){}

				unzCloseCurrentFile( uf );

				unzClose( uf );
				return cachePtr;
			}
		}

		unzClose( uf );

		LOGI("Error: as");
		return NULL;

	#endif
	#endif
}

/*!
	Delete initialized CACHE.

	\param[in,out] cachePtr A valid CACHE structure object.

	\return Return a NULL CACHE pointer.
*/
CACHE *freeCache( CACHE *cachePtr )
{
	if( cachePtr->buffer ) free( cachePtr->buffer );

	free( cachePtr );
	return NULL;
}