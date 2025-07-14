#pragma once
#include "Interpreter.hpp"
#include "LoxCallable.hpp"
#include "LoxFunction.hpp"
#include <string>
#include <unordered_map>

class LoxClass : public LoxCallable {
private:
    std::string name;
    std::unordered_map<std::string, std::shared_ptr<LoxFunction>> methods;
public:
    LoxClass(std::string name, std::unordered_map<std::string, std::shared_ptr<LoxFunction>> methods);
    std::string toString() const override;
    std::any call(Interpreter&, const std::vector<std::any>&) override;
    int arity() const override;
    std::string getName() { return name; }
    std::shared_ptr<LoxFunction> findMethod(std::string name);
};

