// particle.hpp
#pragma once
#include "constants.hpp"
#include "grid.hpp"

#include <vector>

struct Header {
    float ppm;
    int np;
};

struct Particle {
    float px, py, pz;
    float hvx, hvy, hvz;
    float vx, vy, vz;
    float rho;
    float ax, ay, az;
};

struct ParticlePair {
    Particle * pi;
    Particle * pj;
};

struct DeltaValues {
    float deltaX;
    float deltaY;
    float deltaZ;
    float deltaVX;
    float deltaVY;
    float deltaVZ;
};

struct Bounds {
    float min;
    float max;
};

struct CollisionInfo {
    float newPos;
    bool isCollision;
    Bounds bounds;
    char axis;
};

float calculateParticleMass(float density, float ppm);
float calculateSmoothingLength(float radiusMultiplier, float ppm);
GridSize calculateNumberOfBlocks(float height);
GridSize calculateBlockSize(GridSize blocks);
float calculateIncrementedDensity(Particle & parti, Particle & partj, float height);
void initializeDensitiesAndAccelerations(Particle & particle);
void updateDensity(Particle & particle, Particle & particle2, float height);
void transformDensity(Particle & particle, float height, float mass);
void calculateTerms(ParticlePair & pair, DeltaValues & deltas, float height, float mass);
void updateAcceleration(Particle & pi, Particle & pj, float height, float mass);
bool checkCollision(float pos, float velocity, Bounds const & bounds, float dp);
void processCollisions(Particle & particle);
void processCollisionResponse(Particle & particle, CollisionInfo const & xInfo,
                              CollisionInfo const & yInfo, CollisionInfo const & zInfo);
void handleCollisionAxis(Particle & particle, CollisionInfo const & info);
void updateParticleMotion(Particle & particle);
