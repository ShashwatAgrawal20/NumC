# NumC: A NumPy-like numerical computing library in C

## Build and Installation
A standard C compiler (GCC or Clang), make, and CMake are required.

## Development (Building from Source)
The Makefile is provided for development on NumC itself.

```bash
# Build the static library
make

# Run the test suite
make test

# Clean build artifacts
make clean
```

## Installation (System-wide for Consumers)
For using NumC as a dependency in another project, a CMake-based installation
is the standard method. This installs the library and headers to a
system-standard location

```bash
# Create an out-of-source build directory
mkdir build && cd build

# Configure and generate the build system
cmake..

# Compile the library
make

# Install the library and headers (may require sudo)
sudo make install
```
