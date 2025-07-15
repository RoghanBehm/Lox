#pragma once

#include "Expr.hpp"
#include "ExprVisitor.hpp"
#include "Token.hpp"

class This : public Expr {
public:
    This(Token keyword);
    std::any accept(ExprVisitor<std::any>& visitor) const override;
    const Token& getKeyword() const { return keyword; }


private:
    Token keyword;
};

