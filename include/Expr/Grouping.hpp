#pragma once

#include <memory>

#include "Expr.hpp"
#include "ExprVisitor.hpp"

class Grouping : public Expr {
public:
    Grouping(std::unique_ptr<Expr> expr);
    std::any accept(ExprVisitor<std::any>& visitor) const override;

    const Expr& getExpr() const { return *expr; }

private:
    std::unique_ptr<Expr> expr;
};