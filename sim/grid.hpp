// GRID_HPP
#pragma once

#include "constants.hpp"

// Estructura para representar el tamaño de la malla de simulación
struct GridSize {
    float nx, ny, nz;

    // Constructor por defecto
    GridSize() : nx(0), ny(0), nz(0) { }

    // Constructor con parámetros
    GridSize(int x, int y, int z) : nx(x), ny(y), nz(z) { }
};

// Funciones para calcular el número y tamaño de bloques en la malla
void calculateNumberOfBlocks(float height, GridSize const & domainSize, GridSize & blocks);
void calculateBlockSize(GridSize const & numberOfBlocks, GridSize const & domainSize,
                        GridSize & blockSize);
