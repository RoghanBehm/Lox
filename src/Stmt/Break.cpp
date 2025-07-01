#include <cassert>
#include "Stmt/Break.hpp"

Break::Break(Token name) :
    name(std::move(name)){}

void Break::accept(StmtVisitor<void>& visitor) const
{
    return visitor.visitBreak(*this);
}

