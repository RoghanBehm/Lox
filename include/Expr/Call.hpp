#pragma once

#include <memory>
#include <vector>
#include "Expr.hpp"
#include "ExprVisitor.hpp"
#include "Token.hpp"


class Call : public Expr {
public:
    Call(std::unique_ptr<Expr> callee, Token paren,
             std::vector<std::unique_ptr<Expr>> arguments);

    std::any accept(ExprVisitor<std::any>& visitor) const override;

    const Expr& getCallee() const { return *callee; }
    const Token& getParen() const { return paren; }
    const std::vector<std::unique_ptr<Expr>>& getArgruments() const { return arguments; }

private:
    std::unique_ptr<Expr> callee;
    Token paren;
    std::vector<std::unique_ptr<Expr>> arguments;
};
