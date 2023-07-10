#include <iomanip>
#include <iostream>

#include "interface.hpp"

static constexpr char CLEAR_SCREEN[] = "\e[2J";

// Input Defines
static const std::regex INSTRUCTION_REGEX(R"(-i(\s*)([0-9]+))");
static const std::regex MEMORY_REGEX(R"(-m(\s*)([0-9]+))");
static const std::regex CREATE_REGEX(
    R"(c(reate)?(\s+)((-i(\s*)([0-9]+)(\s+)-m(\s*)([0-9]+))|(-m(\s*)([0-9]+)(\s+)-i(\s*)([0-9]+))))");

static const std::regex RUN_REGEX(R"((r(un)?)?)");
static const std::regex KILL_REGEX(R"(k(ill)?(\s+)([0-9]+))");
static const std::regex HELP_REGEX(R"(h(elp)?)");
static const std::regex EXIT_REGEX(R"(e(xit)?)");

// Output Defines
static constexpr uint16_t MEMORY_ROW_SIZE = 5;

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
    this->out << process_manager;
    this->out.flush();
}

void Interface::Output::print_invalid_pid() {
    this->out << "Process not found" << std::endl;
}

void Interface::Output::print_insufficient_memory() {
    this->out << "Not enough memory to create process" << std::endl;
}

std::string Interface::Output::place_vertical(uint16_t x_pos, std::string text) {
    text = std::regex_replace(text, std::regex("\n"), "\e[" + std::to_string(x_pos) + "G\e[1B");

    return "\e[" + std::to_string(x_pos) + "G" + text + "\n";
}

std::ostream& operator <<(std::ostream& output, const Memory& memory) {
    output << "\e[1m Mapa de Bits\e[22m" << std::endl;
    output << "\e[7m+-------------------+" << std::endl;

    for (uint16_t i = 0; i < memory.bitmap.size(); i++) {
        output << "| " << memory.bitmap[i] << " ";

        if (i % MEMORY_ROW_SIZE == MEMORY_ROW_SIZE - 1) {
            output << "|" << std::endl << "+-------------------+" << std::endl;
        }
    }

    output << "\e[27m";

    return output;
}

std::ostream& operator <<(std::ostream& output, const Process& process) {
    output << "\e[1m TCB\e[22m" << std::endl;

    output << "+-------------------+" << std::endl;
    output << "| PID: " << std::left << std::setw(13) << process.id << "|" << std::endl;
    output << "+-------------------+" << std::endl;
    output << "| PC: " << std::left << std::setw(14) << process.program_counter << "|" << std::endl;
    output << "+-------------------+" << std::endl;
    output << "| NUM_INST: " << std::left << std::setw(8) << process.instruction_amount << "|" << std::endl;
    output << "+-------------------+" << std::endl;
    output << "| MEM_ADDR: " << std::left << std::setw(8) << process.start_address << "|" << std::endl;
    output << "+-------------------+" << std::endl;
    output << "| MEM_SZ: " << std::left << std::setw(10) << process.memory_size << "|" << std::endl;
    output << "+-------------------+" << std::endl;

    return output;
}

std::ostream& operator <<(std::ostream& output, const ProcessManager& process_manager) {
    std::stringstream temp;
    temp << process_manager.memory;
    output << Interface::Output::place_vertical(50, temp.str()) << "\n\n";

    temp.str("");

    if (process_manager.tasks_queue.empty() or process_manager.tasks_queue.front().command != RUN) {
        temp << "\e[1m TCB\e[22m" << std::endl;
        temp << "+-------------------+" << std::endl;
        temp << "| PID:              |" << std::endl;
        temp << "+-------------------+" << std::endl;
        temp << "| PC:               |" << std::endl;
        temp << "+-------------------+" << std::endl;
        temp << "| NUM_INST:         |" << std::endl;
        temp << "+-------------------+" << std::endl;
        temp << "| MEM_ADDR:         |" << std::endl;
        temp << "+-------------------+" << std::endl;
        temp << "| MEM_SZ:           |" << std::endl;
        temp << "+-------------------+" << std::endl;
    } else {
        temp << process_manager.processes.at(process_manager.tasks_queue.front().process_id);
    }

    output << "\e[H" << Interface::Output::place_vertical(20, temp.str());
    output << "\e[1m Fila de Prontos\e[22m" << std::endl;

    if (process_manager.tasks_queue.empty()) {
        output << "+----------+" << std::endl;
        output << "|    --    |" << std::endl;
        output << "+----------+" << std::endl;
        return output;
    }

    output << "+";

    for (uint16_t i = 0; i < process_manager.tasks_queue.size(); i++) {
        output << "----------+";
    }

    output << "\n|";

    for (auto task: process_manager.tasks_queue) {
        switch (task.command) {
            case Command::CREATE: {
                output << "  create  |";
                break;
            }

            case Command::RUN: {
                output << " PID " << std::left << std::setw(4) << task.process_id << " |";
                break;
            }

            case Command::KILL: {
                output << " kill " << std::left << std::setw(3) << task.process_id << " |";
                break;
            }

            default:
                break;
        }
    }

    output << "\n+";

    for (uint16_t i = 0; i < process_manager.tasks_queue.size(); i++) {
        output << "----------+";
    }

    return output;
}
