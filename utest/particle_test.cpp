#include "particle.hpp"

#include <cassert>
#include <cmath>
#include <gtest/gtest.h>
#include <iostream>

int const expected_blocks_1_x_z = 5;
int const expected_blocks_1_y   = 7;
int const expected_blocks_2_x_z = 9;
int const expected_blocks_2_y   = 13;
float const height_test_1       = 0.025F;
float const height_test_2       = 0.013F;
float const height_test_3       = 1.5F;
float const test_rho            = 10.0F;
float const test_mass           = 1.0F;
float const boundmin            = 0.0F;
float const boundmax            = 10.0F;
float const boundmax_2          = 100.0F;
float const test_accelerations  = 2.0F;
float const blocksize_margin    = 0.001F;
float const acceleration_margin = 0.1F;
Particle const test_particle_1  = {
  0.0F, 0.0F, 0.0F,       // Initial position
  1.0F, 1.0F, 1.0F,       // Initial half-step velocity
  0.0F, 0.0F, 0.0F,       // Initial acceleration
  0.0F, 0.0F, 0.0F, 0.0F  // Initial velocity
};

Particle const test_particle_2 = {
  0.0F,
  0.0F,
  0.0F,  // Initial position
  1.0F,
  1.0F,
  1.0F,  // Initial half-step velocity
  0.0F,
  0.0F,
  0.0F,  // Initial acceleration
  0.0F,
  test_accelerations,
  test_accelerations,
  test_accelerations  // Initial velocity
};  // Initial accelerations

Particle const test_parti = {0.02F, 0.2F, 0.0F,         // Initial position
                             1.0F,  1.0F, 1.0F,         // Initial half-step velocity
                             1.0F,  1.0F, 1.0F,         // Initial acceleration
                             0.0F,  0.5F, 0.5F, 0.5F};  // Initial accelerations
Particle const test_partj = {-0.02F, -0.2F, 0.0F,       // Initial position
                             1.0F,   1.0F,  1.0F,       // Initial half-step velocity
                             1.0F,   1.0F,  -.0F,       // Initial acceleration
                             0.0F,   0.5F,  0.5F, 0.5F};

// Example unit tests for calculateParticleMass
TEST(ParticleMassTest, ParticleMass) {
  assert(std::abs(calculateParticleMass(1000.0f, 1.0f) - 1000.0f) < 0.0001f);
  assert(std::abs(calculateParticleMass(0.0f, 1.0f)) < 0.0001f);
}

TEST(SmothingLengthTest, SmothingLength) {
  assert(std::abs(calculateSmoothingLength(1.0f, 1.0f) - 1.0f) < 0.0001f);
}

TEST(NumBlocksTest, NumBlocks) {
  GridSize blocks = calculateNumberOfBlocks(height_test_1);
  EXPECT_EQ(blocks.nx, expected_blocks_1_x_z);
  EXPECT_EQ(blocks.ny, expected_blocks_1_y);
  EXPECT_EQ(blocks.nz, expected_blocks_1_x_z);

  blocks = calculateNumberOfBlocks(height_test_2);
  EXPECT_EQ(blocks.nx, expected_blocks_2_x_z);
  EXPECT_EQ(blocks.ny, expected_blocks_2_y);
  EXPECT_EQ(blocks.nz, expected_blocks_2_x_z);
}

TEST(BlockSizeTest, BlockSize) {
  GridSize blocks    = {expected_blocks_1_x_z, expected_blocks_1_y, expected_blocks_1_x_z};
  GridSize blockSize = calculateBlockSize(blocks);
  EXPECT_NEAR(blockSize.nx, height_test_1, blocksize_margin);
  EXPECT_NEAR(blockSize.ny, height_test_1, blocksize_margin);
  EXPECT_NEAR(blockSize.nz, height_test_1, blocksize_margin);
}

TEST(IncrementedDensityTest, IncrementedDensity) {
  Particle parti           = test_parti;
  Particle partj           = test_partj;
  const float height             = height_test_3;
  const float incrementedDensity = calculateIncrementedDensity(parti, partj, height);
  EXPECT_GT(incrementedDensity, 0.0F);
}

TEST(InitializeDensitiesAndAccelerationsTest, InitializeDensitiesAndAccelerations) {
  Particle particle = test_particle_1;
  particle.rho      = test_rho;
  particle.ax       = 0.0F;
  particle.ay       = -g;
  particle.az       = 0.0F;

  initializeDensitiesAndAccelerations(particle);

  EXPECT_EQ(particle.rho, 0.0F);
  EXPECT_EQ(particle.ax, a_ext_x);
  EXPECT_EQ(particle.ay, a_ext_y);
  EXPECT_EQ(particle.az, a_ext_z);
}

TEST(UpdateDensityTest, UpdateDensity) {
  Particle particle             = test_parti;
  Particle particle2            = test_partj;
  const float height                  = height_test_3;
  const float initialDensityParticle  = particle.rho;
  const float initialDensityParticle2 = particle2.rho;
  updateDensity(particle, particle2, height);
  EXPECT_GT(particle.rho, initialDensityParticle);
  EXPECT_GT(particle2.rho, initialDensityParticle2);
}

TEST(TransformDensityTest, TransformDensity) {
  Particle particle = test_particle_2;
  const float height      = height_test_3;
  const float mass        = test_mass;
  transformDensity(particle, height, mass);
  assert(particle.rho != test_rho);
}

