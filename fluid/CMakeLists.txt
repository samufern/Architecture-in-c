add_executable(fluid fluid.cpp)
target_include_directories(fluid PRIVATE ../sim)
target_link_libraries(fluid sim)