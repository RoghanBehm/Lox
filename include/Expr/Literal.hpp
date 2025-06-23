#pragma once

#include <any>
#include "Expr.hpp"
#include "ExprVisitor.hpp"


class Literal : public Expr {
public:
    Literal(std::any literal);
    std::any accept(ExprVisitor<std::any>& visitor) const override;

    const std::any& getLiteral() const { return literal; }

private:
    std::any literal;
};

