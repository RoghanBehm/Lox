#include "Expr/Assign.hpp"
#include <cassert>

Assign::Assign(Token name, std::unique_ptr<Expr> value) :
    name(std::move(name)), value(std::move(value))
{
    assert(this->value != nullptr);
}

std::any Assign::accept(ExprVisitor<std::any>& visitor) const
{
    return visitor.visitAssign(*this);
}