#include <string>

#include "process_manager.hpp"

int main() {
    Memory memory;
    ProcessManager process_manager(memory);

    std::string input_command;

    while (true) {
        std::cout << memory << std::endl;
        std::cout << process_manager << std::endl;
        std::cin >> input_command;

        if (input_command == "create") {
            uint16_t instruction_amount, memory_size;

            std::cin >> instruction_amount >> memory_size;

            process_manager.plan_create(instruction_amount, memory_size);
        } else if (input_command == "kill") {
            uint16_t process_id;

            std::cin >> process_id;

            process_manager.plan_kill(process_id);
        } else if (input_command == "exit") {
            break;
        } else {
            try {
                process_manager.run_process();
            } catch (std::out_of_range& e) {
                std::cout << "Process not found" << std::endl;
            } catch (std::runtime_error& e) {
                std::cout << "Not enough memory to create process" << std::endl;
            }
        }

        std::cout << "------------------------" << std::endl;
    }

    return 0;
}
