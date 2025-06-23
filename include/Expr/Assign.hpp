#pragma once

#include <any>
#include <memory>
#include "Expr.hpp"
#include "ExprVisitor.hpp"
#include "Token.hpp"


class Assign : public Expr {
public:
    Assign(Token name, std::unique_ptr<Expr> value);
    std::any accept(ExprVisitor<std::any>& visitor) const override;

    const Token& getName() const { return name; }
    const Expr& getValue() const { return *value; }

private:
    Token name;
    std::unique_ptr<Expr> value;
};