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

#include <ethane.hpp>
#include <resource.hpp>
#include <bass/bass.hpp>

Constructor ethaneStart () {
    debug( "Greetings from %s %s!", PROJECT_NAME, PROJECT_VERSION );
    debug( "Visit %s for more info.", PROJECT_SOURCE );

    // Initialize the sound/music library that we are using
    Boolean i = BASS::Init( "res/libbass.so", -1, 44100 );
}

Destructor ethaneStop () {
    debug( "Goodbye, I go now :3" );

    // Deinitialize everything
    BASS::Free();
}