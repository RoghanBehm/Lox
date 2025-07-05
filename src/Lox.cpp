
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
#include "Resolver.hpp"



Lox::Lox() 
    : hadError(false),
      hadRuntimeError(false),
      interpreter(std::make_unique<Interpreter>(*this)) {}


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

    run(source, false);
    if (hadError) {
        exit(65);
    }
    if (hadRuntimeError) {
        exit(70);
    }

    return true;
}

void Lox::run(std::string source, bool repl) {
    Scanner scanner(source, *this);
    std::vector<Token> tokens = scanner.scanTokens();
    Parser parser(tokens, *this, repl);
    
    try {
        std::vector<std::unique_ptr<Stmt>> statements = parser.parse();
        
        //Stop if there was a parse error
        if (hadError) return;

        Resolver resolver(*interpreter, *this);
        resolver.resolve(statements);

        // Stop if there was a resolution error
        if (hadError) return;
        
        if (statements.empty()) {
            std::cerr << "Parse returned no statements.\n";
            return;
        }

        if (repl && statements.size() == 1) {
            if (auto exprStmt = dynamic_cast<Expression*>(statements[0].get())) {
                std::any result = interpreter->evaluate(exprStmt->getExpr());
                std::cout << interpreter->stringify(result) << "\n";
                return;
            }
        }
        interpreter->interpret(statements);
    } catch (const Parser::ParseError&) {
        return;
    }    
    interpreter->globals->clear();
}

void Lox::runPrompt() {
    std::string line;
    while (true) {
        std::cout << "> ";
        if (!std::getline(std::cin, line)) break;

        try {
            run(line, true);
        } catch (const RuntimeError& error) {
            std::cerr << error.what() << "\n";
            hadRuntimeError = true;
        }

        hadError = false;
        hadRuntimeError = false;
    }
}





void Lox::report(int line, std::string where, std::string message) {
    std::cout << "[line " << std::to_string(line) << "] Error" << where << ": " << message;   
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
    std::cout << error.what() << "\n[line " << error.token.line << "]\n";
    hadRuntimeError = true;
}

