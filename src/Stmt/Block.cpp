#include "Stmt/Block.hpp"
#include <cassert>


Block::Block(std::vector<std::unique_ptr<Stmt>> statements) :
    statements(std::move(statements))
{}

void Block::accept(StmtVisitor<void>& visitor) const
{
    return visitor.visitBlock(*this);
}
