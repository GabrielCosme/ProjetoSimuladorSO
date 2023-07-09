#include <regex>
#include <string>

#include "process_manager.hpp"

const std::regex INSTRUCTION_ARG("-i(\\s*)([0-9]+)");
const std::regex MEMORY_ARG("-m(\\s*)([0-9]+)");
const std::regex CREATE_COMMAND(
    "c(reate)?(\\s+)((-i(\\s*)([0-9]+)(\\s+)-m(\\s*)([0-9]+))|(-m(\\s*)([0-9]+)(\\s+)-i(\\s*)([0-9]+)))");

const std::regex RUN_COMMAND("(r(un)?)?");
const std::regex KILL_COMMAND("k(ill)?(\\s+)([0-9]+)");
const std::regex HELP_COMMAND("h(elp)?");
const std::regex EXIT_COMMAND("e(xit)?");

int main() {
    Memory memory;
    ProcessManager process_manager(memory);

    std::string input_command;
    std::smatch input_matches;

    while (true) {
        std::cout << memory << std::endl;
        std::cout << process_manager << std::endl;

        std::getline(std::cin, input_command);

        if (std::regex_match(input_command, input_matches, CREATE_COMMAND)) {
            std::regex_search(input_command, input_matches, INSTRUCTION_ARG);
            uint16_t instruction_amount = std::stoi(input_matches[2]);

            std::regex_search(input_command, input_matches, MEMORY_ARG);
            uint16_t memory_size = std::stoi(input_matches[2]);

            process_manager.plan_create(instruction_amount, memory_size);
        } else if (std::regex_match(input_command, input_matches, KILL_COMMAND)) {
            uint16_t process_id = std::stoi(input_matches[3]);

            process_manager.plan_kill(process_id);
        } else if (std::regex_match(input_command, input_matches, EXIT_COMMAND)) {
            break;
        } else if (std::regex_match(input_command, input_matches, RUN_COMMAND)) {
            try {
                process_manager.run_process();
            } catch (std::out_of_range& e) {
                std::cout << "Process not found" << std::endl;
            } catch (std::runtime_error& e) {
                std::cout << "Not enough memory to create process" << std::endl;
            }
        } else if (std::regex_match(input_command, input_matches, HELP_COMMAND)) {
            std::cout << "<empty>|r|run: run one clock step" << std::endl;
            std::cout << "create -i <instruction_amount> -m <memory_size>: create process" << std::endl;
            std::cout << "kill <process_id>: kill process" << std::endl;
        } else {
            std::cout << "Invalid command " << input_command << std::endl;
            std::cout << "use \"help\" to get options" << std::endl;
        }

        std::cout << "---------------------------------------------------------------------" << std::endl;
    }

    return 0;
}
