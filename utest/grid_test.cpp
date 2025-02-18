#include "grid.hpp"

#include <gtest/gtest.h>

float const num_blocks   = 10;
float const domain_size  = 100.0F;
float const block_height = 10.0F;

class MockConstants {
  public:
    static float xmin() { return 0.0F; }

    static float ymin() { return 0.0F; }

    static float zmin() { return 0.0F; }
};

class GridTest : public ::testing::Test {
  private:
    GridSize domainSize{};
    const float blockHeight{};

  public:
    [[nodiscard]] GridSize const & getDomainSize() const { return domainSize; }

    [[nodiscard]] float getBlockHeight() const { return blockHeight; }

    void setDomainSize(GridSize const & size) { domainSize = size; }

    void setBlockHeight(float height) { blockHeight = height; }

  protected:
    void SetUp() override {
      domainSize.nx = domain_size;
      domainSize.ny = domain_size;
      domainSize.nz = domain_size;

      blockHeight = block_height;
    }
};

TEST_F(GridTest, TestCalculateNumberOfBlocks) {
  GridSize domainSize = getDomainSize();
  const float blockHeight   = getBlockHeight();
  GridSize blocks;
  calculateNumberOfBlocks(blockHeight, domainSize, blocks);

  EXPECT_EQ(blocks.nx, num_blocks);
  EXPECT_EQ(blocks.ny, num_blocks);
  EXPECT_EQ(blocks.nz, num_blocks);
}

TEST_F(GridTest, TestCalculateBlockSize) {
  GridSize domainSize = getDomainSize();
  const float blockHeight   = getBlockHeight();
  GridSize numberOfBlocks{static_cast<int>(num_blocks), static_cast<int>(num_blocks),
                          static_cast<int>(num_blocks)};
  GridSize blockSize;
  calculateBlockSize(numberOfBlocks, domainSize, blockSize);

  EXPECT_NEAR(blockSize.nx, blockHeight, 1e-1);
  EXPECT_NEAR(blockSize.ny, blockHeight, 1e-1);
  EXPECT_NEAR(blockSize.nz, blockHeight, 1e-1);
}

int main_grid(int argc, char ** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
