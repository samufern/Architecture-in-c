// ftest.cpp
#include "block.hpp"
#include "constants.hpp"
#include "grid.hpp"
#include "particle.hpp"
#include "progargs.hpp"
#include "simulation.hpp"
#include "utils.hpp"

#include <filesystem>
#include <fstream>
#include <functional>
#include <gtest/gtest.h>
#include <iostream>
#include <string>
#include <vector>

// Definición de constantes globales para la prueba
static constexpr float testParticlePosX = 10.0F;
static constexpr float testParticlePosY = 20.0F;
static constexpr float testParticlePosZ = 30.0F;
static constexpr int blockSizeX         = 1;
static constexpr int blockSizeY         = 1;
static constexpr int blockSizeZ         = 1;
static constexpr int blocksNumX         = 10;
static constexpr int blocksNumY         = 10;
static constexpr int blocksNumZ         = 10;
static constexpr float TOLERANCE        = 0.001F;

class BlockManagementTest : public ::testing::Test {
  private:
    Particle testParticle = {};
    GridSize blockSize;
    GridSize gridDimensions;

  protected:
    void SetUp() override {
      // Completa la inicialización con los valores específicos
      testParticle = {
        testParticlePosX,
        testParticlePosY,
        testParticlePosZ,  // Posición
        0.0F,
        0.0F,
        0.0F,  // Velocidades hvx, hvy, hvz
        0.0F,
        0.0F,
        0.0F,  // Velocidades vx, vy, vz
        rho,   // Valor para rho
        0.0F,
        0.0F,
        0.0F,  // Aceleraciones ax, ay, az
      };
      blockSize      = {blockSizeX, blockSizeY, blockSizeZ};
      gridDimensions = {blocksNumX, blocksNumY, blocksNumZ};
    }

    [[nodiscard]] std::vector<int> GetCalculatedIndices() const {
      auto indicesArray = getBlockIndices(testParticle, blockSize, gridDimensions);
      return {indicesArray.begin(), indicesArray.end()};
    }
};

TEST_F(BlockManagementTest, TestBlockIndexCalculation) {
  auto const indices                     = GetCalculatedIndices();
  std::vector<int> const expectedIndices = {9, 9, 9};

  ASSERT_EQ(indices.size(), expectedIndices.size())
      << "Los tamaños de los vectores de índices no coinciden.";

  for (size_t i = 0; i < expectedIndices.size(); ++i) {
    EXPECT_EQ(indices[i], expectedIndices[i])
        << "Índice de bloque en la posición " << i << " es incorrecto.";
  }
}

class GridManagementFunctionalTest : public ::testing::Test {
  private:
    static int const DomainSizeX     = 100;
    static int const DomainSizeY     = 200;
    static int const DomainSizeZ     = 300;
    static int const ReferenceHeight = 1;

    float referenceHeight = static_cast<float>(ReferenceHeight);
    GridSize domainSize   = {DomainSizeX, DomainSizeY, DomainSizeZ};
    GridSize expectedNumberOfBlocks;
    GridSize expectedBlockSize;

  protected:
    void SetUp() override {
      expectedNumberOfBlocks = {DomainSizeX, DomainSizeY, DomainSizeZ};
      expectedBlockSize      = {ReferenceHeight, ReferenceHeight, ReferenceHeight};
    }

    [[nodiscard]] float getReferenceHeight() const { return referenceHeight; }

    [[nodiscard]] GridSize getDomainSize() const { return domainSize; }

    [[nodiscard]] GridSize getExpectedNumberOfBlocks() const { return expectedNumberOfBlocks; }

    [[nodiscard]] GridSize getExpectedBlockSize() const { return expectedBlockSize; }
};