TEST(updateAccTest, updateAccel) {
  Particle parti = test_parti;
  Particle partj = test_partj;
  const float height   = height_test_1;
  const float mass     = test_mass;
  updateAcceleration(parti, partj, height, mass);
  EXPECT_NEAR(parti.ax, partj.ax, acceleration_margin);
  EXPECT_NEAR(parti.ay, partj.ay, acceleration_margin);
  EXPECT_NEAR(parti.az, partj.az, acceleration_margin);
}

TEST(CheckCollisionTest, CheckCollision) {
  Bounds bounds = {boundmin, boundmax_2};
  EXPECT_FALSE(checkCollision(50.0F, 10.0F, bounds, 5.0F));
  EXPECT_TRUE(checkCollision(95.0F, 10.0F, bounds, 5.0F));
  EXPECT_TRUE(checkCollision(5.0F, -10.0F, bounds, 5.0F));
  EXPECT_TRUE(checkCollision(0.0F, -5.0F, bounds, 5.0F));
  EXPECT_TRUE(checkCollision(100.0F, 5.0F, bounds, 5.0F));
  EXPECT_FALSE(checkCollision(5.1F, -0.1F, bounds, 5.0F));
  EXPECT_FALSE(checkCollision(94.9F, 0.1F, bounds, 5.0F));
  EXPECT_FALSE(checkCollision(50.0F, 0.0F, bounds, 5.0F));
  EXPECT_TRUE(checkCollision(5.0F, -5.0F, bounds, 10.0F));
  EXPECT_FALSE(checkCollision(15.0F, -5.0F, bounds, 10.0F));
}

TEST(testProcessCollisions, ProcessCollision) {
  Particle particle = test_particle_1;
  processCollisions(particle);
  particle.px = xmax - dp;
  processCollisions(particle);
  particle.py = ymin + dp;
  processCollisions(particle);
  particle.pz = zmax - dp;
  processCollisions(particle);
}

TEST(testProcessCollisionResponse, processResponse) {
  Particle particle   = {/* initialize with test values */};
  CollisionInfo xInfo = {/* initialize based on whether x-axis collision occurred */};
  CollisionInfo yInfo = {/* initialize based on whether y-axis collision occurred */};
  CollisionInfo zInfo = {/* initialize based on whether z-axis collision occurred */};

  processCollisionResponse(particle, xInfo, yInfo, zInfo);

  xInfo.isCollision = true;
  processCollisionResponse(particle, xInfo, yInfo, zInfo);

  yInfo.isCollision = true;
  zInfo.isCollision = true;
  processCollisionResponse(particle, xInfo, yInfo, zInfo);
}

TEST(testHandleCollisionAxisX, HandleCollsX) {
  Particle particle  = test_particle_1;
  CollisionInfo info = {
    particle.px + particle.vx, true, {boundmin, boundmax},
        'x'
  };
  Particle originalParticle = particle;
  handleCollisionAxis(particle, info);
  assert(particle.px == info.bounds.min + dp);
  assert(particle.vx < originalParticle.vx);
  assert(particle.ax != originalParticle.ax);
  info.newPos      = 1.0F;
  originalParticle = particle;
  handleCollisionAxis(particle, info);
  assert(particle.px == info.bounds.max - dp);
  assert(particle.vx < originalParticle.vx);
  assert(particle.ax != originalParticle.ax);
}

TEST(testHandleCollisionAxisY, HandleCollsY) {
  Particle particle  = test_particle_1;
  CollisionInfo info = {
    particle.py + particle.vy, true, {boundmin, boundmax},
        'y'
  };
  Particle originalParticle = particle;
  handleCollisionAxis(particle, info);
  assert(particle.py == info.bounds.min + dp);
  assert(particle.vy < originalParticle.vy);
  assert(particle.ay != originalParticle.ay);
  info.newPos      = 1.0F;
  originalParticle = particle;
  handleCollisionAxis(particle, info);
  assert(particle.py == info.bounds.max - dp);
  assert(particle.vy < originalParticle.vy);
  assert(particle.ay != originalParticle.ay);
}

TEST(testHandleCollisionAxisZ, HandleCollsZ) {
  Particle particle  = test_particle_1;
  CollisionInfo info = {
    particle.pz + particle.vz, true, {boundmin, boundmax},
        'z'
  };
  Particle originalParticle = particle;
  handleCollisionAxis(particle, info);
  assert(particle.pz == info.bounds.min + dp);
  assert(particle.vz < originalParticle.vz);
  assert(particle.az != originalParticle.az);
  info.newPos      = 1.0F;
  originalParticle = particle;
  handleCollisionAxis(particle, info);
  assert(particle.pz == info.bounds.max - dp);
  assert(particle.vz < originalParticle.vz);
  assert(particle.az != originalParticle.az);
}

TEST(testUpdateParticleMotion, UpdateMotion) {
  Particle particle = test_particle_2;
  const float expected_vx = particle.hvx + particle.ax * delta_t;
  const float expected_vy = particle.hvy + particle.ay * delta_t;
  const float expected_vz = particle.hvz + particle.az * delta_t;
  const float expected_px = particle.px + particle.hvx * delta_t + HALF * particle.ax * delta_t * delta_t;
  const float expected_py = particle.py + particle.hvy * delta_t + HALF * particle.ay * delta_t * delta_t;
  const float expected_pz = particle.pz + particle.hvz * delta_t + HALF * particle.az * delta_t * delta_t;
  updateParticleMotion(particle);
  EXPECT_EQ(particle.px, expected_px);
  EXPECT_EQ(particle.py, expected_py);
  EXPECT_EQ(particle.pz, expected_pz);
  EXPECT_EQ(particle.vx, expected_vx);
  EXPECT_EQ(particle.vy, expected_vy);
  EXPECT_EQ(particle.vz, expected_vz);
}

int particle_tests(int argc, char ** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}