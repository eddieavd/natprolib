cmake_minimum_required( VERSION 3.21 )

project( natprolib_test VERSION 1.0 )

option( CTEST "Build dumb test suite" OFF )
option( GTEST "Build gtest suite"     OFF )
option( MAIN  "Build main.cpp"        OFF )
option( CMAKE_EXPORT_COMPILE_COMMANDS OFF )

#configure_file( config.h.in config.h )

set( CMAKE_CXX_STANDARD          17   )
set( CMAKE_CXX_STANDARD_REQUIRED True )
add_compile_options( -Wall -Wextra -pedantic -Werror )
add_link_options( -fsanitize=address -fsanitize=undefined )

add_subdirectory( tests   )
add_subdirectory( include )

if( CTEST )
	list( APPEND EXTRA_LIBS test_range_queries )
	list( APPEND EXTRA_INCLUDES "$(PROJECT_SOURCE_DIR)/tests" )
elseif( MAIN )
	list( APPEND EXTRA_LIBS natprolib )
	list( APPEND EXTRA_INCLUDES "$(PROJECT_SOURCE_DIR)/include" )
endif()

add_executable( natprolib_test main.cpp )

target_link_libraries( natprolib_test PUBLIC ${EXTRA_LIBS} )

target_include_directories( natprolib_test PUBLIC
	"$(PROJECT_BINARY_DIR)"
	${EXTRA_INCLUDES}
)
