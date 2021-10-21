cmake_minimum_required( VERSION 3.21 )

project( natprolib_test VERSION 1.0 )

option( CTEST "Build dumb test suite" OFF )
option( GTEST "Build gtest suite"     OFF )
option( MAIN  "Build main.cpp"        OFF )

configure_file( config.h.in config.h )

set( CMAKE_CXX_STANDARD          17   )
set( CMAKE_CXX_STANDARD_REQUIRED True )

if( CTEST )
	add_subdirectory( tests )
	list( APPEND EXTRA_LIBS test_range_queries )
	list( APPEND EXTRA_INCLUDES "$(PROJECT_SOURCE_DIR)/tests" )
elseif( GTEST )
	add_subdirectory( tests )
endif()

if( CTEST OR MAIN )
	add_subdirectory( include )
	add_executable( natprolib_test main.cpp )

	list( APPEND EXTRA_LIBS natprolib )
	list( APPEND EXTRA_INCLUDES "$(PROJECT_SOURCE_DIR)/include" )

	target_link_libraries( natprolib_test PUBLIC ${EXTRA_LIBS} )

	target_include_directories( natprolib_test PUBLIC
		"$(PROJECT_BINARY_DIR)"
		${EXTRA_INCLUDES}
	)
endif()