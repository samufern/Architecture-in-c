#include "constants.hpp"
#include "utils.hpp"

#include <cstdio>
#include <cstring>
#include <fstream>
#include <gtest/gtest.h>

static constexpr int TEST_PARTICLE_COUNT = 10;

class UtilsTest : public ::testing::Test {
  private:
    std::string test_filename{"init.fld"};
    Header test_header;
    std::vector<Particle> test_particles;

  public:
    UtilsTest() : test_header() { }

    [[nodiscard]] std::string const & getTestFilename() const { return test_filename; }

    void setTestFilename(std::string const & filename) { test_filename = filename; }

    [[nodiscard]] Header const & getTestHeader() const { return test_header; }

    void setTestHeader(Header const & header) { test_header = header; }

  protected:
    void SetUp() override {
      test_header.ppm = 1.0;
      test_header.np  = TEST_PARTICLE_COUNT;
      setTestFilename("small.fld");
      std::ofstream outFile(getTestFilename(), std::ios::binary);
      if (!outFile) { exit(-1); }
      std::array<char, sizeof(Header)> buffer{};
      std::memcpy(buffer.data(), &test_header, sizeof(Header));
      outFile.write(buffer.data(), sizeof(buffer));
      for (int i = 0; i < test_header.np; ++i) {
        Particle particle = {};
        std::array<char, sizeof(Particle)> buffer2{};
        std::memcpy(buffer2.data(), &particle, sizeof(Particle));
        outFile.write(buffer2.data(), sizeof(Particle));
      }
      outFile.close();
    }

    void TearDown() override { (void) std::remove(getTestFilename().c_str()); }
};

TEST_F(UtilsTest, TestReadHeader) {
  std::ifstream inFile(getTestFilename(), std::ios::binary);
  Header header = getTestHeader();
  bool result   = readHeader(inFile, header);
  inFile.close();

  EXPECT_TRUE(result);
  EXPECT_FLOAT_EQ(header.ppm, header.ppm);
  EXPECT_EQ(header.np, header.np);
}

TEST_F(UtilsTest, TestIsInteger) {
  EXPECT_TRUE(isInteger("12345"));
  EXPECT_FALSE(isInteger("123.45"));
  EXPECT_FALSE(isInteger("abcde"));
  EXPECT_FALSE(isInteger(""));
}

int main_utils(int argc, char ** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
