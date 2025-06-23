#include "Expr/Binary.hpp"
#include <cassert>


Binary::Binary(std::unique_ptr<Expr> left, Token op, std::unique_ptr<Expr> right) :
    left(std::move(left)),
    op(op),
    right(std::move(right))
{
    assert(this->left != nullptr);
    assert(this->right != nullptr);
}

std::any Binary::accept(ExprVisitor<std::any>& visitor) const
{
    return visitor.visitBinary(*this);
}
