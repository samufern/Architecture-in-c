// fluid.cpp
#include "block.hpp"
#include "constants.hpp"
#include "grid.hpp"
#include "particle.hpp"
#include "progargs.hpp"
#include "simulation.hpp"
#include "utils.hpp"

#include <chrono>
#include <iostream>
#include <string>
#include <vector>

int main(int argc, char * argv[]) {
  std::vector<std::string> args(argv, argv + argc);
  if (args.size() != 4) {
    std::cerr << "Uso: " << args[0]
              << " <iteraciones> <archivo_entrada>.fld <archivo_salida>.fld\n";
    return 1;
  }
  const int iterations = std::stoi(args[1]);
  Header header{};
  std::vector<Particle> particles;
  if (!readInputFile(args[2], header, particles)) {
    std::cerr << "Error al leer el archivo de entrada.\n";
    return -3;
  }
  const int particleCount     = header.np;
  const int fileParticleCount = static_cast<int>(particles.size());
  if (!ProgArgs::validate(args, iterations, particleCount, fileParticleCount)) { return 1; }
  runSimulation(iterations, args[2], args[3]);
  return 0;
}