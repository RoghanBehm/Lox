#pragma once

#include <any>
#include <memory>
#include "Expr.hpp"
#include "ExprVisitor.hpp"
#include "Token.hpp"


class Get : public Expr {
public:
    Get(std::unique_ptr<Expr> object, Token name);
    std::any accept(ExprVisitor<std::any>& visitor) const override;

    const Expr& getObject() const { return *object; }
    const Token getName() const { return name; }
    std::unique_ptr<Expr>& takeObject() { return object; }

private:
    std::unique_ptr<Expr> object;
    Token name;
};

