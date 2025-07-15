#include "LoxInstance.hpp"
#include "RuntimeError.hpp"
#include <memory>

LoxInstance::LoxInstance(const LoxClass* klass)
    : klass(klass) {}

std::string LoxInstance::toString() const {
    return klass->toString() + " instance";
}

std::any LoxInstance::get(Token name) {
    if (fields.contains(name.lexeme)) {
        return fields.at(name.lexeme);
    }
    if (auto method = klass->findMethod(name.lexeme)) {
        return method->bind(shared_from_this());
    }
    throw RuntimeError(name, "Undefined property '" + name.lexeme + "'.");
}

void LoxInstance::set(Token name, std::any value) {
    fields[name.lexeme] = value;
}
