#include <fstream>
#include <iostream>
#include <regex>
#include <string>

#include "process_manager.hpp"

const std::regex INSTRUCTION_ARG(R"(-i(\s*)([0-9]+))");
const std::regex MEMORY_ARG(R"(-m(\s*)([0-9]+))");
const std::regex CREATE_COMMAND(
    R"(c(reate)?(\s+)((-i(\s*)([0-9]+)(\s+)-m(\s*)([0-9]+))|(-m(\s*)([0-9]+)(\s+)-i(\s*)([0-9]+))))");

const std::regex RUN_COMMAND(R"((r(un)?)?)");
const std::regex KILL_COMMAND(R"(k(ill)?(\s+)([0-9]+))");
const std::regex HELP_COMMAND(R"(h(elp)?)");
const std::regex EXIT_COMMAND(R"(e(xit)?)");

constexpr char CLEAR_SCREEN[] = "\033c";

int main() {
    ProcessManager process_manager;

    while (true) {
        out << CLEAR_SCREEN;

        // Write interface
        out << memory << std::endl;
        out << process_manager << std::endl;

        std::cout << "$> ";

        std::getline(std::cin, input_command);
        std::cout << CLEAR_SCREEN;

        if (std::regex_match(input_command, CREATE_COMMAND)) {
            std::regex_search(input_command, input_matches, INSTRUCTION_ARG);
            uint16_t instruction_amount = std::stoi(input_matches[2]);

            std::regex_search(input_command, input_matches, MEMORY_ARG);
            uint16_t memory_size = std::stoi(input_matches[2]);

            process_manager.plan_create(instruction_amount, memory_size);
        } else if (std::regex_match(input_command, input_matches, KILL_COMMAND)) {
            uint16_t process_id = std::stoi(input_matches[3]);

            process_manager.plan_kill(process_id);
        } else if (std::regex_match(input_command, EXIT_COMMAND)) {
            break;
        } else if (std::regex_match(input_command, RUN_COMMAND)) {
            try {
                process_manager.run_process();
            } catch (std::out_of_range& e) {
                out << "Process not found" << std::endl;
            } catch (std::runtime_error& e) {
                out << "Not enough memory to create process" << std::endl;
            }
        } else if (std::regex_match(input_command, HELP_COMMAND)) {
            std::cout << "<empty>|r|run: run one clock step" << std::endl;
            std::cout << "create -i <instruction_amount> -m <memory_size>: create process" << std::endl;
            std::cout << "kill <process_id>: kill process" << std::endl;
        } else {
            std::cout << "Invalid command " << input_command << std::endl;
            std::cout << "use \"help\" to get options" << std::endl;
        }
    }

    out.close();
    return 0;
}
