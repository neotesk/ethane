/******************************************************************************

             ▓█████ ▄▄▄█████▓ ██░ ██  ▄▄▄       ███▄    █ ▓█████
             ▓█   ▀ ▓  ██▒ ▓▒▓██░ ██▒▒████▄     ██ ▀█   █ ▓█   ▀
             ▒███   ▒ ▓██░ ▒░▒██▀▀██░▒██  ▀█▄  ▓██  ▀█ ██▒▒███
             ▒▓█  ▄ ░ ▓██▓ ░ ░▓█ ░██ ░██▄▄▄▄██ ▓██▒  ▐▌██▒▒▓█  ▄
             ░▒████▒  ▒██▒ ░ ░▓█▒░██▓ ▓█   ▓██▒▒██░   ▓██░░▒████▒
             ░░ ▒░ ░  ▒ ░░    ▒ ░░▒░▒ ▒▒   ▓▒█░░ ▒░   ▒ ▒ ░░ ▒░ ░
              ░ ░  ░    ░     ▒ ░▒░ ░  ▒   ▒▒ ░░ ░░   ░ ▒░ ░ ░  ░
                ░     ░       ░  ░░ ░  ░   ▒      ░   ░ ░    ░

             Ethane, a Research Project for hacking GoldSrc games.
               Open-Source & Public Domain. Free as in freedom.

             This is a research project (helped me learn some C++)
             and I wanted to share it to the interweb. It's a hack
             library made specifically for those who want to learn
             how this stuff works. I've wrote lots of comments and
                tried my best to *overexplain* everything here.

                                   Have fun!
                             ..: 2026 neotesk. :..

******************************************************************************/

/*
    This is the injector. Please note that this code is
    written for x86 32-bit architecture.
*/

#include <ethane.hpp>
#include <injector/pinject.h>

#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// Custom assertion ONLY for injector
#define AssertP( cond, message, ... ) \
    if ( cond ) { \
        info( "Fatal Error! " message, ##__VA_ARGS__ ); \
        safeExit(); \
    }

// Some globals
pinject_data session;

// This is the safe exit
Sub safeExit () {
    if ( session.valid )
        pinject_finish( &session );
    exit( EXIT_FAILURE );
}

// This is where the magic happens
Boolean inject ( pinject_data data, Boolean uninject = False ) {
    // Get the library path
    char absLibPath[ BUFFER_MAX ];
    char binPath[ BUFFER_MAX ];

    // Get the process directory
    Long len = readlink( "/proc/self/exe", binPath, sizeof( binPath ) - 1 );
    AssertP( len == -1, "Failed to read injector's execution path." );
    binPath[ len ] = '\0';

    // Remove the last section so we can get the parent directory
    // that our exec resides in.
    char *lastSlash = strrchr( binPath, '/' );
    if ( lastSlash )
        *lastSlash = '\0';

    // Combine 'em together
    snprintf( absLibPath, BUFFER_MAX,
        "%s/%s", binPath, TARGET_LIBRARY );

    // Call dlopen
    Pointer result = pinject_dlopen( &data, absLibPath, uninject ? 5 : 1 );

    // Check if there's any error
    if ( !result ) {
        char message[ BUFFER_MAX ];
        message[ 0 ] = '\0';
        pinject_dlerror( &data, message, BUFFER_MAX );
        if ( message[ 0 ] != '\0' )
            return false;
    }

    // If uninject is true, call dlclose twice
    if ( uninject ) {
        pinject_dlclose( &data, result );
        pinject_dlclose( &data, result );
    }

    // Return the result of dlopen
    return result;
}

Integer main ( Integer argc, CString *argv ) {
    info( "Welcome to " PROJECT_NAME "'s Injector! Build " PROJECT_VERSION );
    debug( "Visit %s for more info.", PROJECT_SOURCE );

    if ( argc < 2 ) {
        info( "Usage: %s <inject|eject|reload> [debug]", argv[ 0 ] );
        exit( EXIT_FAILURE );
    }

    CString command = argv[ 1 ];
    pinject_debug_enabled = argc > 2
        ? strcmp( argv[ 2 ], "debug" ) == 0 : false;

    // Do we have root privileges?
    AssertP( geteuid() != 0, "Please run this program as root." );

    // Get PID of the executable
    Integer hlPid = pinject_pidof( "hl_linux" );
    AssertP( hlPid == -1, "Half-Life (hl_linux) is not running." );

    // Begin the pinject session
    session = pinject_begin( hlPid );

    if ( !strcmp( command, "inject" ) ) {
        AssertP( pinject_getmodbase( hlPid, TARGET_LIBRARY ),
            "The cheat is already injected. Skipping." );
        inject( session, false );
        info( "Injection successful! Have fun." );
    } else if ( !strcmp( command, "eject" ) ) {
        AssertP( !pinject_getmodbase( hlPid, TARGET_LIBRARY ),
            "The cheat is already ejected. Skipping." );
        inject( session, true );
        info( "Ejection successful! Have a great night." );
    } else if ( !strcmp( command, "reload" ) ) {
        if ( !pinject_getmodbase( hlPid, TARGET_LIBRARY ) )
            inject( session, false );
        else {
            inject( session, true );
            inject( session, false );
        }
        info( "Reloading successful! Have fun." );
    }

    // Finish pinject session
    pinject_finish( &session );
    return 0;
}