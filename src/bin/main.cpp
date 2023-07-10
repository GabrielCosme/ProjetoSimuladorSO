#include "interface.hpp"

int main() {
    ProcessManager process_manager;
    Interface interface;

    while (true) {
        interface.output.update_output(process_manager);
        interface.input.update_input();

        switch (interface.input.get_command()) {
            case Command::CREATE: {
                uint16_t instruction_amount = interface.input.get_instruction_amount();
                uint16_t memory_size = interface.input.get_memory_size();

                process_manager.plan_create(instruction_amount, memory_size);
                break;
            }

            case Command::KILL: {
                uint16_t process_id = interface.input.get_process_id();

                process_manager.plan_kill(process_id);
                break;
            }

            case Command::EXIT: {
                return 0;
            }

            case Command::RUN: {
                try {
                    process_manager.run_process();
                } catch (std::out_of_range& e) {
                    interface.output.print_invalid_pid();
                } catch (std::length_error& e) {
                    interface.output.print_insufficient_memory();
                }

                break;
            }

            case Command::HELP: {
                interface.input.print_help();
                break;
            }

            case Command::INVALID: {
                interface.input.print_invalid();
                break;
            }
        }
    }

    return 0;
}
