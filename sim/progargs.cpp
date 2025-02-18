// progargs.cpp
#include "progargs.hpp"

#include "utils.hpp"

#include <array>
#include <cstdint>
#include <cstring>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

constexpr size_t const ARG_COUNT = 4;

ProgArgs::ProgArgs(std::vector<std::string> const & args) : args(args) { }

bool ProgArgs::checkArgCount(std::vector<std::string> const & args) {
  if (args.size() != ARG_COUNT) {
    std::cerr << "Error: Incorrect number of arguments.\n";
    std::cerr << "Usage: " << args[0]
              << " <iterations> <input_filename>.fld <output_filename>.fld\n";
    exit(ERROR_INCORRECT_ARG_COUNT);
  }
  return true;
}

bool ProgArgs::checkFirstArg(std::vector<std::string> const & args) {
  if (!isInteger(args[1])) {
    std::cerr << "Error: First argument ('" << args[1] << "') must be an integer.\n";
    exit(ERROR_INVALID_FIRST_ARG);
  }
  return true;
}

bool ProgArgs::checkIterations(int iterations) {
  if (iterations < 0) {
    std::cerr << "Error: Number of iterations (" << iterations << ") cannot be negative.\n";
    exit(ERROR_NEGATIVE_ITERATIONS);
  }
  return true;
}

bool ProgArgs::checkInputFile(std::vector<std::string> const & args) {
  const std::ifstream infile(args[2]);
  if (!infile.good()) {
    std::cerr << "Error: Cannot open " << args[2] << " for reading.\n";
    exit(ERROR_INPUT_FILE_OPEN);
  }
  return true;
}

bool ProgArgs::checkOutputFile(std::vector<std::string> const & args) {
  const std::ofstream outfile(args[3]);
  if (!outfile.good()) {
    std::cerr << "Error: Cannot open " << args[3] << " for writing.\n";
    exit(ERROR_OUTPUT_FILE_OPEN);
  }
  return true;
}

bool ProgArgs::checkParticleCount(int particleCount) {
  if (particleCount <= 0) {
    std::cerr << "Error: Invalid number of particles: " << particleCount << ".\n";
    exit(ERROR_INVALID_PARTICLE_COUNT);
  }
  return true;
}

bool ProgArgs::checkParticleCountMatch(int headerCount, int fileCount) {
  if (headerCount != fileCount) {
    std::cerr << "Error: Number of particles mismatch. Header: " << headerCount
              << ", Found: " << fileCount << ".\n";
    exit(ERROR_INVALID_PARTICLE_COUNT);
  }
  return true;
}

bool ProgArgs::validate(std::vector<std::string> const & args, int iterations, int particleCount,
                        int fileParticleCount) {
  if (args.empty()) {
    std::cerr << "Error: No arguments provided.\n";
    exit(ERROR_INCORRECT_ARG_COUNT);
  }
  checkArgCount(args);
  checkFirstArg(args);
  checkIterations(iterations);
  checkInputFile(args);
  checkOutputFile(args);
  checkParticleCount(particleCount);
  checkParticleCountMatch(particleCount, fileParticleCount);
  return true;
}