TEST_F(GridManagementFunctionalTest, TestGridSizeAndBlockCalculations) {
  // Calcular el número de bloques
  GridSize calculatedNumberOfBlocks;
  calculateNumberOfBlocks(getReferenceHeight(), getDomainSize(), calculatedNumberOfBlocks);
  EXPECT_EQ(calculatedNumberOfBlocks.nx, getExpectedNumberOfBlocks().nx);
  EXPECT_EQ(calculatedNumberOfBlocks.ny, getExpectedNumberOfBlocks().ny);
  EXPECT_EQ(calculatedNumberOfBlocks.nz, getExpectedNumberOfBlocks().nz);

  // Calcular el tamaño de los bloques
  GridSize calculatedBlockSize;
  calculateBlockSize(calculatedNumberOfBlocks, getDomainSize(), calculatedBlockSize);
  EXPECT_NEAR(calculatedBlockSize.nx, getExpectedBlockSize().nx, TOLERANCE);
  EXPECT_NEAR(calculatedBlockSize.ny, getExpectedBlockSize().ny, TOLERANCE);
  EXPECT_NEAR(calculatedBlockSize.nz, getExpectedBlockSize().nz, TOLERANCE);

  // For total grid size comparisons, consider the cumulative error
  EXPECT_NEAR(getDomainSize().nx, calculatedNumberOfBlocks.nx * calculatedBlockSize.nx,
              TOLERANCE * calculatedNumberOfBlocks.nx);
  EXPECT_NEAR(getDomainSize().ny, calculatedNumberOfBlocks.ny * calculatedBlockSize.ny,
              TOLERANCE * calculatedNumberOfBlocks.ny);
  EXPECT_NEAR(getDomainSize().nz, calculatedNumberOfBlocks.nz * calculatedBlockSize.nz,
              TOLERANCE * calculatedNumberOfBlocks.nz);
}

class CommandLineArgumentsTest : public ::testing::Test {
  private:
    static constexpr int DefaultIterations    = 10;
    static constexpr int DefaultParticleCount = 100;

    std::vector<std::string> args;
    int iterations{DefaultIterations};
    int particleCount{DefaultParticleCount};
    int fileParticleCount{DefaultParticleCount};

  protected:
    void SetUp() override {
      args = {"program", std::to_string(DefaultIterations), "input.fld", "output.fld"};
    }

    [[nodiscard]] std::vector<std::string> const & getArgs() const { return args; }

    [[nodiscard]] int getIterations() const { return iterations; }

    [[nodiscard]] int getParticleCount() const { return particleCount; }

    [[nodiscard]] int getFileParticleCount() const { return fileParticleCount; }

    void modifyArgs(std::function<void(std::vector<std::string> &)> const & modifier) {
      modifier(args);
    }

    void setIterations(int newIterations) { iterations = newIterations; }

    void setParticleCount(int newParticleCount) { particleCount = newParticleCount; }

    void setFileParticleCount(int newFileParticleCount) {
      fileParticleCount = newFileParticleCount;
    }
};

TEST_F(CommandLineArgumentsTest, ArgumentCountCorrect) {
  EXPECT_TRUE(
      ProgArgs::validate(getArgs(), getIterations(), getParticleCount(), getFileParticleCount()));
}

TEST_F(CommandLineArgumentsTest, MissingArguments) {
  modifyArgs([](auto & args) {
    args.pop_back();
  });
  EXPECT_FALSE(
      ProgArgs::validate(getArgs(), getIterations(), getParticleCount(), getFileParticleCount()));
}

TEST_F(CommandLineArgumentsTest, AdditionalArguments) {
  modifyArgs([](auto & args) {
    args.emplace_back("extra_arg");
  });
  EXPECT_FALSE(
      ProgArgs::validate(getArgs(), getIterations(), getParticleCount(), getFileParticleCount()));
}

TEST_F(CommandLineArgumentsTest, NonIntegerIterationCount) {
  modifyArgs([](auto & args) {
    args[1] = "no_number";
  });
  EXPECT_FALSE(ProgArgs::validate(getArgs(), -1, getParticleCount(), getFileParticleCount()));
}

