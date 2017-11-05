CC = g++
CFLAGS = -std=c++14 -Wall -Wextra -Wpedantic
# DEBUG mode
CFLAGS += -g
# RELEASE mode
# CFLAGS += -O2 -DNDEBUG
LDFLAGS = -lm

SIM_DIR = sim
SIM_NAME = riscvsim
SIM_SOURCES = $(wildcard $(SIM_DIR)/*.cpp)
SIM_OBJECTS = $(SIM_SOURCES:$(SIM_DIR)/%.cpp=build/%.o)

all: code-style build build/$(SIM_NAME)

code-style:
	clang-format -i -style=file $(SIM_DIR)/*.h $(SIM_DIR)/*.cpp

build:
	mkdir build

build/$(SIM_NAME): $(SIM_OBJECTS)
	$(CC) $(LDFLAGS) $(SIM_OBJECTS) -o build/$(SIM_NAME)

build/%.o: $(SIM_DIR)/%.cpp
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf build/*
