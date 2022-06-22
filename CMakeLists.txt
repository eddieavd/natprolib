cmake_minimum_required( VERSION 3.22 )

project( natprolib_test VERSION 1.0 )

option( METATEST "Test meta_rq"       OFF )
option( GTEST    "Build gtest suite"  OFF )
option( GBENCH   "Build benchmarks"   OFF )
option( CMAKE_EXPORT_COMPILE_COMMANDS ON  )

set( CMAKE_CXX_STANDARD          20   )
set( CMAKE_CXX_STANDARD_REQUIRED True )

add_compile_options( -Wall -Wextra -pedantic -Werror -O3 )
#add_link_options( -fsanitize=address -fsanitize=undefined )

if( NPL_DEBUG )
	add_compile_options( -DNPL_DEBUG )
endif()

add_subdirectory( include )
add_subdirectory( tests   )
add_subdirectory( bench   )

if( METATEST )
	list( APPEND EXTRA_LIBS test_meta_range_queries )
	list( APPEND EXTRA_INCLUDES "$(PROJECT_SOURCE_DIR)/tests" )
endif()

list( APPEND EXTRA_LIBS natprolib )
list( APPEND EXTRA_INCLUDES "$(PROJECT_SOURCE_DIR)/include" )

add_executable( natprolib_test main.cpp )

target_link_libraries( natprolib_test PUBLIC ${EXTRA_LIBS} )

target_include_directories( natprolib_test PUBLIC
	"$(PROJECT_BINARY_DIR)"
	${EXTRA_INCLUDES}
)
