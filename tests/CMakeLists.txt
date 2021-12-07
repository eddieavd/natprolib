if( CTEST )
	add_library( test_meta_range_queries test_meta_range_queries.cpp )

elseif( GTEST )
	include( FetchContent )
	FetchContent_Declare(
		googletest
		URL https://github.com/google/googletest/archive/16f637fbf4ffc3f7a01fa4eceb7906634565242f.zip
	)

	FetchContent_MakeAvailable( googletest )

	enable_testing()

	add_executable(
		gtest_rq
		gtest_range_queries.cpp
	)
	target_link_libraries(
		gtest_rq
		gtest_main
	)

	include( GoogleTest )
	gtest_discover_tests( gtest_rq )
endif()
