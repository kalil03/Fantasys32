# Makefile da VM Fantasys32
# Compila com g++ (C++17) e linka a SDL2.

CXX      = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2 $(shell pkg-config --cflags sdl2)
LDFLAGS  = $(shell pkg-config --libs sdl2) -lm

ALVO    = vm
FONTES  = main.cpp vm.cpp
OBJ     = $(FONTES:.cpp=.o)

all: $(ALVO)

$(ALVO): $(OBJ)
	$(CXX) $(CXXFLAGS) $(OBJ) -o $(ALVO) $(LDFLAGS)

%.o: %.cpp vm.hpp fonte.hpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(ALVO)

.PHONY: all clean
