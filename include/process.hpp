#include "memory.hpp"

#ifndef __PROCESS_HPP__
#define __PROCESS_HPP__

class Process {
    public:
        /**
         * @brief Construct a new Process object
         *
         * @param instruction_amount The amount of instructions the process has
         * @param memory_size The size of the memory block the process will use
         * @param memory The memory object
         */
        Process(uint16_t id, uint16_t instruction_amount, uint16_t memory_size, Memory& memory);

        /**
         * @brief Destroy the Process object
         */
        ~Process();

        /**
         * @brief Create the process
         */
        void create();

        /**
         * @brief Run the process
         *
         * @return bool Whether the process has finished running
         */
        bool run();

        /**
         * @brief Kill the process
         */
        void kill();

        /**
         * @brief Get the process id
         *
         * @return uint16_t The id
         */
        uint16_t get_id() const;

        /**
         * @brief Output stream operator overloading
         */
        friend std::ostream& operator <<(std::ostream& output, const Process& process);

    private:
        /**
         * @brief The id used by the process
         */
        uint16_t id;

        /**
         * @brief The number of instructions the process has executed
         */
        uint16_t program_counter = 0;

        /**
         * @brief The number of instructions the process has
         */
        uint16_t instruction_amount;

        /**
         * @brief The start address in the memory block the process will use
         */
        uint16_t start_address;

        /**
         * @brief The size of the memory block the process will use
         */
        uint16_t memory_size;

        /**
         * @brief A reference to the memory object
         */
        Memory& memory;
};

#endif // __PROCESS_HPP__
