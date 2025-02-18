// particle.cpp
#include "particle.hpp"

#include "block.hpp"
#include "constants.hpp"
#include "grid.hpp"

#include <algorithm>
#include <cmath>
#include <limits>
#include <thread>
#include <vector>

float calculateParticleMass(float density, float ppm) {
  return density / (ppm * ppm * ppm);
}

float calculateSmoothingLength(float radiusMultiplier, float ppm) {
  return radiusMultiplier / ppm;
}

GridSize calculateNumberOfBlocks(float height) {
  GridSize blocks;
  blocks.nx = std::floor((xmax - xmin) / height);
  blocks.ny = std::floor((ymax - ymin) / height);
  blocks.nz = std::floor((zmax - zmin) / height);
  return blocks;
}

GridSize calculateBlockSize(GridSize blocks) {
  GridSize blockSize;
  blockSize.nx = (xmax - xmin) / blocks.nx;
  blockSize.ny = (ymax - ymin) / blocks.ny;
  blockSize.nz = (zmax - zmin) / blocks.nz;
  return blockSize;
}

float calculateIncrementedDensity(Particle & parti, Particle & partj, float height) {
  const float deltaX                       = parti.px - partj.px;
  const float deltaY                       = parti.py - partj.py;
  const float deltaZ                       = parti.pz - partj.pz;
  const float distanceSquared              = deltaX * deltaX + deltaY * deltaY + deltaZ * deltaZ;
  const float hSquared                     = height * height;
  const float hSquaredMinusDistanceSquared = hSquared - distanceSquared;

  if (distanceSquared < hSquared) {
    return hSquaredMinusDistanceSquared * hSquaredMinusDistanceSquared *
           hSquaredMinusDistanceSquared;
  }

  return CERO;
}

void initializeDensitiesAndAccelerations(Particle & particle) {
  particle.rho = 0;
  particle.ax  = a_ext_x;
  particle.ay  = a_ext_y;
  particle.az  = a_ext_z;
}

void updateDensity(Particle & particle, Particle & particle2, float height) {
  const float densityIncrement  = calculateIncrementedDensity(particle, particle2, height);
  particle.rho           += densityIncrement;
  particle2.rho          += densityIncrement;
}

void transformDensity(Particle & particle, float height, float mass) {
  const float height_p6 = height * height * height * height * height * height;
  const float height_p9 = height * height * height * height * height * height * height * height * height;
  particle.rho    = (particle.rho + height_p6) *
                 (STIFFNESS_CONSTANT / (DENSITY_MULTIPLIER * PI * height_p9)) * mass;
}

void updateAcceleration(Particle & pi, Particle & pj, float height, float mass) {
  const float deltaX          = pi.px - pj.px;
  const float deltaY          = pi.py - pj.py;
  const float deltaZ          = pi.pz - pj.pz;
  const float distanceSquared = deltaX * deltaX + deltaY * deltaY + deltaZ * deltaZ;
  const float hSquared        = height * height;

  if (distanceSquared < hSquared && distanceSquared > SMALL_NUMBER * SMALL_NUMBER) {
    const float distance            = std::sqrt(distanceSquared);
    const float inverseDistance     = 1.0F / distance;
    const float heightMinusDistance = height - distance;
    const float commonTerm          = PRESSURE_TERM_CONSTANT / (PI * mass * ps);
    const float pressureTerm  = commonTerm * heightMinusDistance * heightMinusDistance * inverseDistance;
    const float viscosityTerm = VISCOSITY_CONSTANT / (PI * mu * mass) * inverseDistance * inverseDistance;
    const float axIncrement   = deltaX * pressureTerm + (pj.vx - pi.vx) * viscosityTerm;
    pi.ax             += axIncrement;
    pj.ax             -= axIncrement;
    const float ayIncrement  = deltaY * pressureTerm + (pj.vy - pi.vy) * viscosityTerm;
    pi.ay             += ayIncrement;
    pj.ay             -= ayIncrement;
    const float azIncrement  = deltaZ * pressureTerm + (pj.vz - pi.vz) * viscosityTerm;
    pi.az             += azIncrement;
    pj.az             -= azIncrement;
  }
}

bool checkCollision(float pos, float velocity, Bounds const & bounds, float dp) {
  return (pos + velocity - dp < bounds.min) || (pos + velocity + dp > bounds.max);
}

void processCollisions(Particle & particle) {
  const Bounds xBounds{xmin, xmax};
  const Bounds yBounds{ymin, ymax};
  const Bounds zBounds{zmin, zmax};
  const CollisionInfo xCollisionInfo = {particle.px + particle.hvx * delta_t,
                                  checkCollision(particle.px, particle.hvx * delta_t, xBounds, dp),
                                  xBounds, 'x'};
  const CollisionInfo yCollisionInfo = {particle.py + particle.hvy * delta_t,
                                  checkCollision(particle.py, particle.hvy * delta_t, yBounds, dp),
                                  yBounds, 'y'};
  const CollisionInfo zCollisionInfo = {particle.pz + particle.hvz * delta_t,
                                  checkCollision(particle.pz, particle.hvz * delta_t, zBounds, dp),
                                  zBounds, 'z'};
  if (xCollisionInfo.isCollision || yCollisionInfo.isCollision || zCollisionInfo.isCollision) {
    processCollisionResponse(particle, xCollisionInfo, yCollisionInfo, zCollisionInfo);
  }
}

void processCollisionResponse(Particle & particle, CollisionInfo const & xInfo,
                              CollisionInfo const & yInfo, CollisionInfo const & zInfo) {
  handleCollisionAxis(particle, xInfo);
  handleCollisionAxis(particle, yInfo);
  handleCollisionAxis(particle, zInfo);
}

void handleCollisionAxis(Particle & particle, CollisionInfo const & info) {
  if (!info.isCollision) { return; }

  const float boundary = info.isCollision ? info.bounds.min : info.bounds.max;
  const float delta =
      dp - (info.isCollision ? (info.newPos - info.bounds.min) : (info.bounds.max - info.newPos));
  float & pos  = (info.axis == 'x') ? particle.px : (info.axis == 'y') ? particle.py : particle.pz;
  float & vel  = (info.axis == 'x') ? particle.vx : (info.axis == 'y') ? particle.vy : particle.vz;
  float & hvel = (info.axis == 'x')   ? particle.hvx
                 : (info.axis == 'y') ? particle.hvy
                                      : particle.hvz;
  float & acc  = (info.axis == 'x') ? particle.ax : (info.axis == 'y') ? particle.ay : particle.az;

  if (delta > SMALL_NUMBER) {
    acc  += info.isCollision ? (sc * delta - dv * vel) : -(sc * delta + dv * vel);
    pos   = boundary + (info.isCollision ? delta : -delta);
    vel   = -vel;
    hvel  = -hvel;
  }
}

void updateParticleMotion(Particle & particle) {
  particle.px  += particle.hvx * delta_t + HALF * particle.ax * delta_t * delta_t;
  particle.py  += particle.hvy * delta_t + HALF * particle.ay * delta_t * delta_t;
  particle.pz  += particle.hvz * delta_t + HALF * particle.az * delta_t * delta_t;
  particle.vx   = particle.hvx + particle.ax * delta_t;
  particle.vy   = particle.hvy + particle.ay * delta_t;
  particle.vz   = particle.hvz + particle.az * delta_t;
  particle.hvx += particle.ax * delta_t;
  particle.hvy += particle.ay * delta_t;
  particle.hvz += particle.az * delta_t;
}
