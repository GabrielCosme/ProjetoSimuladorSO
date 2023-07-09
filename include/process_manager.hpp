#include <list>
#include <map>

#include "process.hpp"

#ifndef __PROCESS_MANAGER_HPP__
#define __PROCESS_MANAGER_HPP__

enum ProcessCommand {
    CREATE,
    RUN,
    KILL
};

typedef struct {
    ProcessCommand command;
    uint16_t       id;
} ProcessQueueItem;

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
        std::map<uint16_t, Process> processes;
        std::list<ProcessQueueItem> process_queue;
};

#endif // __PROCESS_MANAGER_HPP__
