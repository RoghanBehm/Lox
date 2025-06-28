#include "Environment.hpp"
#include "Lox.hpp"

void Environment::define(std::string name, std::any value) {
        values[name] = value;

    }

std::any Environment::get(Token name) {
    if (values.contains(name.lexeme)) {
        return values.at(name.lexeme);
    }

    if (enclosing != nullptr) return enclosing->get(name);

    throw RuntimeError(name, 
        "Undefined variable '" + name.lexeme + "'.");
}

void Environment::assign(Token name, std::any value) {
    if (values.contains(name.lexeme)) {
        values[name.lexeme] = value;

        return;
    }

    if (enclosing != nullptr) {
        enclosing->assign(name, value);
        return;
    }

    throw RuntimeError(name, 
    "Undefined variable '" + name.lexeme + "'.");
}

    