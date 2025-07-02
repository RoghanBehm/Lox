#include "LoxFunction.hpp"
#include "Interpreter.hpp"
#include <memory>

LoxFunction::LoxFunction(const Function& declaration, std::shared_ptr<Environment> closure) 
    : declaration(declaration), closure(std::move(closure)) {}

std::any LoxFunction::call(Interpreter& interpreter, const std::vector<std::any>& arguments) {
    auto env = std::make_shared<Environment>(closure);
    for (int i = 0; i < static_cast<int>(declaration.getParams().size()); ++i) {
        env->define(declaration.getParams().at(i).lexeme, arguments.at(i));
    }

    interpreter.executeBlock(declaration.getBody(), env);
    return nullptr;
}

int LoxFunction::arity() const {
    return declaration.getParams().size();
}

std::string LoxFunction::toString() const {
    return "<fn " + declaration.getName().lexeme + ">";
}