TEST_F(CommandLineArgumentsTest, NegativeIterationCount) {
  setIterations(-1);
  EXPECT_FALSE(
      ProgArgs::validate(getArgs(), getIterations(), getParticleCount(), getFileParticleCount()));
}

TEST_F(CommandLineArgumentsTest, InvalidInputFile) {
  modifyArgs([](auto & args) {
    args[2] = "nonexistent_input.fld";
  });
  EXPECT_FALSE(
      ProgArgs::validate(getArgs(), getIterations(), getParticleCount(), getFileParticleCount()));
}

TEST_F(CommandLineArgumentsTest, InvalidOutputFile) {
  modifyArgs([](auto & args) {
    args[3] = "nonexistent_output.fld";
  });
  EXPECT_FALSE(
      ProgArgs::validate(getArgs(), getIterations(), getParticleCount(), getFileParticleCount()));
}

TEST_F(CommandLineArgumentsTest, InvalidParticleCount) {
  setParticleCount(-1);
  EXPECT_FALSE(
      ProgArgs::validate(getArgs(), getIterations(), getParticleCount(), getFileParticleCount()));
}

TEST_F(CommandLineArgumentsTest, MismatchParticleCount) {
  setFileParticleCount(getParticleCount() - 1);
  EXPECT_FALSE(
      ProgArgs::validate(getArgs(), getIterations(), getParticleCount(), getFileParticleCount()));
}

TEST(ParticleManagement, TestParticleMotionUpdate) {
  float const INITIAL_POSITION     = 0.0;
  float const INITIAL_VELOCITY     = 1.0;
  float const INITIAL_ACCELERATION = 0.5;
  float const INITIAL_DENSITY      = 0.0;

  Particle particle = {INITIAL_POSITION,    INITIAL_POSITION,     INITIAL_POSITION,
                       INITIAL_VELOCITY,    INITIAL_VELOCITY,     INITIAL_VELOCITY,
                       INITIAL_VELOCITY,    INITIAL_VELOCITY,     INITIAL_VELOCITY,
                       INITIAL_DENSITY,     INITIAL_ACCELERATION, INITIAL_ACCELERATION,
                       INITIAL_ACCELERATION};

  updateParticleMotion(particle);

  EXPECT_FLOAT_EQ(particle.px, INITIAL_ACCELERATION);
  EXPECT_FLOAT_EQ(particle.py, INITIAL_ACCELERATION);
  EXPECT_FLOAT_EQ(particle.pz, INITIAL_ACCELERATION);
  EXPECT_FLOAT_EQ(particle.vx, INITIAL_VELOCITY + INITIAL_ACCELERATION);
  EXPECT_FLOAT_EQ(particle.vy, INITIAL_VELOCITY + INITIAL_ACCELERATION);
  EXPECT_FLOAT_EQ(particle.vz, INITIAL_VELOCITY + INITIAL_ACCELERATION);
  EXPECT_FLOAT_EQ(particle.hvx, INITIAL_VELOCITY + INITIAL_ACCELERATION);
  EXPECT_FLOAT_EQ(particle.hvy, INITIAL_VELOCITY + INITIAL_ACCELERATION);
  EXPECT_FLOAT_EQ(particle.hvz, INITIAL_VELOCITY + INITIAL_ACCELERATION);
}

bool fileExists(std::string const & filename) {
  if (std::filesystem::exists(filename)) { return true; }
  std::cerr << "Error: No se pudo abrir el archivo " << filename << '\n';
  return false;
}

TEST(SimulationTest, RunSimulation) {
  std::string archivo_entrada = "/in/small.fld";
  std::string archivo_salida  = "/out/small_output.fld";

  ASSERT_NO_THROW(runSimulation(10, archivo_entrada, archivo_salida));
  ASSERT_TRUE(fileExists(archivo_salida));
}

int main(int argc, char ** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}