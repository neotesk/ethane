/*
    Ethane, a Research Project for hacking GoldSrc games.
    Open-Source, Public Domain.

    2026 neotesk.
*/

/*
    This is the resource manager. Here's how it works:
    When you add a resource through our Makefile, it processes
    those resources: converts them to object files and then
    certain symbols like _binary_*_start and _binary_*_size are
    globalized. The code below gathers those objects and returns
    a Resource structure for you.

    Usage:
        Resource myRes = getResource( "res/myRes.xm" );
        Resource.data  // This is the data
        Resource.size  // This is the data size
        Resource.valid // Make sure to check if this bool is set to true
                       // before doing anything.
*/

#pragma once

#include <string>
#include <algorithm>
#include <dlfcn.h>

typedef struct {
    char* data;
    size_t size;
    bool valid;
} Resource;

static Resource getResource ( const char* path ) {
    // Create a base resource structure
    Resource res = { nullptr, 0, false };

    // If the path is empty or null, skip the
    // processing and return the structure
    if ( !path || path[ 0 ] == '\0' )
        return res;

    // Create an objPath string because we are going to
    // manipulate the path to remove illegal characters.
    std::string objPath = "_binary_";
    objPath += path;

    // Sanitize the objPath
    std::transform( objPath.begin() + 8, objPath.end(),
        objPath.begin() + 8, []( unsigned char c ) {
        if ( std::isalnum( c ) )
            return ( char )c;
        return '_';
    } );

    // Get the current length because we are going to
    // reuse objPath to get two symbols.
    const size_t baseLen = objPath.length();

    // Get the start symbol
    objPath += "_start";
    void *dataPtr = dlsym( RTLD_DEFAULT, objPath.c_str() );

    // Get the size symbol
    objPath.replace( baseLen, std::string::npos, "_size" );
    void *sizePtr = dlsym( RTLD_DEFAULT, objPath.c_str() );

    // If both of those symbols exist, populate the resource
    // and set the valid bool to true
    if ( dataPtr && sizePtr ) {
        res.data = ( char* )dataPtr;
        res.size = ( size_t )sizePtr;
        res.valid = true;
    }

    return res;
}