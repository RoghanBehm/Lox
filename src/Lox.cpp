
#include <fstream>
#include <iostream>
#include <memory>
#include <vector>
#include "Lox.hpp"
#include "Token.hpp"
#include "Scanner.hpp"
#include "to_string.hpp"
#include "Parser.hpp"
#include "AstPrinter.hpp"
#include "Expr/Expr.hpp"
bool hadError = false;

bool Lox::runFile(const std::string& path) {
    std::ifstream file(path);
    if (!file) {
        std::cerr << "Could not open file\n";
        return false;
    }
    return true;
}

void Lox::run(std::string source, Lox& lox) {
    Scanner scanner(source, *this);
    std::vector<Token> tokens = scanner.scanTokens();
    Parser parser(tokens, lox);
    std::unique_ptr<Expr> expression = parser.parse();

    if (hadError) return;
    AstPrinter p;
    std::cout << p.print(*expression);
}
void Lox::runPrompt(Lox& lox) {
    std::string line;
    while (true) {
        std::cout << "> ";
        if (!std::getline(std::cin, line)) break;

        run(line, lox);
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
        report(token.line, " at end", message);
    } else {
        report(token.line, " at '" + token.lexeme + "'", message);
    }
}


