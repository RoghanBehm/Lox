#pragma once
#include "Token.hpp"
#include "LoxClass.hpp"
#include <any>
#include <memory>
#include <string>
#include <unordered_map>

class LoxInstance : public std::enable_shared_from_this<LoxInstance> {
public:
    explicit LoxInstance(const LoxClass* klass);

    std::string toString() const;
    std::any get(Token name);
    void set(Token name, std::any value);

private:
    const LoxClass* klass;
    std::unordered_map<std::string, std::any> fields;
};