#include "Expr/Grouping.hpp"
#include <cassert>


Grouping::Grouping(std::unique_ptr<Expr> expr) : expr(std::move(expr))
{
    assert(this->expr != nullptr);
}

std::any Grouping::accept(ExprVisitor<std::any>& visitor) const
{
    return visitor.visitGrouping(*this);
}
