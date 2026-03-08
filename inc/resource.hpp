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

#include <ethane.hpp>
#include <string>
#include <algorithm>
#include <dlfcn.h>
#include <sys/mman.h>
#include <unistd.h>

typedef struct {
    char* data;
    size_t size;
    bool valid;
} Resource;

static Resource getResource ( CString path ) {
    // Create a base resource structure
    Resource res = { Nothing, 0, False };

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

static Pointer loadLibraryFromResource ( CString path ) {
    // Get the resource first
    Resource res = getResource( path );
    if ( !res.valid )
        return Nothing;

    // Create a temporary memory file
    Integer fd = memfd_create( "bass_lib", MFD_CLOEXEC );

    if ( fd == -1 )
        return Nothing;

    // Write the data into the memory file
    write( fd, res.data, res.size );

    // Create a path for the memory file
    char fd_path[ 64 ];
    snprintf( fd_path, sizeof( fd_path ), "/proc/self/fd/%d", fd );

    // Load the library through that path
    Pointer ptr = dlopen( fd_path, RTLD_NOW );

    // When we do dlclose, the file will be freed.
    close( fd );

    return ptr;
}