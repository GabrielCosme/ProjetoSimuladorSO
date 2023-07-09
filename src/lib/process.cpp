#include <cstdint>

#include "process.hpp"

Memory Process::memory;

Process::Process(uint16_t instruction_amount, uint16_t memory_size) :
    instruction_amount(instruction_amount), memory_size(memory_size) {
    this->id = id_counter++;
}

void Process::create() {
    this->start_address = Process::memory.allocate(this->memory_size);
}

bool Process::run() {
    this->program_counter++;

    if (this->program_counter >= this->instruction_amount) {
        return true;
    }

    return false;
}

void Process::kill() {
    Process::memory.free(this->start_address, this->memory_size);
}

uint16_t Process::get_id() const {
    return this->id;
}

std::ostream& operator <<(std::ostream& output, const Process& process) {
    output << "Process #" << process.id << std::endl;
    return output;
}
