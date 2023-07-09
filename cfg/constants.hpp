#include <cstdint>
#include <utility>
#include <vector>

#ifndef __CONSTANTS_HPP__
#define __CONSTANTS_HPP__

constexpr uint16_t MEMORY_SIZE = 20;

const std::vector<uint16_t> ALLOCATED_ADDRESSES = {0, 2, 3, 5, 7};

const std::vector<std::pair<uint16_t, uint16_t>> START_PROCESSES = {{2, 4}, {5, 4}};

constexpr bool USE_ROUND_ROBIN = true;
constexpr uint16_t QUANTUM = 2;

#endif
