#include "Expr/Ternary.hpp"

#include <cassert>
#include <memory>

Ternary::Ternary(std::unique_ptr<Expr> condition, std::unique_ptr<Expr> thenBranch, std::unique_ptr<Expr> elseBranch) :
    condition(std::move(condition)), thenBranch(std::move(thenBranch)), elseBranch(std::move(elseBranch))
{
    assert(this->condition != nullptr);
    assert(this->thenBranch != nullptr);
    assert(this->elseBranch != nullptr);
}

std::any Ternary::accept(ExprVisitor<std::any>& visitor) const
{
    return visitor.visitTernary(*this);
}

