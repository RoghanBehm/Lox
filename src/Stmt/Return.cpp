#include <cassert>
#include "Stmt/Return.hpp"

Return::Return(Token keyword, std::unique_ptr<Expr> value) :
    keyword(std::move(keyword)), value(std::move(value))
{
    assert(this->keyword.type == TokenType::RETURN);
}

void Return::accept(StmtVisitor<void>& visitor) const
{
    return visitor.visitReturn(*this);
}
