add_executable(
        gbench_nplib
        gbench_nplib.cpp
)

find_package( benchmark REQUIRED )

target_link_libraries( gbench_nplib PUBLIC benchmark::benchmark      )
target_link_libraries( gbench_nplib PUBLIC benchmark::benchmark_main )

target_include_directories(
        gbench_nplib
        PUBLIC
        ${CMAKE_HOME_DIRECTORY}/include
)
