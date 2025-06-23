#include "Expr/Call.hpp"
#include <cassert>
#include "Expr/ExprVisitor.hpp" 

Call::Call(std::unique_ptr<Expr> callee, Token paren,
                   std::vector<std::unique_ptr<Expr>> arguments) :
    callee(std::move(callee)), paren(paren), arguments(std::move(arguments))
{

}

std::any Call::accept(ExprVisitor<std::any>& visitor) const
{
    return visitor.visitCall(*this);
}
