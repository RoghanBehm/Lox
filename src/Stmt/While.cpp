#include <cassert>
#include "Stmt/While.hpp"

While::While(std::unique_ptr<Expr> condition, std::unique_ptr<Stmt> body) :
    condition(std::move(condition)), body(std::move(body))
{
    assert(this->condition != nullptr);
    assert(this->body != nullptr);
}

void While::accept(StmtVisitor<void>& visitor) const
{
    return visitor.visitWhile(*this);
}

