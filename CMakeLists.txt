cmake_minimum_required( VERSION 3.21 )

project( natprolib VERSION 2.3.1 )

option( CMAKE_EXPORT_COMPILE_COMMANDS ON )

set( CMAKE_CXX_STANDARD          20   )
set( CMAKE_CXX_STANDARD_REQUIRED True )

add_compile_options( -Wall -Wextra -pedantic -Werror )

if( NPL_DEBUG )
        add_compile_options( -O0 )
        add_compile_definitions( NPL_DEBUG )
        add_link_options( -fsanitize=address -fsanitize=undefined )
elseif( NPL_RELEASE )
        add_compile_options( -O3 )
        add_compile_definitions( NPL_RELEASE )
endif()

if( NPL_HAS_ASAN )
        add_compile_definitions( NPL_HAS_ASAN )
endif()

if( GTEST )
        add_subdirectory( tests )
endif()
if( GBENCH )
        add_subdirectory( bench )
endif()

add_executable( nplib main.cpp )

target_include_directories( nplib PUBLIC include )
