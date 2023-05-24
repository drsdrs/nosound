cmake_minimum_required(VERSION 3.5)

set( projectName nosound )

project( ${projectName} )

set( SOURCES  src/nosound.c )
set( BINARY_DIR ../ )

include_directories( include ) 
include_directories( ext/SDL_inprint ) 


#add_compile_options(-fsanitize=address)
#add_link_options(-fsanitize=address)


add_library( alsa           STATIC src/audio/alsa.c)
add_library( beeper         STATIC src/audio/beeper.c )
add_library( waves          STATIC src/waves.c)

add_library( prgManager     STATIC src/prgManager.c )
add_library( looper         STATIC src/loopManager.c)

add_library( tv             STATIC src/video/tv.c)

add_library( path           STATIC src/utils/path.c)
add_library( string         STATIC src/utils/stringHelper.c)
add_library( prg            STATIC src/utils/prg.c)
add_library( file           STATIC src/utils/file.c)
add_library( term           STATIC src/utils/term.c)

add_library( vect           STATIC src/classes/vect.c)
add_library( color12        STATIC src/classes/color12.c)

add_library( SDL2_inprint   STATIC ext/SDL_inprint/inprint2.c)


set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${BINARY_DIR} )
add_executable( ${projectName} ${SOURCES} )

target_link_libraries( ${projectName}
    PRIVATE
    prgManager
    looper
    tv
	m
    SDL2
    asound
    term
    waves
    path
    string
    vect
    prg
    file
	SDL2_inprint
	beeper
    alsa
    color12
)
