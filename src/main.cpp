#include "Lox.hpp"
#include <iostream>
#include "AstPrinter.hpp"

int main(int argc, char* argv[]) {
    Lox lox;
    printer();
    if (argc > 2) {
        std::cout << "Usage: jlox [script]\n";
        return 64;
    } else if (argc == 2) {
        lox.runFile(argv[1]);
    } else {
        lox.runPrompt();
    }
}