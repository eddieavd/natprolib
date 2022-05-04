if( GBENCH )
	add_executable(
		gbench_rq
		gbench_range_queries.cpp
	)

	find_package( benchmark REQUIRED )

	target_link_libraries( gbench_rq PUBLIC benchmark::benchmark      )
	target_link_libraries( gbench_rq PUBLIC benchmark::benchmark_main )
endif()
