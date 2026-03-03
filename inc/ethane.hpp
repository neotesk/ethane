/*
    Ethane, a Research Project for hacking GoldSrc games.
    Open-Source, Public Domain.

    2026 neotesk.
*/

/*
    This is Ethane's main header. It provides types, helper
    functions and many more.

    Naming convention:
    - Global functions and macros must be in camel case.
    - Functions in namespaces must be in pascal case.
    - Namespace names must be in pascal case.
    - Custom macros (to make them distinct compared to original)
      must be in pascal case.
    - Type names must be pascal case.
    - Default type value names must be pascal case.
*/

#pragma once

#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>

#include <tessil/ordered_map.hpp>

// ---[ Definitions ]---------------------------------------------------------
#define BANNER "\x1b[1;3m" PROJECT_NAME "\x1b[0m"
// ---------------------------------------------------------------------------

// ---[ Macros ]--------------------------------------------------------------
#define Constructor void __attribute__((constructor))
#define Destructor void __attribute__((destructor))

#ifdef E_DEBUG
#define debug( format, ... ) \
    printf( BANNER ": " format "\n", ##__VA_ARGS__ )
#else
#define debug( format, ... )
#endif

#define info( format, ... ) \
    printf( BANNER ": " format "\n", ##__VA_ARGS__ )

#define Assert( cond, message, ... ) \
    if ( cond ) { \
        debug( message, ##__VA_ARGS__ ); \
        return false; \
    }
#define AssertV( cond, message, ... ) \
    if ( cond ) { \
        debug( message, ##__VA_ARGS__ ); \
        return; \
    }
#define Might( cond, message, ... ) \
    if ( cond ) debug( message, ##__VA_ARGS__ )
// ---------------------------------------------------------------------------

// ---[ Generic Types ]-------------------------------------------------------
typedef void *Pointer;
typedef void Sub;

typedef unsigned long ULong;
typedef uint32_t  UInt;
typedef uint32_t  UInteger;
typedef uintptr_t UIntPtr;
typedef uint64_t  UInt64;
typedef uint32_t  UInt32;
typedef uint16_t  UInt16;
typedef uint8_t   UInt8;
typedef uint8_t   Byte;

typedef long     Long;
typedef int32_t  Int;
typedef int32_t  Integer;
typedef intptr_t IntPtr;
typedef int64_t  Int64;
typedef int32_t  Int32;
typedef int16_t  Int16;
typedef int8_t   Int8;
typedef int8_t   SByte;

typedef float  Single;
typedef double Double;
typedef bool   Boolean;

typedef std::string String;
typedef const char  *CString;
typedef char        *CharArray;

template <typename T>
using Array = std::vector<T>;

template <typename K, typename V>
using Dictionary = tsl::ordered_map<K, V>;

static inline const Boolean        True    = true;
static inline const Boolean        False   = false;
static inline const std::nullptr_t Nothing = nullptr;
// ---------------------------------------------------------------------------