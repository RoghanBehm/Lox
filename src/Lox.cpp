
#include <fstream>
#include <iostream>
#include <memory>
#include <vector>
#include "Lox.hpp"
#include "Interpreter.hpp"
#include "Token.hpp"
#include "Scanner.hpp"
#include "to_string.hpp"
#include "Parser.hpp"
#include "AstPrinter.hpp"
#include "Expr/Expr.hpp"

Interpreter* Lox::interpreter = nullptr;

Lox::Lox() {
    interpreter = new Interpreter(*this);
}
bool Lox::hadRuntimeError = false;

bool Lox::runFile(const std::string& path) {
    std::ifstream file(path);
    if (!file) {
        std::cerr << "Could not open file\n";
        return false;
    }

    std::string line;
    std::string source;

    while (std::getline(file, line)) {
        source += line + "\n";
    }

    run(source);
    if (Lox::hadError) {
        exit(65);
    }
    if (Lox::hadRuntimeError) {
        exit(70);
    }

    return true;
}

void Lox::run(std::string source) {
    Scanner scanner(source, *this);
    std::vector<Token> tokens = scanner.scanTokens();
    Parser parser(tokens, *this);
    std::unique_ptr<Expr> expression = parser.parse();

    if (hadError) return;
    if (!expression) {
    std::cerr << "Parse returned null expression!\n";
    return;
}
    interpreter->interpret(*expression);
}

void Lox::runPrompt() {
    std::string line;
    while (true) {
        std::cout << "> ";
        if (!std::getline(std::cin, line)) break;

        run(line);
        hadError = false;
        hadRuntimeError = false;
    }
}



void Lox::report(int line, std::string where, std::string message) {
    std::cout << "[line ] " << std::to_string(line) << "] Error" << where << ": " << message;   
    hadError = true;     
}

void Lox::error(int line, std::string message) {
    report(line, "", message);
}


void Lox::error(Token token, std::string message) {
    if (token.type == TokenType::EOF_) {
        report(token.line, " at end", message + "\n");
    } else {
        report(token.line, " at '" + token.lexeme + "'", message + "\n");
    }
}

void Lox::runtimeError(RuntimeError error) {
    std::cout << error.what() << "\n[line " << error.token.line << "]";
    hadRuntimeError = true;
}

