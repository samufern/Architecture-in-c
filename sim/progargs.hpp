// progargs.hpp
#pragma once

#include <fstream>
#include <string>
#include <vector>

class ProgArgs {
  public:
    ProgArgs(std::vector<std::string> const & args);
    static bool validate(std::vector<std::string> const & args, int iterations, int particleCount,
                         int fileParticleCount);

  private:
    std::vector<std::string> args;
    static bool checkArgCount(std::vector<std::string> const & args);
    static bool checkFirstArg(std::vector<std::string> const & args);
    static bool checkIterations(int iterations);
    static bool checkInputFile(std::vector<std::string> const & args);
    static bool checkOutputFile(std::vector<std::string> const & args);
    static bool checkParticleCount(int particleCount);
    static bool checkParticleCountMatch(int headerCount, int fileCount);
};
