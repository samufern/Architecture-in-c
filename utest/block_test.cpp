#include "block.hpp"
#include "constants.hpp"
#include "grid.hpp"
#include "particle.hpp"

#include <gtest/gtest.h>

float const X_MIN        = 0.0F;
float const Y_MIN        = 0.0F;
float const Z_MIN        = 0.0F;
float const GRID_SIZE    = 10.0F;
float const PARTICLE_POS = 0.01F;

class MockConstants {
  public:
    static float xmin() { return X_MIN; }

    static float ymin() { return Y_MIN; }

    static float zmin() { return Z_MIN; }
};

class BlockTest : public ::testing::Test {
  private:
    Particle test_particle{};
    GridSize test_block_size{};
    GridSize test_blocks{};

  public:
    BlockTest() = default;

    [[nodiscard]] Particle const & getTestParticle() const { return test_particle; }

    [[nodiscard]] GridSize const & getTestBlockSize() const { return test_block_size; }

    [[nodiscard]] GridSize const & getTestBlocks() const { return test_blocks; }

    void setTestParticle(Particle const & particle) { test_particle = particle; }

    void setTestBlockSize(GridSize const & block_size) { test_block_size = block_size; }

    void setTestBlocks(GridSize const & blocks) { test_blocks = blocks; }

  protected:
    void SetUp() override {
      test_particle.px = PARTICLE_POS;
      test_particle.py = PARTICLE_POS;
      test_particle.pz = PARTICLE_POS;

      test_block_size.nx = GRID_SIZE;
      test_block_size.ny = GRID_SIZE;
      test_block_size.nz = GRID_SIZE;

      test_blocks.nx = 2;
      test_blocks.ny = 2;
      test_blocks.nz = 2;
    }
};

TEST_F(BlockTest, TestGetBlockIndices) {
  Particle test_particle   = getTestParticle();
  GridSize test_block_size = getTestBlockSize();
  GridSize test_blocks     = getTestBlocks();
  auto indices       = getBlockIndices(test_particle, test_blocks, test_block_size);

  EXPECT_EQ(indices[0], 0);
  EXPECT_EQ(indices[1], 0);
  EXPECT_EQ(indices[2], 0);
}

TEST_F(BlockTest, TestRepositionParticle) {
  Particle test_particle   = getTestParticle();
  GridSize test_block_size = getTestBlockSize();
  GridSize test_blocks     = getTestBlocks();
  repositionParticle(test_particle, test_blocks, test_block_size);

  EXPECT_FLOAT_EQ(test_particle.px, PARTICLE_POS);
  EXPECT_FLOAT_EQ(test_particle.py, PARTICLE_POS);
  EXPECT_FLOAT_EQ(test_particle.pz, PARTICLE_POS);
}

int main_block(int argc, char ** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}