#include "process_manager.hpp"

ProcessManager::ProcessManager() {
    for (auto& c: START_PROCESSES) {
        this->plan_create(c.first, c.second);
    }
}

void ProcessManager::plan_create(uint16_t instruction_amount, uint16_t memory_size) {
    Process process(instruction_amount, memory_size);

    this->processes.emplace(process.get_id(), process);
    this->process_queue.push_back({CREATE, process.get_id()});
}

void ProcessManager::run_process() {
    if (this->process_queue.empty()) {
        return;
    }

    auto first_process = this->process_queue.front();

    switch (first_process.command) {
        case CREATE: {
            this->processes.at(first_process.id).create();
            this->process_queue.pop_front();
            this->process_queue.emplace_back(RUN, first_process.id);
            break;
        }

        case RUN: {
            if (this->processes.at(first_process.id).run()) {
                this->process_queue.pop_front();
                break;
            }

            if (is_round_robin) {
                this->process_queue.pop_front();
                this->process_queue.emplace_back(RUN, first_process.id);
            }

            break;
        }

        case KILL: {
            this->processes.at(first_process.id).kill();
            this->processes.erase(first_process.id);

            this->process_queue.remove_if(
                [&first_process](ProcessQueueItem item) {
                    return item.id == first_process.id;
                });

            break;
        }

        default:
            break;
    }
}

void ProcessManager::plan_kill(uint16_t process_id) {
    this->process_queue.emplace_back(KILL, process_id);
}

std::ostream& operator <<(std::ostream& output, const ProcessManager& process_manager) {
    output << "Process Manager" << std::endl;
    return output;
}
