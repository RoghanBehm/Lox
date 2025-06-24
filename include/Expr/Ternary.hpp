#pragma once

#include <memory>

#include "Expr.hpp"
#include "ExprVisitor.hpp"

class Ternary : public Expr {
public:
    Ternary(std::unique_ptr<Expr> condition, std::unique_ptr<Expr> thenBranch, std::unique_ptr<Expr> elseBranch);
    std::any accept(ExprVisitor<std::any>& visitor) const override;
    const Expr& getConditionExpr() const { return *condition; }
    const Expr& getThenBranch() const { return *thenBranch; }
    const Expr& getElseBranch() const { return *elseBranch; }

private:
    std::unique_ptr<Expr> condition;
    std::unique_ptr<Expr> thenBranch;
    std::unique_ptr<Expr> elseBranch;
};