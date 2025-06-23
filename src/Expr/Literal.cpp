#include "Expr/Literal.hpp"


Literal::Literal(std::any literal) : literal(std::move(literal))
{}

std::any Literal::accept(ExprVisitor<std::any>& visitor) const
{
    return visitor.visitLiteral(*this);
}
