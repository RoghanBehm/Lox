#pragma once

#include "Stmt.hpp"
#include "StmtVisitor.hpp"
#include <memory>
#include "Expr/Expr.hpp"
#include "Token.hpp"

class Return : public Stmt {
public:
    Return(Token keyword, std::unique_ptr<Expr> value);
    void accept(StmtVisitor<void>& visitor) const override;

    bool hasValue() const { return value.get() != nullptr; }
    const Expr& getValue() const { return *value; }
    const Token getKeyword() const { return keyword; }

private:
    Token keyword;
    std::unique_ptr<Expr> value;
};
