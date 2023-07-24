#include <stdexcept>

#include "memory.hpp"

Memory::Memory() {
    this->bitmap.fill(false);
}

uint16_t Memory::allocate(uint16_t process_id, uint16_t size) {
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
                this->bitmap[j] = process_id;
            }

            return start;
        }
    }

    throw std::length_error("Out of memory");
}

void Memory::free(uint16_t start, uint16_t size) {
    for (uint16_t i = start; i < start + size; i++) {
        this->bitmap[i] = 0;
    }
}

uint16_t Memory::get_first_fragmented_process() {
    for (uint16_t i = 1; i < MEMORY_SIZE; i++) {
        if (this->bitmap[i] != 0 and this->bitmap[i - 1] == 0) {
            return this->bitmap[i];
        }
    }

    return 0;
}
