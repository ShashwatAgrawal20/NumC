cmake_minimum_required(VERSION 3.12)
project(NumC C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Collect all source files
file(GLOB_RECURSE SRC_FILES "src/*.c")

# Create the library
add_library(numc STATIC ${SRC_FILES})

# Set proper include path (includes are like: #include <numc/numc.h>)
target_include_directories(numc
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

# Build example
add_executable(numc_example example/numc_example.c)
target_link_libraries(numc_example PRIVATE numc)

# Install instructions
install(TARGETS numc DESTINATION lib)
install(DIRECTORY include/ DESTINATION include)

include(CPack)
