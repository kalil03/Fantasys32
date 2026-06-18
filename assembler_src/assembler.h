#include <vector>
#include <cstdint>

struct BinHeader {
    uint32_t dataSize;
};

void montar(const char* arqEntrada, const char* arqSaida);

