// block.hpp
#pragma once

#include "grid.hpp"
#include "particle.hpp"

#include <array>

std::array<int, 3> getBlockIndices(Particle const & particle, GridSize const & blockSize,
                                   GridSize const & gridDimensions);
void repositionParticle(Particle & particle, GridSize const & blockSize,
                        GridSize const & gridDimensions);
