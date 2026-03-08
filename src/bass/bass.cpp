/*
    Ethane, a Research Project for hacking GoldSrc games.
    Open-Source, Public Domain.

    2026 neotesk.
*/

/*
    This is BASS, a great library for playing sounds
    and music. It's a freeware library. The code below
    is a simple wrapper for it so it works in dynamic
    environments.
*/

#include <ethane.hpp>
#include <resource.hpp>

#define SYM( handle, name ) \
    name = ( name##_t )dlsym( handle, #name ); \
    AssertV( !name, "Cannot load symbol of BASS '%s'", #name )

typedef Integer ( *BASS_Init_t )( Integer device, UInteger freq,
    UInteger flags, Pointer win, Pointer dsguid );

typedef UInteger ( *BASS_MusicLoad_t )( UInteger fileType, Pointer file,
    UInt64 offset, UInteger size, UInteger flags, UInteger freq );

typedef UInteger ( *BASS_SampleLoad_t )( UInteger fileType, Pointer file,
    UInt64 offset, UInteger size, UInteger max, UInteger flags );

typedef UInteger ( *BASS_SampleGetChannel_t )( UInteger handle,
    Integer onlyNew );

typedef Integer ( *BASS_ChannelPlay_t )( UInteger handle, Integer restart );
typedef Integer ( *BASS_ChannelStop_t )( UInteger handle );

typedef Integer ( *BASS_Free_t )();
typedef Integer ( *BASS_MusicFree_t )( UInteger handle );
typedef Integer ( *BASS_SampleFree_t )( UInteger handle );

static BASS_Init_t BASS_Init;
static BASS_MusicLoad_t BASS_MusicLoad;
static BASS_SampleLoad_t BASS_SampleLoad;
static BASS_SampleGetChannel_t BASS_SampleGetChannel;
static BASS_ChannelPlay_t BASS_ChannelPlay;
static BASS_ChannelStop_t BASS_ChannelStop;
static BASS_Free_t BASS_Free;
static BASS_MusicFree_t BASS_MusicFree;
static BASS_SampleFree_t BASS_SampleFree;

Pointer bassHandle = Nothing;
Boolean areWeLoaded = False;

void loadLibrary ( CString path ) {
    // Check if already loaded
    if ( areWeLoaded )
        return;

    // Load the library from the memory.
    bassHandle = loadLibraryFromResource( path );
    AssertV( !bassHandle, "BASS Library from resource path '%s' "
        "is not found", path );

    // Now load needed functions
    SYM( bassHandle, BASS_Init );
    SYM( bassHandle, BASS_MusicLoad );
    SYM( bassHandle, BASS_SampleLoad );
    SYM( bassHandle, BASS_SampleGetChannel );
    SYM( bassHandle, BASS_ChannelPlay );
    SYM( bassHandle, BASS_ChannelStop );
    SYM( bassHandle, BASS_Free );
    SYM( bassHandle, BASS_MusicFree );
    SYM( bassHandle, BASS_SampleFree );

    // Now that we are loaded, set areWeLoaded to true
    areWeLoaded = True;
}

namespace BASS {
    Boolean Init ( CString path, Integer device, UInteger sampleRate ) {
        loadLibrary( path );
        Assert( !bassHandle, "Cannot initialize BASS library." );
        return BASS_Init( device, sampleRate, 0, Nothing, Nothing );
    }
    UInteger MusicLoad ( UInteger fileType, Pointer file, UInteger length,
        UInteger flags ) {
        Assert( !bassHandle, "BASS is not initialized." );
        return BASS_MusicLoad( fileType, file, 0, length, flags, 0 );
    }
    UInteger MusicLoadFromMemory ( Pointer file, UInteger length,
        UInteger flags ) {
        return MusicLoad( 1, file, length, flags );
    }
    UInteger MusicLoadFromFile ( Pointer file, UInteger flags ) {
        return MusicLoad( 0, file, 0, flags );
    }
    UInteger SampleLoad ( UInteger fileType, Pointer file, UInteger length,
        UInteger flags, UInteger max = 3 ) {
        Assert( !bassHandle, "BASS is not initialized." );
        return BASS_SampleLoad( fileType, file, 0, length, max, flags );
    }
    UInteger SampleLoadFromMemory ( Pointer file, UInteger length,
        UInteger flags, UInteger max = 3 ) {
        return SampleLoad( 1, file, length, flags );
    }
    UInteger SampleLoadFromFile ( Pointer file, UInteger flags,
        UInteger max = 3 ) {
        return SampleLoad( 0, file, 0, flags, max );
    }
    UInteger SampleGetChannel ( UInteger handle, Boolean onlyNew ) {
        Assert( !bassHandle, "BASS is not initialized." );
        return BASS_SampleGetChannel( handle, onlyNew );
    }
    Boolean ChannelPlay ( UInteger handle, Boolean restart ) {
        Assert( !bassHandle, "BASS is not initialized." );
        return BASS_ChannelPlay( handle, restart );
    }
    Boolean SamplePlay ( UInteger sampleHandle ) {
        UInteger channel = SampleGetChannel( sampleHandle, false );
        Assert( !channel, "Given channel at pointer %p is invalid.", channel );
        return ChannelPlay( channel, True );
    }
    Boolean ChannelStop ( UInteger handle ) {
        Assert( !bassHandle, "BASS is not initialized." );
        return BASS_ChannelStop( handle );
    }
    Sub MusicFree ( UInteger handle ) {
        AssertV( !bassHandle, "BASS is not initialized." );
        BASS_MusicFree( handle );
    }
    Sub SampleFree ( UInteger handle ) {
        AssertV( !bassHandle, "BASS is not initialized." );
        BASS_SampleFree( handle );
    }
    Sub Free () {
        if ( bassHandle == Nothing )
            return;
        Integer isFreed = BASS_Free();
        if ( isFreed == 1 )
            dlclose( bassHandle );
    }
}