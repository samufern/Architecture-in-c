// utils.cpp
#include "utils.hpp"

#include "block.hpp"
#include "particle.hpp"

#include <array>
#include <chrono>
#include <cmath>
#include <cstring>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <cctype>
#include <string>

constexpr size_t ParticleDataSize = sizeof(float) * 9;

bool readHeader(std::ifstream & inFile, Header & header) {
  std::array<char, sizeof(header.ppm)> buffer{};
  inFile.read(buffer.data(), sizeof(header.ppm));
  memcpy(&header.ppm, buffer.data(), sizeof(header.ppm));

  inFile.read(buffer.data(), sizeof(header.np));
  memcpy(&header.np, buffer.data(), sizeof(header.np));

  return !inFile.fail();
}

bool readParticleData(std::ifstream & inFile, std::vector<Particle> & particles, int np) {
  std::vector<char> buffer(ParticleDataSize * np);
  inFile.read(buffer.data(), static_cast<std::streamsize>(ParticleDataSize * np));

  if (inFile.fail()) {
    std::cerr << "Error reading particles from file.\n";
    return false;
  }

  particles.clear();
  particles.reserve(np);
  for (int i = 0; i < np; ++i) {
    Particle particle{};
    memcpy(&particle, &buffer[i * ParticleDataSize], ParticleDataSize);
    particles.push_back(particle);
  }

  return true;
}

bool readParticlesFromFile(std::ifstream & inFile, Header & header,
                           std::vector<Particle> & particles) {
  if (!readHeader(inFile, header)) {
    std::cerr << "Error reading header from file.\n";
    return false;
  }
  return readParticleData(inFile, particles, header.np);
}

bool writeParticlesToFile(std::string const & filename, Header const & header,
                          std::vector<Particle> const & particles) {
  std::ofstream outFile(filename, std::ios::binary);
  if (!outFile.is_open()) {
    std::cerr << "Could not open output file: " << filename << '\n';
    return false;
  }

  std::array<char, sizeof(Header)> headerBuffer{};
  memcpy(headerBuffer.data(), &header, sizeof(Header));
  outFile.write(headerBuffer.data(), sizeof(Header));

  std::vector<char> particleBuffer(particles.size() * sizeof(Particle));
  for (size_t i = 0; i < particles.size(); ++i) {
    memcpy(&particleBuffer[i * sizeof(Particle)], &particles[i], sizeof(Particle));
  }
  outFile.write(particleBuffer.data(), static_cast<std::streamsize>(particleBuffer.size()));

  outFile.close();
  return true;
}

bool readInputFile(const std::string & filename, Header & header,
                   std::vector<Particle> & particles) {
  std::ifstream inFile(filename, std::ios::binary);
  if (!inFile.is_open()) {
    std::cerr << "Could not open input file: " << filename << '\n';
    exit(-3);
  }

  if (!readParticlesFromFile(inFile, header, particles)) { // Corrected function call
    exit(-3);
  }

  return true;
}

void updateParticles(std::vector<Particle> & particles, ParticleParameters params) {
  for (size_t i = 0; i < particles.size(); ++i) {
    Particle & particle = particles[i];
    repositionParticle(particle, params.blockSize, params.blocks);
    for (size_t j = i + 1; j < particles.size(); ++j) {
      Particle & particle2 = particles[j];
      updateParticlePair(particle, particle2, params.smoothingLength, params.mass);
    }

    processCollisions(particle);
    updateParticleMotion(particle);
  }
}

void updateParticlePair(Particle & particle1, Particle & particle2, float smoothingLength,
                        float mass) {
  updateDensity(particle1, particle2, smoothingLength);
  transformDensity(particle1, smoothingLength, mass);
  updateAcceleration(particle1, particle2, smoothingLength, mass);
}

void simulationWithIterations(std::vector<Particle> & particles, const SimulationParameters & params) {
  auto start = std::chrono::high_resolution_clock::now();

  const ParticleParameters particleParams = {params.parametros[0], params.parametros[1],
                                             params.bloques[0], params.bloques[1]}; // const added

  for (Particle & particle : particles) {
    initializeDensitiesAndAccelerations(particle);
  }

  for (int it = 0; it < params.iterations; ++it) {
    updateParticles(particles, particleParams);
  }

  auto finish = std::chrono::high_resolution_clock::now();
  const std::chrono::duration<double> elapsed = finish - start; // const added
  std::cout << "La función de simulacion tardó: " << elapsed.count() << " segundos.\n";
}


bool isInteger(const std::string &s) {
    if (s.empty()) {
        return false;
    }

    auto start = s.begin();
    if (s[0] == '-' || s[0] == '+') {
        start++;
        if (start == s.end()) {
            return false;
        }
    }

    return std::all_of(start, s.end(), ::isdigit);
}


bool salida(const SalidaParameters & params) {
    const auto length = params.parametros[0];
    const auto mass = params.parametros[1];
    const auto &blocks = params.bloques[0];
    const auto &blockSize = params.bloques[1];

    const int totalBlocks =
        static_cast<int>(blocks.nx) * static_cast<int>(blocks.ny) * static_cast<int>(blocks.nz);

    std::ostringstream output;
    output << "Number of particles: " << params.np << '\n'
           << "Particles per meter: " << params.ppm << '\n'
           << "Smoothing length: " << length << '\n'
           << "Mass of particles: " << mass << '\n'
           << "Grid size: " << blocks.nx << " x " << blocks.ny << " x " << blocks.nz << '\n'
           << "Number of blocks: " << totalBlocks << '\n'
           << "Block size: " << blockSize.nx << " x " << blockSize.ny << " x " << blockSize.nz << '\n';
    std::cout << output.str();

    return true;
}
