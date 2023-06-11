cmake_minimum_required(VERSION 3.5)

set( projectName nosound )

project( ${projectName} )

set( SOURCES  src/nosound.c )
set( BINARY_DIR ../ )

include_directories( include )
include_directories( ext )
include_directories( ext/SDL_inprint )

#add_compile_options(-fsanitize=address)
#add_link_options(-fsanitize=address)

add_library( prgManager     STATIC src/prgManager.c )
add_library( loopManager    STATIC src/loopManager.c)
add_library( spriteManager  STATIC src/spriteManager.c)

add_library( path           STATIC src/utils/path.c)
add_library( string         STATIC src/utils/stringHelper.c)
add_library( prg            STATIC src/utils/prg.c)
add_library( file           STATIC src/utils/file.c)
add_library( term           STATIC src/utils/term.c)

add_library( Vect2          STATIC src/classes/Vect2.c)
add_library( Color12        STATIC src/classes/Color12.c)

add_library( SDL2_inprint   STATIC ext/SDL_inprint/inprint2.c)


set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${BINARY_DIR} )
add_executable( ${projectName} ${SOURCES} )
target_compile_options(${projectName} PRIVATE -Wall -Wextra -Wpedantic -Werror)

target_link_libraries( ${projectName}
    PRIVATE
    m
    loopManager
    spriteManager
    prgManager
    term
    path
    string
    prg
    file
    Color12
    Vect2
)
