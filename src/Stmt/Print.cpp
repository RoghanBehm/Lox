#include <cassert>
#include "Stmt/Print.hpp"

Print::Print(std::unique_ptr<Expr> expr) : expr(std::move(expr))
{
    assert(this->expr != nullptr);
}

void Print::accept(StmtVisitor<void>& visitor) const
{
    return visitor.visitPrint(*this);
}
