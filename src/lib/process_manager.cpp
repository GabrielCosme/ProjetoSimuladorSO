#include "process_manager.hpp"

ProcessManager::ProcessManager() {
    for (auto& task: START_PROCESSES) {
        this->plan_create(task.first, task.second);
    }
}

void ProcessManager::plan_create(uint16_t instruction_amount, uint16_t memory_size) {
    this->processes.try_emplace(this->id_counter, this->id_counter, instruction_amount, memory_size, this->memory);
    this->tasks_queue.emplace_back(CREATE, this->id_counter);

    this->id_counter++;
}

void ProcessManager::run_process() {
    if (this->tasks_queue.empty()) {
        return;
    }

    auto first_task = this->tasks_queue.front();

    switch (first_task.command) {
        case CREATE: {
            this->tasks_queue.pop_front();
            this->processes.at(first_task.process_id).create();
            this->tasks_queue.emplace_back(RUN, first_task.process_id);
            break;
        }

        case RUN: {
            if (this->processes.at(first_task.process_id).run()) {
                this->quantum_counter = 0;
                this->tasks_queue.pop_front();
                this->processes.erase(first_task.process_id);
                break;
            }

            if (USE_ROUND_ROBIN and (++this->quantum_counter) >= QUANTUM) {
                this->quantum_counter = 0;
                this->tasks_queue.pop_front();
                this->tasks_queue.emplace_back(RUN, first_task.process_id);
            }

            break;
        }

        case KILL: {
            this->tasks_queue.remove_if(
                [&first_task](Task task) {
                    return task.process_id == first_task.process_id;
                });

            this->processes.erase(first_task.process_id);
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
    if (process_manager.tasks_queue.empty()) {
        return output;
    }

    output << "|";

    for (auto task: process_manager.tasks_queue) {
        if (task.command == CREATE) {
            output << "create" << "|";
        } else if (task.command == RUN) {
            output << "PID " << task.process_id << "|";
        } else if (task.command == KILL) {
            output << "kill " << task.process_id << "|";
        }
    }

    auto first_task = process_manager.tasks_queue.front();

    if (first_task.command == RUN) {
        output << std::endl << process_manager.processes.at(first_task.process_id);
    }

    return output;
}
