#include <array>
#include <iostream>

#include "constants.hpp"

#ifndef __MEMORY_HPP__
#define __MEMORY_HPP__

class Memory {
    public:
        /**
         * @brief Construct a new Memory object
         */
        Memory();

        /**
         * @brief Destroy the Memory object
         */
        ~Memory() = default;

        /**
         * @brief Allocate a block of memory and return its initial address
         *
         * @param size The size of the block to allocate
         * @return uint16_t The initial address of the block
         */
        uint16_t allocate(uint16_t size);

        /**
         * @brief Free a block of memory
         *
         * @param start The start memory to deallocate
         * @param end The end memory to deallocate
         */
        void free(uint16_t start, uint16_t size);

        /**
         * @brief Output stream operator overloading
         */
        friend std::ostream& operator <<(std::ostream& output, const Memory& memory);

    private:
        /**
         * @brief The bit map array
         */
        std::array<bool, MEMORY_SIZE> bitmap;
};

#endif // __MEMORY_HPP__
