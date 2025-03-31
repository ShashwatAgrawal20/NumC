CC = gcc
CFLAGS = -Wall -Wextra -pedantic
TARGET = numc
SOURCE = src/*.c

all: $(TARGET)

$(TARGET): $(SOURCE)
		$(CC) $(CFLAGS) -o $@ $^ -lm

run: $(TARGET)
		./numc

.PHONY: all clean
clean:
		rm -f $(TARGET)
