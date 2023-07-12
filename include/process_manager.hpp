#include <list>
#include <unordered_map>

#include "process.hpp"

#ifndef __PROCESS_MANAGER_HPP__
#define __PROCESS_MANAGER_HPP__

enum Command {
    CREATE,
    RUN,
    KILL,
    HELP,
    EXIT,
    INVALID
};

typedef struct {
    Command  command;
    uint16_t process_id;
} Task;

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
         * @brief Create a process object
         *
         * @param instruction_amount The amount of instructions the process has
         * @param memory_size The size of the memory block the process will use
         */
        void plan_create(uint16_t instruction_amount, uint16_t memory_size);

        /**
         * @brief Run the process
         */
        void run_process();

        /**
         * @brief Kill the process
         */
        void plan_kill(uint16_t process_id);

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
         * @brief The id of the next process created
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
