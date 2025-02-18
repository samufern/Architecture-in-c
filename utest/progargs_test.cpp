#include "progargs.hpp"

#include <gtest/gtest.h>

constexpr int NUM_ITERATIONS      = 10;
constexpr int PARTICLE_COUNT      = 100;
constexpr int FILE_PARTICLE_COUNT = 100;

class ProgArgsTest : public ::testing::Test {
  private:
    std::vector<std::string> args{};
    const int iterations        = NUM_ITERATIONS;
    const int particleCount     = PARTICLE_COUNT;
    const int fileParticleCount = FILE_PARTICLE_COUNT;

  public:
    [[nodiscard]] std::vector<std::string> const & getArgs() const { return args; }

    [[nodiscard]] int getIterations() const { return iterations; }

    [[nodiscard]] int getParticleCount() const { return particleCount; }

    [[nodiscard]] int getFileParticleCount() const { return fileParticleCount; }

    void setArgs(std::vector<std::string> const & newArgs) { args = newArgs; }

    void setIterations(int newIterations) { iterations = newIterations; }

    void setParticleCount(int newParticleCount) { particleCount = newParticleCount; }

    void setFileParticleCount(int newFileParticleCount) {
      fileParticleCount = newFileParticleCount;
    }

  protected:
    void SetUp() override {
      args              = {"program", "10", "input.fld", "output.fld"};
      iterations        = NUM_ITERATIONS;
      particleCount     = PARTICLE_COUNT;
      fileParticleCount = FILE_PARTICLE_COUNT;
    }
};

TEST_F(ProgArgsTest, TestIncorrectArgCount) {
  const int iterations                          = getIterations();
  const int particleCount                       = getParticleCount();
  const int fileParticleCount                   = getFileParticleCount();
  std::vector<std::string> incorrect_args = {"program"};
  EXPECT_FALSE(ProgArgs::validate(incorrect_args, iterations, particleCount, fileParticleCount));
}

TEST_F(ProgArgsTest, TestFirstArgNotInteger) {
  std::vector<std::string> args = getArgs();
  const int iterations                = getIterations();
  const int particleCount             = getParticleCount();
  const int fileParticleCount         = getFileParticleCount();
  args[1]                       = "not_an_integer";
  EXPECT_FALSE(ProgArgs::validate(args, iterations, particleCount, fileParticleCount));
}

TEST_F(ProgArgsTest, TestNegativeIterations) {
  std::vector<std::string> args = getArgs();
  const int iterations                = getIterations();
  const int particleCount             = getParticleCount();
  const int fileParticleCount         = getFileParticleCount();
  iterations                    = -1;
  EXPECT_FALSE(ProgArgs::validate(args, iterations, particleCount, fileParticleCount));
}

TEST_F(ProgArgsTest, TestValidArguments) {
  std::vector<std::string> args = getArgs();
  const int iterations                = getIterations();
  const int particleCount             = getParticleCount();
  const int fileParticleCount         = getFileParticleCount();
  EXPECT_TRUE(ProgArgs::validate(args, iterations, particleCount, fileParticleCount));
}

int main_progargs(int argc, char ** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
