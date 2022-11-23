include( FetchContent )

FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/16f637fbf4ffc3f7a01fa4eceb7906634565242f.zip
)

FetchContent_MakeAvailable( googletest )

enable_testing()

file( GLOB_RECURSE sources ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

add_executable(
        gtest_nplib
        ${sources}
)
target_link_libraries(
        gtest_nplib
        gtest_main
)
target_include_directories(
        gtest_nplib
        PUBLIC
        ${CMAKE_HOME_DIRECTORY}/include
)

include( GoogleTest )
gtest_discover_tests( gtest_nplib )
