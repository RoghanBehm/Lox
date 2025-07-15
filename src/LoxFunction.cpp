#include "LoxFunction.hpp"
#include "Interpreter.hpp"
#include "ReturnException.hpp"
#include <memory>

LoxFunction::LoxFunction(const Function& declaration, std::shared_ptr<Environment> closure, bool isInitialiser) 
    : declaration(declaration), closure(std::move(closure)), isInitialiser(isInitialiser) {}


std::any LoxFunction::call(Interpreter& interpreter, const std::vector<std::any>& arguments) const {
    auto env = std::make_shared<Environment>(closure);
    for (int i = 0; i < static_cast<int>(declaration.getParams().size()); ++i) {
        env->define(declaration.getParams().at(i).lexeme, arguments.at(i));
    }

    try {
        interpreter.executeBlock(declaration.getBody(), env);
    } catch (const ReturnException& returnValue) {
        auto closurePtr = closure;
        if (isInitialiser) return closurePtr->getAt(0, "this");

        return returnValue.value;
    }

    auto closurePtr = closure;

    if (isInitialiser) return closurePtr->getAt(0, "this");
    return nullptr;
}

int LoxFunction::arity() const {
    return declaration.getParams().size();
}

std::string LoxFunction::toString() const {
    return "<fn " + declaration.getName().lexeme + ">";
}

std::shared_ptr<LoxFunction> LoxFunction::bind(std::shared_ptr<LoxInstance> instance) {
  auto env = std::make_shared<Environment>(closure);
  env->define("this", instance);
  return std::make_shared<LoxFunction>(declaration, env, isInitialiser);
}