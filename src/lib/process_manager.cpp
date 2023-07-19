#include "process_manager.hpp"

ProcessManager::ProcessManager() {
    for (auto& task: START_PROCESSES) {
        this->plan_create(task.first, task.second);
    }
}

void ProcessManager::plan_create(uint16_t instruction_amount, uint16_t memory_size) {
    this->id_counter++;

    this->processes.try_emplace(this->id_counter, this->id_counter, instruction_amount, memory_size, this->memory);
    this->tasks_queue.emplace_back(Command::CREATE, this->id_counter);
}

void ProcessManager::run_first_task() {
    if (this->tasks_queue.empty()) {
        return;
    }

    auto first_task = this->tasks_queue.front();

    switch (first_task.command) {
        case Command::CREATE: {
            this->tasks_queue.pop_front();
            this->processes.at(first_task.process_id).create();
            this->tasks_queue.emplace_back(Command::RUN, first_task.process_id);
            break;
        }

        case Command::RUN: {
            if (this->processes.at(first_task.process_id).run()) {
                this->quantum_counter = 0;
                this->tasks_queue.pop_front();
                this->processes.erase(first_task.process_id);
                break;
            }

            if (USE_ROUND_ROBIN and (++this->quantum_counter) >= QUANTUM) {
                this->quantum_counter = 0;
                this->tasks_queue.pop_front();
                this->tasks_queue.emplace_back(Command::RUN, first_task.process_id);
            }

            break;
        }

        case Command::KILL: {
            this->tasks_queue.remove_if(
                [&first_task](Task task) {
                    return task.process_id == first_task.process_id;
                });

            this->processes.erase(first_task.process_id);

            if (AUTOMATIC_DEFRAGMENTATION) {
                this->defragment_memory();
            }

            break;
        }

        default:
            break;
    }
}

void ProcessManager::plan_kill(uint16_t process_id) {
    this->tasks_queue.emplace_back(KILL, process_id);
}

void ProcessManager::defragment_memory() {
    uint16_t fragmented_process_id;

    while ((fragmented_process_id = this->memory.get_first_fragmented_process()) != 0) {
        this->processes.at(fragmented_process_id).kill();
        this->processes.at(fragmented_process_id).create();
    }
}
