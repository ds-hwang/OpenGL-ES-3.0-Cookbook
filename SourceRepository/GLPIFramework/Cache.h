#ifndef CACHE_H
#define CACHE_H
#ifdef _WIN32
#include <GL/glew.h>
#else
#endif
#include "constant.h"
#include "zlib/zlib.h"
#include "zlib/unzip.h"
#include "zlib/zutil.h"
#define MAX_PATH	256

/*!
	\file Cache.h

	\brief Function prototypes and definitions to use with the CACHE structure.
*/


//! Structure for store Cached information.
typedef struct
{
	char			filename[ MAX_PATH ];

	unsigned int	size;

	unsigned int	position;

	unsigned char	*buffer;
} CACHE;


CACHE *reserveCache( char *filename, unsigned char relative_path );

CACHE *freeCache(CACHE *cachePtr );
#endif
