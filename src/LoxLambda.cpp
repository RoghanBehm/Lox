#include "LoxLambda.hpp"
#include "Interpreter.hpp"
#include "ReturnException.hpp"
#include <memory>

LoxLambda::LoxLambda(std::vector<Token> params,
                     std::vector<std::unique_ptr<Stmt>> body,
                     std::shared_ptr<Environment> closure)
    : params(std::move(params)), body(std::move(body)), closure(std::move(closure)) {}

std::any LoxLambda::call(Interpreter& interpreter, const std::vector<std::any>& arguments) const {
    auto env = std::make_shared<Environment>(closure);

    for (int i = 0; i < static_cast<int>(params.size()); ++i) {
        env->define(params[i].lexeme, arguments[i]);
    }

    try {
        interpreter.executeBlock(body, env);
    } catch (const ReturnException& returnValue) {
        return returnValue.value;
    }

    return nullptr;
}

int LoxLambda::arity() const {
    return static_cast<int>(params.size());
}

std::string LoxLambda::toString() const {
    return "<fn>";
}
