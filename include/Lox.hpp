#pragma once
#include "Token.hpp"
#include <string>

class Lox {
    public:
        bool hadError = false;
        bool runFile(const std::string& path);
        void runPrompt(Lox& lox);
        void run(std::string source, Lox& lox);
        void error(int line, std::string message);
        void error(Token token, std::string message);

    private:
        void report(int line, std::string where, std::string message);
};