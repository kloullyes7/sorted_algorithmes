CC = gcc
CFLAGS = -std=c11 -O2 -Wall -Wextra -pedantic -D_POSIX_C_SOURCE=200809L
TARGET = sorting
SOURCE = src/main.c

all: $(TARGET)

$(TARGET): $(SOURCE)
	$(CC) $(CFLAGS) $(SOURCE) -o $(TARGET)

clean:
	rm -f $(TARGET)

.PHONY: all clean
