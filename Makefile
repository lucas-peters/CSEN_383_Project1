CC = gcc
CFLAGS = -Wall -g
SRC = $(wildcard *.c)
OBJ = $(patsubst %.c, build/%.o, $(SRC))
TARGET = build/main

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

build/%.o: %.c | build
	$(CC) $(CFLAGS) -c $< -o $@

build:
	mkdir -p build

clean:
	rm -rf build