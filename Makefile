CC = g++
CFLAGS = -std=c++14 -Wall -Wextra -Wpedantic -Wno-packed-bitfield-compat
# DEBUG mode
CFLAGS += -g
# RELEASE mode
# CFLAGS += -O2 -DNDEBUG
LDFLAGS = -l elf -lm

SIM_DIR = sim
SIM_NAME = riscvsim
SIM_SOURCES = $(wildcard $(SIM_DIR)/*.cpp)
SIM_OBJECTS = $(SIM_SOURCES:$(SIM_DIR)/%.cpp=build/%.o)

all: code-style sim

code-style:
	clang-format -i -style=file $(SIM_DIR)/*.h $(SIM_DIR)/*.cpp

sim: build build/$(SIM_NAME)

build:
	mkdir build

build/$(SIM_NAME): $(SIM_OBJECTS)
	$(CC) $(LDFLAGS) $(SIM_OBJECTS) -o build/$(SIM_NAME)

build/%.o: $(SIM_DIR)/%.cpp
	$(CC) $(CFLAGS) -c $< -o $@

profile: build/$(SIM_NAME)
	rm -f callgrind.out
	valgrind --tool=callgrind --callgrind-out-file=callgrind.out ./build/$(SIM_NAME)
	kcachegrind callgrind.out

clean:
	rm -rf build/*
