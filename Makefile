CC = gcc
CFLAGS = -Wall -Wextra -pedantic -funroll-loops -ffast-math -O3 -march=native -Iinclude
TARGET = numc_example
BENCHMARK_TARGET = benchmarks/c_benchmark
SRC = src/*/*.c
EXAMPLE = example/numc_example.c
BENCHMARK_SRC = benchmarks/c_bm.c

ifeq ($(PROFILE), 1)
    CFLAGS += -pg
endif

all: $(TARGET)

$(TARGET): $(SRC) $(EXAMPLE)
		$(CC) $(CFLAGS) -o $@ $^ -lm

$(BENCHMARK_TARGET): $(SRC) $(BENCHMARK_SRC)
		@$(CC) $(CFLAGS) -o $@ $^ -lm

run: $(TARGET)
		./$(TARGET)

run_benchmark: $(BENCHMARK_TARGET)
	@echo "=== C Benchmark ==="
	@./$(BENCHMARK_TARGET)
	@echo ""
	@echo "=== Python Benchmark ==="
	@python benchmarks/py_bm.py

.PHONY: all clean run_benchmark
clean:
		rm -f $(TARGET) $(BENCHMARK_TARGET)
