#include "Expr/Set.hpp"
#include <memory>

Set::Set(std::unique_ptr<Expr> object, Token name, std::unique_ptr<Expr> value)
    : object(std::move(object)), name(name), value(std::move(value)) {}

std::any Set::accept(ExprVisitor<std::any>& visitor) const
{
    return visitor.visitSet(*this);
}


