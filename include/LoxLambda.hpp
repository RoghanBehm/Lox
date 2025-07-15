#pragma once

#include <memory>
#include <vector>
#include "Environment.hpp"
#include "LoxCallable.hpp"
#include "Stmt/Stmt.hpp"
#include "Token.hpp"

class LoxLambda : public LoxCallable {
public:
    LoxLambda(std::vector<Token> params,
              std::vector<std::unique_ptr<Stmt>> body,
              std::shared_ptr<Environment> closure);

    std::any call(Interpreter& interpreter, const std::vector<std::any>& arguments) const override;
    int arity() const override;
    std::string toString() const override;

private:
    std::vector<Token> params;
    std::vector<std::unique_ptr<Stmt>> body;
    std::shared_ptr<Environment> closure;
};
