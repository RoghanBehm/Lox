#include <cassert>
#include "Stmt/VarStmt.hpp"

VarStmt::VarStmt(Token name, std::unique_ptr<Expr> expr) :
    name(std::move(name)), expr(std::move(expr))
{
    assert(this->name.type == TokenType::IDENTIFIER);
}

void VarStmt::accept(StmtVisitor<void>& visitor) const
{
    return visitor.visitVarStmt(*this);
}
