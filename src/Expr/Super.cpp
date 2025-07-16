#include "Expr/Super.hpp"

Super::Super(Token keyword, Token method)
    : keyword(keyword), method(method) {}

std::any Super::accept(ExprVisitor<std::any>& visitor) const
{
    return visitor.visitSuper(*this);
}


