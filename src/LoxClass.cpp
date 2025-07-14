#include "LoxClass.hpp"
#include "LoxInstance.hpp"
#include <cstddef>

LoxClass::LoxClass(std::string name, std::unordered_map<std::string, std::shared_ptr<LoxFunction>> methods) : name(name), methods(std::move(methods)) {}

std::string LoxClass::toString() const {
    return name;
}

int LoxClass::arity() const {
    return 0;
}

std::any LoxClass::call(Interpreter&, const std::vector<std::any>&) {
    return std::make_shared<LoxInstance>(this);
}

std::shared_ptr<LoxFunction> LoxClass::findMethod(std::string name) {
    if (methods.contains(name)) {
        return methods.at(name);
    }

    return nullptr;
}