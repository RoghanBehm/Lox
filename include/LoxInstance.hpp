#pragma once
#include "LoxClass.hpp"

class LoxInstance {
private:
    LoxClass* klass;
    std::unordered_map<std::string, std::any> fields;
public:
    LoxInstance(LoxClass* klass);
    std::any get(Token name);
    std::string toString() const;
    void set(Token name, std::any value);
};