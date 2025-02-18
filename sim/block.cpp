// block.cpp
#include "block.hpp"

#include "constants.hpp"

#include <cmath>

std::array<int, 3> getBlockIndices(Particle const & particle, GridSize const & blockSize,
                                   GridSize const & gridDimensions) {
  return {std::max(0, std::min(static_cast<int>((particle.px - xmin) / blockSize.nx), static_cast<int>(gridDimensions.nx) - 1)),
          std::max(0, std::min(static_cast<int>((particle.py - ymin) / blockSize.ny), static_cast<int>(gridDimensions.ny) - 1)),
          std::max(0, std::min(static_cast<int>((particle.pz - zmin) / blockSize.nz), static_cast<int>(gridDimensions.nz) - 1))};
}

void repositionParticle(Particle & particle, GridSize const & blockSize,
                        GridSize const & gridDimensions) {
  auto indices = getBlockIndices(particle, blockSize, gridDimensions);

  const float baseX = xmin + static_cast<float>(indices[0]) * blockSize.nx - SMALL_NUMBER;
  const float baseY = ymin + static_cast<float>(indices[1]) * blockSize.ny - SMALL_NUMBER;
  const float baseZ = zmin + static_cast<float>(indices[2]) * blockSize.nz - SMALL_NUMBER;
  const float maxX  = baseX + blockSize.nx;
  const float maxY  = baseY + blockSize.ny;
  const float maxZ  = baseZ + blockSize.nz;

  particle.px = (particle.px < baseX) ? baseX : ((particle.px > maxX) ? maxX : particle.px);
  particle.py = (particle.py < baseY) ? baseY : ((particle.py > maxY) ? maxY : particle.py);
  particle.pz = (particle.pz < baseZ) ? baseZ : ((particle.pz > maxZ) ? maxZ : particle.pz);
}
