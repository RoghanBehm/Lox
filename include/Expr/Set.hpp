#pragma once

#include <any>
#include <memory>
#include "Expr.hpp"
#include "ExprVisitor.hpp"
#include "Token.hpp"


class Set : public Expr {
public:
    Set(const Expr* object, Token name, std::unique_ptr<Expr> value);
    std::any accept(ExprVisitor<std::any>& visitor) const override;

    const Expr& getObject() const { return *object; }
    const Token getName() const { return name; }
    const Expr& getValue() const { return *value; }

private:
    const Expr* object;
    Token name;
    std::unique_ptr<Expr> value;
};

