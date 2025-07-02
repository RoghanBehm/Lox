#include "Expr/Lambda.hpp"


Lambda::Lambda(std::vector<Token> params, std::vector<std::unique_ptr<Stmt>> body)
    : params(std::move(params)), body(std::move(body)) {}

std::any Lambda::accept(ExprVisitor<std::any>& visitor) const {
    return visitor.visitLambda(*this);
}

