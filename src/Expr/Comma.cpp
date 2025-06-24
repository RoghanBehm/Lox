#include "Expr/Comma.hpp"

#include <cassert>

Comma::Comma(std::unique_ptr<Expr> left, std::unique_ptr<Expr> right) :
    left(std::move(left)), right(std::move(right))
{
    assert(this->left != nullptr);
    assert(this->right != nullptr);
}

std::any Comma::accept(ExprVisitor<std::any>& visitor) const
{
    return visitor.visitComma(*this);
}

