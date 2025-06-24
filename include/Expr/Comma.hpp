#pragma once

#include <memory>

#include "Expr.hpp"
#include "ExprVisitor.hpp"

class Comma : public Expr {
public:
    Comma(std::unique_ptr<Expr> left, std::unique_ptr<Expr> right);
    std::any accept(ExprVisitor<std::any>& visitor) const override;
    const Expr& getLeftExpr() const { return *left; }
    const Expr& getRightExpr() const { return *right; }

private:
    std::unique_ptr<Expr> left;
    std::unique_ptr<Expr> right;
};