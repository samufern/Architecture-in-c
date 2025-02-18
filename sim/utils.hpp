// utils.hpp
#pragma once
#include "particle.hpp"

#include <string>
#include <vector>

struct SimulationParameters {
    int iterations;
    std::vector<float> parametros;
    std::vector<GridSize> bloques;
};

struct SalidaParameters {
    int np;
    double ppm;
    std::vector<float> const & parametros;
    std::vector<GridSize> const & bloques;
};

struct ParticleParameters {
    float smoothingLength;
    float mass;
    GridSize blockSize;
    GridSize blocks;
};

bool readHeader(std::ifstream & inFile, Header & header);
bool readParticleData(std::ifstream & inFile, std::vector<Particle> & particles, int np);

// Function to check if a string is an integer
bool isInteger(std::string const & s);

// Function to read particles from a file
bool readParticlesFromFile(std::string const & filename, Header & header,
                           std::vector<Particle> & particles);

// Function to write particles to a file
bool writeParticlesToFile(std::string const & filename, Header const & header,
                          std::vector<Particle> const & particles);
bool readInputFile(std::string const & filename, Header & header,
                   std::vector<Particle> & particles);
void updateParticles(std::vector<Particle> & particles, ParticleParameters params);
void updateParticlePair(Particle & particle1, Particle & particle2, float smoothingLength,
                        float mass);
void simulationWithIterations(std::vector<Particle> & particles, const SimulationParameters & params);
bool isInteger(std::string const & s);
bool salida(const SalidaParameters & params);
