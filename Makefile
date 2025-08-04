# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -pedantic -funroll-loops -ffast-math -O3 -march=native -Iinclude
LDFLAGS = -lm

ifeq ($(PROFILE), 1)
    CFLAGS += -pg
endif

# Directories
SRCDIR = src
EXAMPLEDIR = example
BENCHDIR = benchmarks
TESTDIR = tests
BUILDDIR = build

# Targets
TARGET = numc_example
BENCHMARK_TARGET = $(BENCHDIR)/c_benchmark
TEST_RUNNER_SRC = $(TESTDIR)/test_runner.c
TEST_RUNNER_BIN = $(TESTDIR)/test_runner

# Source files
SRC_FILES := $(wildcard $(SRCDIR)/*/*.c)
EXAMPLE_SRC = $(EXAMPLEDIR)/numc_example.c
BENCHMARK_SRC = $(BENCHDIR)/c_bm.c
TEST_SRC := $(shell find $(TESTDIR) -name '*.c')

# Object files
OBJ_FILES := $(patsubst %.c, $(BUILDDIR)/%.o, $(SRC_FILES))
EXAMPLE_OBJ := $(patsubst %.c, $(BUILDDIR)/%.o, $(EXAMPLE_SRC))
BENCHMARK_OBJ := $(patsubst %.c, $(BUILDDIR)/%.o, $(BENCHMARK_SRC))
TEST_OBJ := $(patsubst %.c, $(BUILDDIR)/%.o, $(TEST_SRC))

# Default rule
.PHONY: all clean run run_benchmark test help

all: $(TARGET)

# Build the main target
$(TARGET): $(OBJ_FILES) $(EXAMPLE_OBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

# Build the benchmark target
$(BENCHMARK_TARGET): $(OBJ_FILES) $(BENCHMARK_OBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

# Generic object build rule
$(BUILDDIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# Run example
run: $(TARGET)
	./$(TARGET)

# Run benchmark
run_benchmark: $(BENCHMARK_TARGET)
	@echo "=== C Benchmark ==="
	@./$(BENCHMARK_TARGET)
	@echo ""
	@echo "=== Python Benchmark ==="
	@python $(BENCHDIR)/py_bm.py

test: $(TEST_RUNNER_BIN)
	@./$(TEST_RUNNER_BIN)

# Build test runner binary
$(TEST_RUNNER_BIN): $(OBJ_FILES) $(TEST_OBJ)
	$(CC) $(CFLAGS) -Iinclude -o $@ $^ $(LDFLAGS)

# Clean everything
clean:
	rm -rf $(BUILDDIR) $(TARGET) $(BENCHMARK_TARGET) $(TEST_RUNNER_BIN)

# Show help
help:
	@echo "Available targets:"
	@echo "  all             - Build main executable"
	@echo "  run             - Run the main program"
	@echo "  run_benchmark   - Run both C and Python benchmarks"
	@echo "  test            - Generate and run tests"
	@echo "  clean           - Clean build artifacts"
