#include <string>

#include "process_manager.hpp"

int main() {
    Memory memory;
    ProcessManager process_manager(memory);

    std::string input_command;

    while (true) {
        std::cin >> input_command;
        std::cout << input_command << std::endl;

        if (input_command == "create") {
            uint16_t instruction_amount, memory_size;

            std::cin >> instruction_amount >> memory_size;

            process_manager.plan_create(instruction_amount, memory_size);
        } else if (input_command == "kill") {
            uint16_t process_id;

            std::cin >> process_id;

            process_manager.plan_kill(process_id);
        } else if (input_command == "run") {
            process_manager.run_process();
        } else if (input_command == "exit") {
            break;
        }

        process_manager.run_process();
    }

    return 0;
}
