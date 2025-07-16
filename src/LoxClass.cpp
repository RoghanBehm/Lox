#include "LoxClass.hpp"
#include "LoxInstance.hpp"
#include <memory>

LoxClass::LoxClass(std::string name, std::shared_ptr<LoxClass> superclass, std::unordered_map<std::string, std::shared_ptr<LoxFunction>> methods)
    : name(std::move(name)), superclass(std::move(superclass)), methods(std::move(methods)) {}

std::string LoxClass::toString() const {
    return name;
}

int LoxClass::arity() const {
    if (auto initializer = findMethod("init")) {
        return initializer->arity();
    }
    return 0;
}

std::any LoxClass::call(Interpreter& interpreter, const std::vector<std::any>& args) const {
    auto instance = std::make_shared<LoxInstance>(this);
    if (auto initializer = findMethod("init")) {
        initializer->bind(instance)
                   ->call(interpreter, args);
    }
    return instance;
}

std::shared_ptr<LoxFunction> LoxClass::findMethod(const std::string& methodName) const {
    auto it = methods.find(methodName);
    if (it != methods.end()) {
        return it->second;
    }

    if (superclass != nullptr) {
        return superclass->findMethod(methodName);
    }

    return nullptr;
}

