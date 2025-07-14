#include "Stmt/Class.hpp"
#include <memory>



Class::Class(Token name, /*std::unique_ptr<Var> superclass,*/ std::vector<std::unique_ptr<Function>> methods) 
    : name(name), /*superclass(std::move(superclass)),*/ methods(std::move(methods)) {}

void Class::accept(StmtVisitor<void>& visitor) const
{
    return visitor.visitClass(*this);
}
