#include <list>
#include <unordered_map>

#include "process.hpp"

#ifndef __PROCESS_MANAGER_HPP__
#define __PROCESS_MANAGER_HPP__

enum Command {
    CREATE,
    RUN,
    KILL,
    DEFRAG,
    HELP,
    EXIT,
    INVALID
};

struct Task {
    Command  command;
    uint16_t process_id;
};

class ProcessManager {
    public:
        /**
         * @brief Construct a new ProcessManager object
         */
        ProcessManager();

        /**
         * @brief Destroy the ProcessManager object
         */
        ~ProcessManager() = default;

        /**
         * @brief Add the task to create the process to the queue
         *
         * @param instruction_amount The amount of instructions the process has
         * @param memory_size The size of the memory block the process will use
         */
        void plan_create(uint16_t instruction_amount, uint16_t memory_size);

        /**
         * @brief Run the first task in the queue
         */
        void run_first_task();

        /**
         * @brief Add the task to kill the process to the queue
         */
        void plan_kill(uint16_t process_id);

        /**
         * @brief Defragment the memory
         */
        void defragment_memory();

        /**
         * @brief Output stream operator overloading
         */
        friend std::ostream& operator <<(std::ostream& output, const ProcessManager& process_manager);

    private:
        /**
         * @brief Shared Memory object
         */
        Memory memory;

        /**
         * @brief The number id of the last process created (0 is reserved for empty memory)
         */
        uint16_t id_counter = 0;

        /**
         * @brief Counter for the round-robin algorithm
         */
        uint16_t quantum_counter = 0;

        /**
         * @brief Map to store the processes using the id as key
         */
        std::unordered_map<uint16_t, Process> processes;

        /**
         * @brief Queue to store the tasks to be executed
         */
        std::list<Task> tasks_queue;
};

#endif // __PROCESS_MANAGER_HPP__
