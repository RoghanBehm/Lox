#include "Expr/Var.hpp"

Var::Var(Token name) : name(std::move(name))
{}

std::any Var::accept(ExprVisitor<std::any>& visitor) const
{
    return visitor.visitVar(*this);
}

