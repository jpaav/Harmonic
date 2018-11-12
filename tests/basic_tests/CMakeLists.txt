set(gtest_SOURCE_DIR ../libs/gtest-1.8.0/googletest)
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

add_executable(runBasicHarmonicTests
        physics_object.cpp)

target_link_libraries(runBasicHarmonicTests gtest gtest_main)
target_link_libraries(runBasicHarmonicTests Harmonic)