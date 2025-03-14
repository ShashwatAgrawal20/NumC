CC = gcc
CFLAGS = -Wall -Wextra
TARGET = numc
SOURCE = src/numc.c

all: $(TARGET)

$(TARGET): $(SOURCE)
		$(CC) $(CFLAGS) -o $@ $^

run: $(TARGET)
		./numc

.PHONY: all clean
clean:
		rm -f $(TARGET)
