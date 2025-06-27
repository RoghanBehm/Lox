#include <cassert>
#include "Stmt/If.hpp"


If::If(std::unique_ptr<Expr> condition, std::unique_ptr<Stmt> thenBranch,
               std::unique_ptr<Stmt> elseBranch) :
    condition(std::move(condition)),
    thenBranch(std::move(thenBranch)),
    elseBranch(std::move(elseBranch))
{
    assert(this->condition != nullptr);
    assert(this->thenBranch != nullptr);
 
}

void If::accept(StmtVisitor<void>& visitor) const
{
    return visitor.visitIf(*this);
}
