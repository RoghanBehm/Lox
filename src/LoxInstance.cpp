#include "LoxInstance.hpp"
#include "RuntimeError.hpp"
#include <memory>

LoxInstance::LoxInstance(LoxClass* klass) 
    : klass(klass) {}

std::string LoxInstance::toString() const {
    return klass->toString() + " instance"; 
}

std::any LoxInstance::get(Token name) {
    if (fields.contains(name.lexeme)) {
        return fields.at(name.lexeme);
    }

    std::shared_ptr<LoxFunction> method = klass->findMethod(name.lexeme);
    if (method) return std::static_pointer_cast<LoxCallable>(method);

    throw RuntimeError(name, "Undefined property '" + name.lexeme + "'.");
}

void LoxInstance::set(Token name, std::any value) {
    fields[name.lexeme] = value;
}