# Add to this list all files related to sim library
add_library(sim
progargs.hpp
progargs.cpp
grid.cpp
grid.hpp
block.cpp
block.hpp
particle.hpp
particle.cpp
constants.hpp
utils.cpp
utils.hpp
simulation.hpp
simulation.cpp
)
# Use this line only if you have dependencies from sim to GSL
target_link_libraries(sim PRIVATE Microsoft.GSL::GSL)
