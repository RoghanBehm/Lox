#pragma once

#include <string>
#include "Token.hpp"
#include "RuntimeError.hpp"
#include "Interpreter.hpp"

class Lox {
public:
    Lox();
    bool runFile(const std::string& path);
    void runPrompt();
    void run(std::string source, bool repl);
    void error(int line, std::string message);
    void error(Token token, std::string message);
    void runtimeError(RuntimeError error);

private:
    void report(int line, std::string where, std::string message);

    bool hadError;
    bool hadRuntimeError;
    std::unique_ptr<Interpreter> interpreter;
};
