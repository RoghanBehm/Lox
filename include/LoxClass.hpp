#pragma once
#include "Interpreter.hpp"
#include "LoxCallable.hpp"
#include "LoxFunction.hpp"
#include <memory>
#include <string>
#include <unordered_map>

class LoxClass : public LoxCallable {
public:
    LoxClass(std::string name, std::unordered_map<std::string, std::shared_ptr<LoxFunction>> methods);
    std::string toString() const override;
    int arity() const override;
    std::any call(Interpreter& interpreter, const std::vector<std::any>& args) const override;
    std::shared_ptr<LoxFunction> findMethod(const std::string& name) const;

private:
    std::string name;
    std::unordered_map<std::string, std::shared_ptr<LoxFunction>> methods;
};
