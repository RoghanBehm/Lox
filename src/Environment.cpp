#include "Environment.hpp"
#include "Lox.hpp"

void Environment::define(std::string name, std::any value) {
        values[name] = value;
    }

void Environment::clear() {
    values.clear();
}

std::any Environment::getAt(int distance, std::string name) {
    return ancestor(distance)->values.at(name);
}

void Environment::assignAt(int distance, Token name, std::any value) {
    ancestor(distance)->values[name.lexeme] = value;
}

Environment* Environment::ancestor(int distance) {
    Environment* environment = this;
    for (int i = 0; i < distance; i++) {
        if (!environment->enclosing) throw std::runtime_error("No enclosing environment.");
        environment = environment->enclosing.get();
    }
    return environment;
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

    