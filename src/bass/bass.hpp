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

#pragma once

#include <ethane.hpp>

#define BASS_FileType_Path       0
#define BASS_FileType_Memory     1
#define BASS_FileType_MemoryCopy 3
#define BASS_FileType_Handle     4

#define BASS_OK                 0
#define BASS_ERROR_MEM          1
#define BASS_ERROR_FILEOPEN     2
#define BASS_ERROR_DRIVER       3
#define BASS_ERROR_BUFLOST      4
#define BASS_ERROR_HANDLE       5
#define BASS_ERROR_FORMAT       6
#define BASS_ERROR_POSITION     7
#define BASS_ERROR_INIT	        8
#define BASS_ERROR_START        9
#define BASS_ERROR_SSL          10
#define BASS_ERROR_REINIT       11
#define BASS_ERROR_TRACK        13
#define BASS_ERROR_ALREADY      14
#define BASS_ERROR_NOTAUDIO     17
#define BASS_ERROR_NOCHAN       18
#define BASS_ERROR_ILLTYPE      19
#define BASS_ERROR_ILLPARAM     20
#define BASS_ERROR_NO3D         21
#define BASS_ERROR_NOEAX        22
#define BASS_ERROR_DEVICE       23
#define BASS_ERROR_NOPLAY       24
#define BASS_ERROR_FREQ	        25
#define BASS_ERROR_NOTFILE      27
#define BASS_ERROR_NOHW         29
#define BASS_ERROR_EMPTY        31
#define BASS_ERROR_NONET        32
#define BASS_ERROR_CREATE       33
#define BASS_ERROR_NOFX         34
#define BASS_ERROR_NOTAVAIL     37
#define BASS_ERROR_DECODE       38
#define BASS_ERROR_DX           39
#define BASS_ERROR_TIMEOUT      40
#define BASS_ERROR_FILEFORM     41
#define BASS_ERROR_SPEAKER      42
#define BASS_ERROR_VERSION      43
#define BASS_ERROR_CODEC        44
#define BASS_ERROR_ENDED        45
#define BASS_ERROR_BUSY         46
#define BASS_ERROR_UNSTREAMABLE 47
#define BASS_ERROR_PROTOCOL     48
#define BASS_ERROR_DENIED       49
#define BASS_ERROR_FREEING      50
#define BASS_ERROR_CANCEL       51
#define BASS_ERROR_UNKNOWN      -1
#define BASS_ERROR_NOTLOADED    -2

#define BASS_MUSIC_RAMP         0x200
#define BASS_MUSIC_RAMP         0x200
#define BASS_SAMPLE_LOOP        0x004
#define BASS_SAMPLE_MONO        0x002
#define BASS_SAMPLE_8BITS       0x001

namespace BASS {
    Boolean Init ( CString path, Integer device, UInteger sampleRate );
    UInteger MusicLoad ( UInteger fileType, Pointer file, UInteger length,
        UInteger flags );
    UInteger MusicLoadFromMemory ( Pointer file, UInteger length,
        UInteger flags );
    UInteger MusicLoadFromFile ( Pointer file, UInteger flags );
    UInteger SampleLoad ( UInteger fileType, Pointer file, UInteger length,
        UInteger flags, UInteger max = 3 );
    UInteger SampleLoadFromMemory ( Pointer file, UInteger length,
        UInteger flags, UInteger max = 3 );
    UInteger SampleLoadFromFile ( Pointer file, UInteger flags,
        UInteger max = 3 );
    UInteger SampleGetChannel ( UInteger handle, Boolean onlyNew );
    Boolean ChannelPlay ( UInteger handle, Boolean restart );
    Boolean SamplePlay ( UInteger sampleHandle );
    Boolean ChannelStop ( UInteger handle );
    Sub MusicFree ( UInteger handle );
    Sub SampleFree ( UInteger handle );
    Sub Free ();
}