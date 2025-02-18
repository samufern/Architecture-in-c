add_executable(ftest ftest.cpp)

include_directories(${CMAKE_SOURCE_DIR}/sim)

target_link_libraries(ftest
PRIVATE
sim
GTest::gtest_main
Microsoft.GSL::GSL)

include(GoogleTest)
gtest_discover_tests(ftest)

set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -march=native")