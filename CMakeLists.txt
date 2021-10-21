cmake_minimum_required( VERSION 3.21 )

project( natprolib_test VERSION 1.0 )

option( CTEST "Compile test suite" OFF )
option( GTEST "Run gtest suite"    OFF )

configure_file( config.h.in config.h )

set( CMAKE_CXX_STANDARD          17   )
set( CMAKE_CXX_STANDARD_REQUIRED True )

if( TEST )
	add_subdirectory( tests )
	list( APPEND EXTRA_LIBS test_range_queries )
	list( APPEND EXTRA_INCLUDES "$(PROJECT_SOURCE_DIR)/tests" )
endif()

if( GTEST )

endif()

add_executable( natprolib_test main.cpp )

target_link_libraries( natprolib_test PUBLIC ${EXTRA_LIBS} )

target_include_directories( natprolib_test PUBLIC
				"$(PROJECT_BINARY_DIR)"
				${EXTRA_INCLUDES}
				)
