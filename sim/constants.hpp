// constants.hpp
#pragma once

constexpr float PI                           = 3.14159265358979323846F;
constexpr float SMALL_NUMBER                 = 1e-10F;
constexpr float const CERO                   = 0.0F;
constexpr float const HALF                   = 0.5F;
constexpr float const STIFFNESS_CONSTANT     = 315.0F;
constexpr float const VISCOSITY_CONSTANT     = 45.0F;
constexpr float const PRESSURE_TERM_CONSTANT = 15.0F;
constexpr float const DENSITY_MULTIPLIER     = 64.0F;

// Physical parameters of the simulation
constexpr float rho = 1000.0F;  // Fluid density (kg/m^3)
constexpr float r   = 1.695F;   // Smoothing length radius (m)
constexpr float mu  = 0.4F;     // Fluid dynamic viscosity (Pa·s)
constexpr float g   = 9.81F;    // Gravity acceleration (m/s^2)

// External acceleration
constexpr float a_ext_x = 0.0F;
constexpr float a_ext_y = -g;
constexpr float a_ext_z = 0.0F;

// Simulation box parameters
constexpr float xmin = -0.065F, xmax = 0.065F;
constexpr float ymin = -0.08F, ymax = 0.1F;
constexpr float zmin = -0.065F, zmax = 0.065F;

// Collision parameters
constexpr float dp = 0.0002F;   // Penetration distance
constexpr float sc = 30000.0F;  // Spring constant
constexpr float dv = 128.0F;    // Damping coefficient

// Integration parameters
constexpr float delta_t = 0.001F;  // Time step for integration

// SPH kernel parameters
constexpr float ps = 3.0F;  // Static pressure

// errors
constexpr int const ERROR_INCORRECT_ARG_COUNT    = -1;
constexpr int const ERROR_INVALID_FIRST_ARG      = -1;
constexpr int const ERROR_NEGATIVE_ITERATIONS    = -2;
constexpr int const ERROR_INPUT_FILE_OPEN        = -3;
constexpr int const ERROR_OUTPUT_FILE_OPEN       = -4;
constexpr int const ERROR_INVALID_PARTICLE_COUNT = -5;
