#pragma once

#include "Expr.hpp"
#include "ExprVisitor.hpp"
#include "Token.hpp"

class Super : public Expr {
public:
    Super(Token keyword, Token method);
    std::any accept(ExprVisitor<std::any>& visitor) const override;
    const Token& getKeyword() const { return keyword; }
    const Token& getMethod() const { return method; }


private:
    Token keyword;
    Token method;
};

