#pragma once

#include <any>
#include <memory>
#include <string>
#include <unordered_map>
#include "Token.hpp"

class Environment {
private:
    std::unordered_map<std::string, std::any> values;
    std::shared_ptr<Environment> enclosing;

public:
    void define(std::string name, std::any value);
    std::any get(Token name);
    void assign(Token name, std::any value);
    Environment() = default;

    Environment(std::shared_ptr<Environment> enclosing)
        : enclosing(std::move(enclosing)) {}

    std::shared_ptr<Environment> getEnclosing() const {
        return enclosing;
    }
    
    ~Environment() {
        values.clear();
    }

    void clear();

    std::any getAt(int distance, std::string name);
    void assignAt(int distance, Token name, std::any value);
    Environment* ancestor(int distance);
};