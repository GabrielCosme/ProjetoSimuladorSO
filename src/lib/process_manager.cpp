#include "process_manager.hpp"

ProcessManager::ProcessManager(Memory& memory) :
    memory(memory) {
    for (auto& task: START_PROCESSES) {
        this->plan_create(task.first, task.second);
    }
}

void ProcessManager::plan_create(uint16_t instruction_amount, uint16_t memory_size) {
    this->processes.try_emplace(this->id_counter, this->id_counter, instruction_amount, memory_size, this->memory);
    this->tasks_queue.emplace_back(CREATE, this->id_counter++);
}

void ProcessManager::run_process() {
    if (this->tasks_queue.empty()) {
        return;
    }

    auto first_process = this->tasks_queue.front();

    switch (first_process.command) {
        case CREATE: {
            this->tasks_queue.pop_front();
            this->processes.at(first_process.id).create();
            this->tasks_queue.emplace_back(RUN, first_process.id);
            break;
        }

        case RUN: {
            if (this->processes.at(first_process.id).run()) {
                this->tasks_queue.pop_front();
                this->processes.erase(first_process.id);
                break;
            }

            if (USE_ROUND_ROBIN) {
                this->tasks_queue.pop_front();
                this->tasks_queue.emplace_back(RUN, first_process.id);
            }

            break;
        }

        case KILL: {
            this->tasks_queue.remove_if(
                [&first_process](Task task) {
                    return task.id == first_process.id;
                });

            this->processes.erase(first_process.id);
            break;
        }

        default:
            break;
    }
}

void ProcessManager::plan_kill(uint16_t process_id) {
    this->tasks_queue.emplace_back(KILL, process_id);
}

std::ostream& operator <<(std::ostream& output, const ProcessManager& process_manager) {
    output << "|";

    for (auto task: process_manager.tasks_queue) {
        if (task.command == CREATE) {
            output << "create" << "|";
        } else if (task.command == RUN) {
            output << "PID " << task.id << "|";
        } else if (task.command == KILL) {
            output << "kill " << task.id << "|";
        }
    }

    auto first_task = process_manager.tasks_queue.front();

    if (first_task.command == RUN) {
        output << std::endl << process_manager.processes.at(first_task.id);
    }

    return output;
}
