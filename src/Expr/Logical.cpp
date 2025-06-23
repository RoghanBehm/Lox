#include "Expr/Logical.hpp"

#include <cassert>

Logical::Logical(std::unique_ptr<Expr> left, Token op, std::unique_ptr<Expr> right) :
    left(std::move(left)), op(op), right(std::move(right))
{
    assert(this->left != nullptr);
    assert(this->right != nullptr);
}

std::any Logical::accept(ExprVisitor<std::any>& visitor) const
{
    return visitor.visitLogical(*this);
}

