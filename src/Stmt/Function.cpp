#include <cassert>
#include "Stmt/Function.hpp"

Function::Function(Token name, std::vector<Token> params,
                           std::vector<std::unique_ptr<Stmt>> body) :
    name(name), params(std::move(params)), body(std::move(body))
{
    assert(name.type == TokenType::IDENTIFIER);
}

void Function::accept(StmtVisitor<void>& visitor) const
{
    return visitor.visitFunction(*this);
}
