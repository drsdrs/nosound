cmake_minimum_required(VERSION 3.5)

set( projectName main )

project( ${projectName} )
set( SOURCES
    prg_wrapper.c
    ../../../src/audio/alsaHelper.c
    ../../../src/audio/beeper.c
    ../../../src/waves.c

    ../../../src/prgManager.c
    ../../../src/loopManager.c
    ../../../src/spriteManager.c

    ../../../src/utils/path.c
    ../../../src/utils/stringHelper.c
    ../../../src/utils/prg.c
    ../../../src/utils/file.c
    ../../../src/utils/term.c

    ../../../src/classes/Vect2.c
    ../../../src/classes/Color12.c
    ../../../ext/SDL_inprint/inprint2.c
)

set( BINARY_DIR ../ )

include_directories( ../../../include )
include_directories( ../../../ext )
include_directories( ../../../ext/SDL_inprint )

find_package(SDL2 CONFIG REQUIRED)

#add_compile_options(-fsanitize=address)
#add_link_options(-fsanitize=address)

add_library( alsaHelper     STATIC ../../../src/audio/alsaHelper.c)
add_library( beeper         STATIC ../../../src/audio/beeper.c )
add_library( waves          STATIC ../../../src/waves.c)

add_library( prgManager     STATIC ../../../src/prgManager.c )
add_library( loopManager    STATIC ../../../src/loopManager.c)
add_library( spriteManager  STATIC ../../../src/spriteManager.c)

add_library( sdlHelper      STATIC ../../../src/video/sdlHelper.c)

add_library( path           STATIC ../../../src/utils/path.c)
add_library( string         STATIC ../../../src/utils/stringHelper.c)
add_library( prg            STATIC ../../../src/utils/prg.c)
add_library( file           STATIC ../../../src/utils/file.c)
add_library( term           STATIC ../../../src/utils/term.c)

add_library( Vect2          STATIC ../../../src/classes/Vect2.c)
add_library( Color12        STATIC ../../../src/classes/Color12.c)

add_library( SDL2_inprint   STATIC ../../../ext/SDL_inprint/inprint2.c)

set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${BINARY_DIR} )
add_executable( ${projectName} ${SOURCES} )
#target_compile_options(${projectName} PRIVATE -Wall -Wextra -Wpedantic -Werror)
set_target_properties(${projectName} PROPERTIES LINKER_LANGUAGE CXX)

target_link_libraries( ${projectName}
    PRIVATE
    m
    asound
    pthread
    SDL2
    SDL2_image
    alsaHelper
    beeper
    prgManager
    loopManager
    spriteManager
    sdlHelper
    prg
    waves
    term
    path
    string
    Vect2
    file
    SDL2_inprint
    Color12
)
