
#include <fstream>
#include <iostream>
#include <vector>
#include "Lox.hpp"
#include "Token.hpp"
#include "Scanner.hpp"
#include "to_string.hpp"

bool hadError = false;

bool Lox::runFile(const std::string& path) {
    std::ifstream file(path);
    if (!file) {
        std::cerr << "Could not open file\n";
        return false;
    }
    return true;
}

void Lox::run(std::string source) {
    Scanner scanner(source, *this);
    std::vector<Token> tokens = scanner.scanTokens();

    for (auto &t : tokens) {
        std::cout << t.toString() << "\n";
    }
}

void Lox::runPrompt() {
    std::string line;
    while (true) {
        std::cout << "> ";
        if (!std::getline(std::cin, line)) break;

        run(line);
    }
}



void Lox::report(int line, std::string where, std::string message) {
    std::cout << "[line ] " << std::to_string(line) << "] Error" << where << ": " << message;   
    hadError = true;     
}

void Lox::error(int line, std::string message) {
    report(line, "", message);
}



