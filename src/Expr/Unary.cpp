#include "Expr/Unary.hpp"

#include <cassert>

Unary::Unary(Token op, std::unique_ptr<Expr> right) : op(op), right(std::move(right))
{
    assert(this->right != nullptr);
}

std::any Unary::accept(ExprVisitor<std::any>& visitor) const
{
    return visitor.visitUnary(*this);
}
    