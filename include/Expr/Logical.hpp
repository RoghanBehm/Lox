#pragma once

#include <memory>
#include "Expr.hpp"
#include "ExprVisitor.hpp"
#include "Token.hpp"

class Logical : public Expr {
public:
    Logical(std::unique_ptr<Expr> left, Token op, std::unique_ptr<Expr> right);
    std::any accept(ExprVisitor<std::any>& visitor) const override;

    const Expr& getLeftExpr() const { return *left; }
    const Token& getOp() const { return op; }
    const Expr& getRightExpr() const { return *right; }

private:
    std::unique_ptr<Expr> left;
    Token op;
    std::unique_ptr<Expr> right;
};
