# Architecture in C++

## 📌 Project Overview
This project is designed for **numerical simulations** with a focus on **fluid dynamics and particle-based computations**. It is implemented in **C++** and utilizes **CMake** for building and testing.

## 🚀 Getting Started

### 📋 Prerequisites
Ensure you have the following installed before compiling the project:
- C++ Compiler (GCC, Clang, MSVC)
- CMake
- Make (Linux/macOS) or Visual Studio (Windows)

### 🔧 Installation
Clone the repository:
```sh
git clone https://github.com/your_username/Architecture-in-c.git
cd Architecture-in-c-main
```

### 🏗 Building the Project
Use **CMake** to configure and compile the project:
```sh
mkdir build
cd build
cmake ..
make
```

To compile on Windows with Visual Studio:
```sh
mkdir build
cd build
cmake .. -G "Visual Studio 16 2019"
```

## 🏗 Project Structure
```
Architecture-in-c/
│── CMakeLists.txt          # Project build configuration
│── ComandosCompilarProyecto.txt  # Compilation instructions
│── fluid/                  # Fluid simulation module
│    ├── fluid.cpp
│    ├── CMakeLists.txt
│── sim/                    # Core simulation logic
│    ├── block.cpp
│    ├── block.hpp
│    ├── grid.cpp
│    ├── grid.hpp
│    ├── simulation.cpp
│    ├── simulation.hpp
│    ├── utils.cpp
│    ├── utils.hpp
│── utest/                  # Unit tests
│    ├── block_test.cpp
│    ├── grid_test.cpp
│    ├── simulation_test.cpp
│── in/                     # Input data files
│    ├── large.fld
│    ├── small.fld
│── out/                    # Output simulation results
│    ├── large_output.fld
│    ├── small_output.fld
```

## ✅ Running Tests
To run unit tests after compilation:
```sh
ctest --output-on-failure
```
Or manually execute the test binaries in the `build/` directory.

## 🛠 Built With
- **C++**
- **CMake**
- **Google Test (if applicable)**

## 🤝 Contributing
If you wish to contribute, feel free to fork the repository, make improvements, and submit a pull request.

---
