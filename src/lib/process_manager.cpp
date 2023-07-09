#include "process_manager.hpp"

ProcessManager::ProcessManager(Memory& memory) :
    memory(memory) {
    for (auto& c: START_PROCESSES) {
        this->plan_create(c.first, c.second);
    }
}

void ProcessManager::plan_create(uint16_t instruction_amount, uint16_t memory_size) {
    Process process(this->id_counter++, instruction_amount, memory_size, this->memory);

    this->processes.emplace(process.get_id(), process);
    this->processes_queue.push_back({CREATE, process.get_id()});
}

void ProcessManager::run_process() {
    if (this->processes_queue.empty()) {
        return;
    }

    auto first_process = this->processes_queue.front();

    switch (first_process.command) {
        case CREATE: {
            this->processes_queue.pop_front();
            this->processes.at(first_process.id).create();
            this->processes_queue.emplace_back(RUN, first_process.id);
            break;
        }

        case RUN: {
            if (this->processes.at(first_process.id).run()) {
                this->processes_queue.pop_front();
                this->processes.at(first_process.id).kill();
                break;
            }

            if (use_round_robin) {
                this->processes_queue.pop_front();
                this->processes_queue.emplace_back(RUN, first_process.id);
            }

            break;
        }

        case KILL: {
            this->processes_queue.remove_if(
                [&first_process](ProcessQueueItem item) {
                    return item.id == first_process.id;
                });

            this->processes.at(first_process.id).kill();
            this->processes.erase(first_process.id);

            break;
        }

        default:
            break;
    }
}

void ProcessManager::plan_kill(uint16_t process_id) {
    this->processes_queue.emplace_back(KILL, process_id);
}

std::ostream& operator <<(std::ostream& output, const ProcessManager& process_manager) {
    output << "|";

    for (auto p: process_manager.processes_queue) {
        if (p.command == CREATE) {
            output << "create" << "|";
        } else if (p.command == RUN) {
            output << "PID " << p.id << "|";
        } else if (p.command == KILL) {
            output << "kill " << p.id << "|";
        }
    }

    auto first_item = process_manager.processes_queue.front();

    if (first_item.command == RUN) {
        output << std::endl << process_manager.processes.at(first_item.id);
    }

    return output;
}
