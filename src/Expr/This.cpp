#include "Expr/This.hpp"

This::This(Token keyword)
    : keyword(keyword) {}

std::any This::accept(ExprVisitor<std::any>& visitor) const
{
    return visitor.visitThis(*this);
}


