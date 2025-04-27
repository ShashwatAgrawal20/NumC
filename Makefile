CC = gcc
CFLAGS = -Wall -Wextra -pedantic -Iinclude
TARGET = numc_example
SRC = src/*/*.c
EXAMPLE = example/numc_example.c

ifeq ($(PROFILE), 1)
    CFLAGS += -pg
endif

all: $(TARGET)

$(TARGET): $(SRC) $(EXAMPLE)
		$(CC) $(CFLAGS) -o $@ $^ -lm

run: $(TARGET)
		./$(TARGET)

.PHONY: all clean
clean:
		rm -f $(TARGET)
