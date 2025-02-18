// simulation.cpp
#include "simulation.hpp"

#include "block.hpp"
#include "constants.hpp"
#include "grid.hpp"
#include "particle.hpp"
#include "progargs.hpp"
#include "utils.hpp"

#include <chrono>
#include <iostream>
#include <string>
#include <vector>

void runSimulation(int iterations, std::string const & inputFile, std::string const & outputFile) {
  Header header{};
  std::vector<Particle> particles;
  readInputFile(inputFile, header, particles);
  const float height       = calculateSmoothingLength(r, header.ppm);
  const float mass         = calculateParticleMass(rho, header.ppm);
  GridSize numBlocks = calculateNumberOfBlocks(height);
  GridSize blockSize = calculateBlockSize(numBlocks);
  const SimulationParameters simParams{
    iterations, {   height,      mass},
     {numBlocks, blockSize}
  };
  simulationWithIterations(particles, simParams);
  writeParticlesToFile(outputFile, header, particles);
  const SalidaParameters salidaParams{
    header.np, header.ppm, {   height,      mass},
      {numBlocks, blockSize}
  };
  salida(salidaParams);
  std::cout << "Simulacion realizada con exito.\n";
}
