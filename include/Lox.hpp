#pragma once
#include <string>

class Lox {
    public:
        bool hadError = false;
        bool runFile(const std::string& path);
        void runPrompt();
        void run(std::string source);
        void error(int line, std::string message);

    private:
        void report(int line, std::string where, std::string message);
};