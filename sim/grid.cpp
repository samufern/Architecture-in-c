// grid.cpp
#include "grid.hpp"

#include "constants.hpp"

#include <cmath>

void calculateNumberOfBlocks(float height, GridSize const & domainSize, GridSize & blocks) {
  if (std::abs(height) < SMALL_NUMBER) { return; }

  const float inv_height = 1.0F / height;
  blocks.nx        = std::round((domainSize.nx - xmin) * inv_height);
  blocks.ny        = std::round((domainSize.ny - ymin) * inv_height);
  blocks.nz        = std::round((domainSize.nz - zmin) * inv_height);
}

void calculateBlockSize(GridSize const & numberOfBlocks, GridSize const & domainSize,
                        GridSize & blockSize) {
  if (std::abs(numberOfBlocks.nx) < SMALL_NUMBER || std::abs(numberOfBlocks.ny) < SMALL_NUMBER ||
      std::abs(numberOfBlocks.nz) < SMALL_NUMBER) {
    return;
  }

  const float inv_nx = 1.0F / numberOfBlocks.nx;
  const float inv_ny = 1.0F / numberOfBlocks.ny;
  const float inv_nz = 1.0F / numberOfBlocks.nz;
  blockSize.nx = (domainSize.nx - xmin) * inv_nx;
  blockSize.ny = (domainSize.ny - ymin) * inv_ny;
  blockSize.nz = (domainSize.nz - zmin) * inv_nz;
}
