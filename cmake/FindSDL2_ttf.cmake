# Locate SDL2_ttf library
# This module defines
# SDL2_TTF_LIBRARY, the name of the library to link against
# SDL2_TTF_FOUND, if false, do not try to link to SDL2_ttf
# SDL2_TTF_INCLUDE_DIR, where to find SDL_ttf.h
#
# This module responds to the the flag:
# SDL2_TTF_BUILDING_LIBRARY
# If this is defined, then no SDL2_ttf_main will be linked in because
# only applications need main().
# Otherwise, it is assumed you are building an application and this
# module will attempt to locate and set the the proper link flags
# as part of the returned SDL2_TTF_LIBRARY variable.
#
# Don't forget to include SDL2TTF.h in your source file:
# #include "SDL_ttf.h"
#
# Additional Note: If you see an empty SDL2_TTF_LIBRARY_TEMP in your configuration
# and no SDL2_TTF_LIBRARY, it means CMake did not find your SDL2_ttf library
# (SDL2_ttf.dll, libsdl2_ttf.so, SDL2_ttf.framework, etc).
# Set SDL2_TTF_LIBRARY_TEMP to point to your SDL2_ttf library, and configure again.
# Similarly, if you see an empty SDL2_TTF_LIBRARY_TEMP, it means CMake did not find
# your SDL2_ttf library or any of its dependencies.

SET(SDL2_TTF_SEARCH_PATHS
    ~/Library/Frameworks
    /Library/Frameworks
    /usr/local
    /usr
    /sw # Fink
    /opt/local # DarwinPorts
    /opt/csw # Blastwave
    /opt
)

FIND_PATH(SDL2_TTF_INCLUDE_DIR SDL_ttf.h
    HINTS
    $ENV{SDL2TTFDIR}
    PATH_SUFFIXES include/SDL2 include
    PATHS ${SDL2_TTF_SEARCH_PATHS}
)

if (CMAKE_SIZEOF_VOID_P EQUAL 8)
    set(PATH_SUFFIXES lib64 lib/x64 lib)
else()
    set(PATH_SUFFIXES lib/x86 lib)
endif()

FIND_LIBRARY(SDL2_TTF_LIBRARY
    NAMES SDL2_ttf
    HINTS
    $ENV{SDL2TTFDIR}
    PATH_SUFFIXES ${PATH_SUFFIXES}
    PATHS ${SDL2_TTF_SEARCH_PATHS}
)

SET(SDL2_TTF_FOUND "NO")
IF(SDL2_TTF_LIBRARY AND SDL2_TTF_INCLUDE_DIR)
    SET(SDL2_TTF_FOUND "YES")
ENDIF(SDL2_TTF_LIBRARY AND SDL2_TTF_INCLUDE_DIR)
