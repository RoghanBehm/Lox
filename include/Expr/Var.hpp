#pragma once

#include <any>
#include "Expr.hpp"
#include "ExprVisitor.hpp"
#include "Token.hpp"

class Var : public Expr {
public:
    Var(Token name);
    std::any accept(ExprVisitor<std::any>& visitor) const override;

    const Token& getName() const { return name; }
private:
    Token name;
};
