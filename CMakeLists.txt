cmake_minimum_required(VERSION 3.5)

set( projectName nosound )

project( ${projectName} )
set( SOURCES  src/nosound.c )
set( BINARY_DIR ../ )

add_library( beeper STATIC lib/beeper/beeper.c )
add_library( prgManager STATIC lib/prgManager/prgManager.c )
add_library( tv STATIC lib/tv/tv.c)
add_library( SDL2_inprint STATIC lib/SDL_inprint/inprint2.c)
add_library( looper STATIC lib/looper/looper.c)
add_library( path STATIC lib/path/path.c)
add_library( string STATIC lib/string/string.c)
add_library( prg STATIC lib/prg/prg.c)
add_library( file STATIC lib/file/file.c)
add_library( alsa STATIC lib/alsa/alsa.c)
add_library( vect STATIC lib/vect/vect.c)
add_library( term STATIC lib/term/term.c)
add_library( waves STATIC lib/waves.c)

set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${BINARY_DIR} )
add_executable( ${projectName} ${SOURCES} )

target_link_libraries( ${projectName}
    PRIVATE
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
    looper
		tv
		SDL2_inprint
    prgManager
		beeper
    alsa
)
