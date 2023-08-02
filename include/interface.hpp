#include <fstream>
#include <regex>
#include <string>

#include "process_manager.hpp"

class Interface {
    public:
        class Input {
            public:
                /**
                 * @brief Construct a new Input object
                 */
                Input();

                /**
                 * @brief Destroy the Input object
                 */
                ~Input() = default;

                /**
                 * @brief Get the input from terminal and store it
                 */
                void update_input();

                /**
                 * @brief Get the command type from stored input
                 *
                 * @return Command The command type
                 */
                Command get_command();

                /**
                 * @brief Get the instruction amount from stored input
                 *
                 * @return uint16_t The instruction amount
                 */
                uint16_t get_instruction_amount();

                /**
                 * @brief Get the memory size from stored input
                 *
                 * @return uint16_t The memory size
                 */
                uint16_t get_memory_size();

                /**
                 * @brief Get the process id from stored input
                 *
                 * @return uint16_t The process id
                 */
                uint16_t get_process_id() const;

                /**
                 * @brief Print help message on input screen
                 */
                void print_help();

                /**
                 * @brief Print invalid command message on input screen
                 */
                void print_invalid_command();

                /**
                 * @brief Print invalid pid error message on output screen
                 */
                void print_invalid_pid();

                /**
                 * @brief Print insufficient memory error message on output screen
                 */
                void print_insufficient_memory();

            private:
                /**
                 * @brief The input command read from terminal
                 */
                std::string input_command;

                /**
                 * @brief The input command matches
                 */
                std::smatch input_matches;
        };

        class Output {
            public:
                /**
                 * @brief Construct a new Output object
                 */
                Output();

                /**
                 * @brief Destroy the Output object
                 */
                ~Output();

                /**
                 * @brief
                 */
                void update_output(const ProcessManager& process_manager);

                /**
                 * @brief Adapt text to be placed vertically in any column
                 *
                 * @param x_pos The column to place the text
                 * @param text The text to be placed
                 */
                static std::string place_vertical(uint16_t x_pos, std::string text);

            private:
                /**
                 * @brief The stream to output screen
                 */
                std::ofstream out;
        };

        /**
         * @brief Object to interact with the input screen
         */
        Input input;

        /**
         * @brief Object to interact with the output screen
         */
        Output output;
};
