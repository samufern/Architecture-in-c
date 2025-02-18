#include "simulation.hpp"
#include "utils.hpp"

#include "gtest/gtest.h"
#include <filesystem>
#include <fstream>

class SimulationTest : public ::testing::Test {
  private:
    std::string testInputFile;
    std::string testOutputFile;

  public:
    [[nodiscard]] std::string getTestInputFile() const { return testInputFile; }

    [[nodiscard]] std::string getTestOutputFile() const { return testOutputFile; }

    void setTestInputFile(std::string const & inputFile) { testInputFile = inputFile; }

    void setTestOutputFile(std::string const & outputFile) { testOutputFile = outputFile; }

  protected:
    void SetUp() override {
      testInputFile  = "/in/small.fld";
      testOutputFile = "/out/small_output.fld";
    }
};

TEST_F(SimulationTest, ValidInput) {
  ASSERT_NO_THROW(runSimulation(10, getTestInputFile(), getTestOutputFile()));
}

TEST_F(SimulationTest, BoundaryTest) {
  ASSERT_NO_THROW(runSimulation(0, getTestInputFile(), getTestOutputFile()));
  ASSERT_NO_THROW(runSimulation(INT_MAX, getTestInputFile(), getTestOutputFile()));
}

TEST_F(SimulationTest, ErrorHandling) {
  ASSERT_THROW(runSimulation(10, "non_existent_input.fld", getTestOutputFile()), std::exception);
  ASSERT_THROW(runSimulation(10, getTestInputFile(), "non_writable_output.fld"), std::exception);
}

int simulation_test(int argc, char ** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
