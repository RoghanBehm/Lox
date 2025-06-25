#pragma once

#include <string>
#include "Token.hpp"
#include "RuntimeError.hpp"
#include "Interpreter.hpp"

class Lox {
    public:
        Lox();
        bool hadError = false;
        static bool hadRuntimeError;
        bool runFile(const std::string& path);
        void runPrompt();
        void run(std::string source);
        void error(int line, std::string message);
        void error(Token token, std::string message);
        static void runtimeError(RuntimeError error);
    private:
        void report(int line, std::string where, std::string message);
        static Interpreter* interpreter;
};