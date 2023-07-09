#include <stdexcept>

#include "memory.hpp"

Memory::Memory() {
    this->bitmap.fill(false);

    for (auto addr: ALLOCATED_ADDRESSES) {
        this->bitmap[addr] = true;
    }
}

uint16_t Memory::allocate(uint16_t size) {
    uint16_t start = 0;
    uint16_t count = 0;

    // Fist-fit algorithm
    for (uint16_t i = 0; i < MEMORY_SIZE; i++) {
        if (this->bitmap[i]) {
            start = i + 1;
            count = 0;
        } else {
            count++;
        }

        if (count == size) {
            for (uint16_t j = start; j < start + size; j++) {
                this->bitmap[j] = true;
            }

            return start;
        }
    }

    throw std::runtime_error("Out of memory");
}

void Memory::free(uint16_t start, uint16_t size) {
    for (uint16_t i = start; i < start + size; i++) {
        this->bitmap[i] = false;
    }
}

std::ostream& operator <<(std::ostream& output, const Memory& memory) {
    for (auto bit: memory.bitmap) {
        output << bit;
    }

    return output;
}
