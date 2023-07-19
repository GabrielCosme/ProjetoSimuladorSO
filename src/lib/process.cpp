#include "process.hpp"

Process::Process(uint16_t id, uint16_t instruction_amount, uint16_t memory_size, Memory& memory) :
    id(id), instruction_amount(instruction_amount), memory_size(memory_size), memory(memory) {
}

Process::~Process() {
    this->kill();
}

void Process::create() {
    this->start_address = this->memory.allocate(this->id, this->memory_size);
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
