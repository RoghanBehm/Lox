#pragma once

#include <memory>

#include "Expr.hpp"
#include "ExprVisitor.hpp"

#include "Token.hpp"


class Unary : public Expr {
public:
    Unary(Token op, std::unique_ptr<Expr> right);
    std::any accept(ExprVisitor<std::any>& visitor) const override;

    const Token& getOp() const { return op; }
    const Expr& getRightExpr() const { return *right; }

private:
    Token op;
    std::unique_ptr<Expr> right;
};
