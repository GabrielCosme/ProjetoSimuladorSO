#include <cstdint>

#include "process.hpp"

Process::Process(uint16_t id, uint16_t instruction_amount, uint16_t memory_size, Memory& memory) :
    id(id), instruction_amount(instruction_amount), memory_size(memory_size), memory(memory) {
}

void Process::create() {
    this->start_address = this->memory.allocate(this->memory_size);
}

bool Process::run() {
    this->program_counter++;

    if (this->program_counter >= this->instruction_amount) {
        return true;
    }

    return false;
}

void Process::kill() {
    this->memory.free(this->start_address, this->memory_size);
}

uint16_t Process::get_id() const {
    return this->id;
}

std::ostream& operator <<(std::ostream& output, const Process& process) {
    output << "| PID: " << process.id <<
    " | PC: " << process.program_counter <<
    " | NUM_INST: " << process.instruction_amount <<
    " | MEM_ADDR: " << process.start_address <<
    " | MEM_SZ: " << process.memory_size << " |";

    return output;
}
