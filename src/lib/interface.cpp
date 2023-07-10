#include <iostream>

#include "interface.hpp"

static constexpr char CLEAR_SCREEN[] = "\033c";

// Input Defines
static const std::regex INSTRUCTION_REGEX(R"(-i(\s*)([0-9]+))");
static const std::regex MEMORY_REGEX(R"(-m(\s*)([0-9]+))");
static const std::regex CREATE_REGEX(
    R"(c(reate)?(\s+)((-i(\s*)([0-9]+)(\s+)-m(\s*)([0-9]+))|(-m(\s*)([0-9]+)(\s+)-i(\s*)([0-9]+))))");

static const std::regex RUN_REGEX(R"((r(un)?)?)");
static const std::regex KILL_REGEX(R"(k(ill)?(\s+)([0-9]+))");
static const std::regex HELP_REGEX(R"(h(elp)?)");
static const std::regex EXIT_REGEX(R"(e(xit)?)");

Interface::Input::Input() {
    std::cout << CLEAR_SCREEN;
}

void Interface::Input::update_input() {
    std::cout << "$> ";
    std::getline(std::cin, this->input_command);
    std::cout << CLEAR_SCREEN;
}

Command Interface::Input::get_command() {
    if (std::regex_match(this->input_command, CREATE_REGEX)) {
        return Command::CREATE;
    } else if (std::regex_match(this->input_command, this->input_matches, KILL_REGEX)) {
        return Command::KILL;
    } else if (std::regex_match(this->input_command, EXIT_REGEX)) {
        return Command::EXIT;
    } else if (std::regex_match(this->input_command, RUN_REGEX)) {
        return Command::RUN;
    } else if (std::regex_match(this->input_command, HELP_REGEX)) {
        return Command::HELP;
    }

    return Command::INVALID;
}

uint16_t Interface::Input::get_instruction_amount() {
    std::regex_search(this->input_command, this->input_matches, INSTRUCTION_REGEX);
    return std::stoi(this->input_matches[2]);
}

uint16_t Interface::Input::get_memory_size() {
    std::regex_search(this->input_command, this->input_matches, MEMORY_REGEX);
    return std::stoi(this->input_matches[2]);
}

uint16_t Interface::Input::get_process_id() const {
    return std::stoi(this->input_matches[3]);
}

void Interface::Input::print_help() {
    std::cout << "<empty>|r|run: run one clock step" << std::endl;
    std::cout << "create -i <instruction_amount> -m <memory_size>: create process" << std::endl;
    std::cout << "kill <process_id>: kill process" << std::endl;
}

void Interface::Input::print_invalid() {
    std::cout << "Invalid command " << this->input_command << std::endl;
    std::cout << "use \"help\" to get options" << std::endl;
}

Interface::Output::Output() : out("/dev/pts/1") {
}

Interface::Output::~Output() {
    this->out.close();
}

void Interface::Output::update_output(const ProcessManager& process_manager) {
    this->out << CLEAR_SCREEN;
    this->out << process_manager << std::endl;
}

void Interface::Output::print_invalid_pid() {
    this->out << "Process not found" << std::endl;
}

void Interface::Output::print_insufficient_memory() {
    this->out << "Not enough memory to create process" << std::endl;
}

std::ostream& operator <<(std::ostream& output, const Memory& memory) {
    for (auto bit: memory.bitmap) {
        output << bit;
    }

    return output;
}

std::ostream& operator <<(std::ostream& output, const Process& process) {
    output << "| PID: " << process.id <<
    " | PC: " << process.program_counter <<
    " | NUM_INST: " << process.instruction_amount <<
    " | MEM_ADDR: " << process.start_address <<
    " | MEM_SZ: " << process.memory_size << " |";

    return output;
}

std::ostream& operator <<(std::ostream& output, const ProcessManager& process_manager) {
    output << process_manager.memory << std::endl;

    if (process_manager.tasks_queue.empty()) {
        return output;
    }

    output << "|";

    for (auto task: process_manager.tasks_queue) {
        switch (task.command) {
            case Command::CREATE: {
                output << "create" << "|";
                break;
            }

            case Command::RUN: {
                output << "PID " << task.process_id << "|";
                break;
            }

            case Command::KILL: {
                output << "kill " << task.process_id << "|";
                break;
            }

            default:
                break;
        }
    }

    auto first_task = process_manager.tasks_queue.front();

    if (first_task.command == Command::RUN) {
        output << std::endl << process_manager.processes.at(first_task.process_id);
    }

    return output;
}
