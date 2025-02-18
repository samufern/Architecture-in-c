add_executable(utest
utils_test.cpp
block_test.cpp
grid_test.cpp
progargs_test.cpp
particle_test.cpp
simulation_test.cpp)
# Library dependencies
target_link_libraries (utest
PRIVATE
sim
GTest::gtest_main
Microsoft.GSL::GSL)
# Discover all tests and add them to the test driver
include(GoogleTest)
gtest_discover_tests(utest)
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -march=native")
include_directories(${CMAKE_SOURCE_DIR}/sim)