# Makefile da VM Fantasys32
# Compila com g++ (C++17). SDL2 será adicionado a partir da Fase 3.

CXX      = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2
# Quando o SDL2 entrar (Fase 3), descomentar:
# LDFLAGS = $(shell sdl2-config --libs)
# CXXFLAGS += $(shell sdl2-config --cflags)

ALVO    = vm
FONTES  = main.cpp vm.cpp
OBJ     = $(FONTES:.cpp=.o)

all: $(ALVO)

$(ALVO): $(OBJ)
	$(CXX) $(CXXFLAGS) $(OBJ) -o $(ALVO) $(LDFLAGS)

%.o: %.cpp vm.hpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(ALVO)

.PHONY: all clean
