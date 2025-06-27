#include <cassert>
#include "Stmt/Expression.hpp"


Expression::Expression(std::unique_ptr<Expr> expr) : expr(std::move(expr))
{
    assert(this->expr != nullptr);
}

void Expression::accept(StmtVisitor<void>& visitor) const
{
    return visitor.visitExpression(*this);
}

